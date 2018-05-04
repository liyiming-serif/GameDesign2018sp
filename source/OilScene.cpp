//
//  OilScene.c
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "OilScene.h"
using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

#define BALLISTA    1
#define OVERWORLD   2
#define LOOKOUT     3
#define REPAIR      4
#define MENU        5
#define AMMO        6
#define MAGE        7
#define OIL         8
#define LEVELS      9
#define LOBBY       10
#define WIN         11
#define LOSE        12

#define JUNGLE  5
#define SNOW  8

#define BUTTON_SCALE .6f
#define DRAW_SCALE 32

#define OIL_COOLDOWN 420
#define TIPPING_POINT 0.45f

#define OIL_MAX_RANGE 191	//farthest enemy oil scene can see
#define OIL_MIN_RANGE 0		//closest enemy oil scene can see
#define OIL_END_ZONE 127	//enemies dissapear past this y-coord; set by castle wall art assets

bool OilScene::inRange(float y) {
	return OIL_MIN_RANGE < y && y < OIL_MAX_RANGE;
}

float OilScene::calcY(float y) {
	int range = OIL_MAX_RANGE - OIL_MIN_RANGE;
	int screen_range = _size.height - OIL_END_ZONE;
	return (y - OIL_MIN_RANGE) / range*(screen_range)+OIL_END_ZONE;
}

bool OilScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }
    
    // Set background color
    Application::get()->setClearColor(Color4(132,180,113,255));
    
    switchscene = 0;
    
    _assets = assets;
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("weaponBG_jungle");
    std::shared_ptr<Texture> texture_s  = _assets->get<Texture>("weaponBG_snow");
    std::shared_ptr<Texture> texture_d  = _assets->get<Texture>("weaponBG_desert");
    if (gameModel.level<JUNGLE) {
        _background = PolygonNode::allocWithTexture(texture);
    }
    else if (gameModel.level<SNOW) {
        _background = PolygonNode::allocWithTexture(texture_s);
    }
    else {
        _background = PolygonNode::allocWithTexture(texture_d);
    }
    _background->setScale(0.54f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);


     std::shared_ptr<Texture> wallGreen  = _assets->get<Texture>("oil_wall_GREEN");
    _wall_GREEN = PolygonNode::allocWithTexture(wallGreen);
    _wall_GREEN->setScale(0.54f); // Magic number to rescale asset
    _wall_GREEN->setAnchor(Vec2::ANCHOR_CENTER);
    _wall_GREEN->setPosition(_size.width/2,_size.height/2);
    
    std::shared_ptr<Texture> wallYellow  = _assets->get<Texture>("oil_wall_YELLOW");
    _wall_YELLOW = PolygonNode::allocWithTexture(wallYellow);
    _wall_YELLOW->setScale(0.54f); // Magic number to rescale asset
    _wall_YELLOW->setAnchor(Vec2::ANCHOR_CENTER);
    _wall_YELLOW->setPosition(_size.width/2,_size.height/2);
    
    std::shared_ptr<Texture> wallOrange  = _assets->get<Texture>("oil_wall_ORANGE");
    _wall_ORANGE = PolygonNode::allocWithTexture(wallOrange);
    _wall_ORANGE->setScale(0.54f); // Magic number to rescale asset
    _wall_ORANGE->setAnchor(Vec2::ANCHOR_CENTER);
    _wall_ORANGE->setPosition(_size.width/2,_size.height/2);
    
    std::shared_ptr<Texture> wallRed  = _assets->get<Texture>("oil_wall_RED");
    _wall_RED = PolygonNode::allocWithTexture(wallRed);
    _wall_RED->setScale(0.54f); // Magic number to rescale asset
    _wall_RED->setAnchor(Vec2::ANCHOR_CENTER);
    _wall_RED->setPosition(_size.width/2,_size.height/2);



	// Create the enemy set
	_enemyArray.clear();
	_enemiesToFree.clear();

	for(int i = 0; i<6; i++){
	gameModel.setOilCooldown(i, 0);
	}
    
	// Create the physics world
	_world = ObstacleWorld::alloc(Rect(Vec2::ZERO, _size / DRAW_SCALE), Vec2::ZERO);

    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    _oilTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
   
    // Create a callback function for the OVERWORLD button
    _oilTOcastle->setName("oilTOcastle");
    _oilTOcastle->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
        }
    });
    
    
    // Position the overworld button in the bottom left
    _oilTOcastle->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _oilTOcastle->setPosition(15,_size.height-18);
    _oilTOcastle->setScale(.6f);
    
    // Add the logo and button to the scene graph
    addChild(_background);
    addChild(_wall_GREEN);
    addChild(_wall_YELLOW);
    addChild(_wall_ORANGE);
    addChild(_wall_RED);
    addChild(_oilTOcastle);
    
    
    // Set the oil model
    _oil = OilModel::alloc(Vec2(_size.width / 2, _background->getContentHeight()*0.11f), _assets);
    if (_oil != nullptr) {
        addChild(_oil->getNode());
    }
    
    // We can only activate a button AFTER it is added to a scene
    _oilTOcastle->activate(input.generateKey("oilTOcastle"));
   
    
    std::shared_ptr<Texture> north_compass  = _assets->get<Texture>("bigC");
    N_compass = PolygonNode::allocWithTexture(north_compass);
    N_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    N_compass->setAnchor(Vec2::ANCHOR_CENTER);
    N_compass->setPosition(950,72.2);
    addChild(N_compass);
    N_compass->setVisible(false);
    
    std::shared_ptr<Texture> northeast_compass  = _assets->get<Texture>("smallC");
    NE_compass = PolygonNode::allocWithTexture(northeast_compass);
    NE_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    NE_compass->setAnchor(Vec2::ANCHOR_CENTER);
    NE_compass->setPosition(950,72.2);
    addChild(NE_compass);
    NE_compass->setVisible(false);
    
    std::shared_ptr<Texture> northwest_compass  = _assets->get<Texture>("smallC");
    NW_compass = PolygonNode::allocWithTexture(northwest_compass);
    NW_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    NW_compass->setAnchor(Vec2::ANCHOR_CENTER);
    NW_compass->setPosition(950,72.2);
    NW_compass->setAngle(M_PI/2);
    addChild(NW_compass);
    NW_compass->setVisible(false);
    
    std::shared_ptr<Texture> south_compass  = _assets->get<Texture>("bigC");
    S_compass = PolygonNode::allocWithTexture(south_compass);
    S_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    S_compass->setAnchor(Vec2::ANCHOR_CENTER);
    S_compass->setPosition(950,72.2);
    S_compass->setAngle(M_PI);
    addChild(S_compass);
    S_compass->setVisible(false);
    
    std::shared_ptr<Texture> southeast_compass  = _assets->get<Texture>("smallC");
    SE_compass = PolygonNode::allocWithTexture(southeast_compass);
    SE_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    SE_compass->setAnchor(Vec2::ANCHOR_CENTER);
    SE_compass->setPosition(950,72.2);
    SE_compass->setAngle(-M_PI/2);
    addChild(SE_compass);
    SE_compass->setVisible(false);
    
    std::shared_ptr<Texture> southwest_compass  = _assets->get<Texture>("smallC");
    SW_compass = PolygonNode::allocWithTexture(southwest_compass);
    SW_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    SW_compass->setAnchor(Vec2::ANCHOR_CENTER);
    SW_compass->setPosition(950,72.2);
    SW_compass->setAngle(M_PI);
    addChild(SW_compass);
    SW_compass->setVisible(false);
    
    

    return true;
}

void OilScene::dispose() {
    if (_active) {
		if (_world != nullptr) {
			_world->clear();
			_world = nullptr;
		}
        removeAllChildren();
        _assets = nullptr;
        _oilTOcastle = nullptr;
        _background = nullptr;
		_oil = nullptr;
        _active = false;
		_enemyArray.clear();
		_enemiesToFree.clear();
    }
}

void OilScene::setCompass(int direction){
    N_compass->setVisible(false);
    NE_compass->setVisible(false);
    NW_compass->setVisible(false);
    S_compass->setVisible(false);
    SE_compass->setVisible(false);
    SW_compass->setVisible(false);
    if (direction == 0) {
        N_compass->setVisible(true);
    }
    else if (direction == 1){
        NW_compass->setVisible(true);
    }
    else if (direction == 2){
        SW_compass->setVisible(true);
    }
    else if (direction == 3){
        S_compass->setVisible(true);
    }
    else if (direction == 4){
        SE_compass->setVisible(true);
    }
    else if (direction == 5){
        NE_compass->setVisible(true);
    }
}

void OilScene::setWall(int direction){
    _wall_GREEN->setVisible(false);
    _wall_YELLOW->setVisible(false);
    _wall_ORANGE->setVisible(false);
    _wall_RED->setVisible(false);
    if (gameModel.getWallHealth(direction) < 25){
        _wall_RED->setVisible(true);
    }
    else if (gameModel.getWallHealth(direction) < 50){
        _wall_ORANGE->setVisible(true);
    }
    else if (gameModel.getWallHealth(direction) < 75){
        _wall_YELLOW->setVisible(true);
    }
    else {
         _wall_GREEN->setVisible(true);
    }
    

    
    if (direction == 0) {
        N_compass->setVisible(true);
    }
    else if (direction == 1){
        NW_compass->setVisible(true);
    }
    else if (direction == 2){
        SW_compass->setVisible(true);
    }
    else if (direction == 3){
        S_compass->setVisible(true);
    }
    else if (direction == 4){
        SE_compass->setVisible(true);
    }
    else if (direction == 5){
        NE_compass->setVisible(true);
    }
}


void OilScene::update(float timestep, int direction){
    
    if (gameModel.getWallHealth(0) == 0 || gameModel.getWallHealth(1) == 0 || gameModel.getWallHealth(2) == 0 ||
        gameModel.getWallHealth(3) == 0 || gameModel.getWallHealth(4) == 0 || gameModel.getWallHealth(5) == 0) {
        switchscene = LOSE;
    }
    if (gameModel._currentTime > gameModel._endTime){
        if (gameModel._enemyArrayMaster[0].size()== 0 && gameModel._enemyArrayMaster[1].size()== 0 && gameModel._enemyArrayMaster[2].size()== 0 && gameModel._enemyArrayMaster[3].size()== 0 && gameModel._enemyArrayMaster[4].size()== 0 && gameModel._enemyArrayMaster[5].size()== 0) {
            switchscene = WIN;
        }
    }
    setWall(direction);
    
	//poll inputs
	if (gameModel.getOilCooldown(direction) == 0 && input.oilTilt()>=TIPPING_POINT && !_oil->isReloading) {
		gameModel.setOilCooldown(direction, OIL_COOLDOWN);
		_oil->isReloading = true;
		//OIL SPILT! Obliterate all enemies!
		for (std::pair<std::string, std::shared_ptr<EnemyModel>> epair : _enemyArray) {
			std::shared_ptr<EnemyModel> e = epair.second;
			std::string k = epair.first;
			std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>::iterator i =
				gameModel._enemyArrayMaster[direction].find(k);
			if (i != gameModel._enemyArrayMaster[direction].end()) {
				std::shared_ptr<EnemyDataModel> ed = i->second;
				gameModel._enemiesToFreeMaster[direction].push_back(k);
				CULog("destroyed!");
			}
		}
	}
	float cooldown = (float)(gameModel.getOilCooldown(direction)) / (float)(OIL_COOLDOWN);
	float tilt = std::fmin(input.oilTilt(), TIPPING_POINT) / TIPPING_POINT;
  
	//advance frame
	_oil->update(cooldown, tilt);

	updateEnemyModels(timestep, direction);

	//crank the physics engine
	_world->update(timestep);
}

void OilScene::updateEnemyModels(float timestep, int direction) {
	//add or update enemy models from master array
	for (std::pair<std::string, std::shared_ptr<EnemyDataModel>> epair : gameModel._enemyArrayMaster[direction]) {
		std::string k = epair.first;
		std::shared_ptr<EnemyDataModel> e = epair.second;

		std::unordered_map<std::string, std::shared_ptr<EnemyModel>>::iterator i =
			_enemyArray.find(k);
		if (i == _enemyArray.end()) {
			if (inRange(e->getPos().y)) {
				Vec2 pos = Vec2(e->getPos().x, calcY(e->getPos().y));
				std::shared_ptr<EnemyModel> en = EnemyModel::alloc(k, pos, e->getType(), DRAW_SCALE, _assets);
				if (en != nullptr) {
					_enemyArray[k] = en;
					_world->addObstacle(en);
					addChild(en->getNode());
				}
			}
		}
		else {
			Vec2 pos = Vec2(e->getPos().x, calcY(e->getPos().y));
			i->second->setPosition(pos / DRAW_SCALE);
			i->second->update(timestep);
		}
	}

	//delete enemy models too close or not found in the master 
	for (std::pair<std::string, std::shared_ptr<EnemyModel>> e : _enemyArray) {
		std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>::iterator i =
			gameModel._enemyArrayMaster[direction].find(e.first);
		if (i == gameModel._enemyArrayMaster[direction].end() || !inRange(i->second->getPos().y)) {
			//mark enemy models for deletion
			_enemiesToFree.insert(e.second);
		}
	}
	// Delete the enemies here because you can't remove elements while iterating
	for (auto it = _enemiesToFree.begin(); it != _enemiesToFree.end(); it++) {
		std::shared_ptr<EnemyModel> e = *it;
		_world->removeObstacle(e.get());
		removeChild(e->getNode());
		_enemyArray.erase(e->getName());
	}
	_enemiesToFree.clear();
}

//Pause or Resume
void OilScene::setActive(bool active, int direction){
    _active = active;
    switchscene = 0;
	_oil->isReloading = false;

	//empty the enemy arrays to prevent data leaks
	for (std::pair<std::string, std::shared_ptr<EnemyModel>> epair : _enemyArray) {
		std::shared_ptr<EnemyModel> e = epair.second;
		_world->removeObstacle(e.get());
		removeChild(e->getNode());
	}
	_enemyArray.clear();

	for (auto it = _enemiesToFree.begin(); it != _enemiesToFree.end(); it++) {
		std::shared_ptr<EnemyModel> e = *it;
		_world->removeObstacle(e.get());
		removeChild(e->getNode());
		_enemyArray.erase(e->getName());
	}
	_enemiesToFree.clear();

    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(255,255,255,255));
        _oilTOcastle->activate(input.findKey("oilTOcastle"));
         OilScene::setCompass(direction);
         OilScene::setWall(direction);
        
        
    }
    else{
        _oilTOcastle->deactivate();
    }
}
