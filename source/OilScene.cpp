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

#define DELUGE_NUM_ROWS 5
#define DELUGE_NUM_COLS 5
#define DELUGE_SPEED 0.2f

#define OIL_COOLDOWN 600
#define TIPPING_POINT 0.45f

#define OIL_MAX_RANGE 127	//farthest enemy oil scene can see
#define OIL_MIN_RANGE 0		//closest enemy oil scene can see
#define OIL_END_ZONE 200	//enemies dissapear past this y-coord; set by castle wall art assets

#define DMG_DURATION 1.0f
#define DMG_ACT_KEY "marker"

// Decide when to use heavy damage indicator
#define HVY_DMG 6

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
    
	//Autosort by ZOrder
	setZAutoSort(true);

    switchscene = 0;
    
    _assets = assets;
    
	// Allocate the damage indicators
	std::shared_ptr<Texture> dmg_img = _assets->get<Texture>("dmg_indicator_n");
	std::shared_ptr<PolygonNode> dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_nw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_sw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_s");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_se");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_ne");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_n");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_nw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_sw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_s");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_se");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_ne");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);

	_dmgFadeOUT = FadeOut::alloc(DMG_DURATION);

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
	_background->setZOrder(-2);

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
    _oilTOcastle->setZOrder(-1);
    
	// Init the oil effect when it's poured
	_deluge = AnimationNode::alloc(_assets->get<Texture>("oil_wave"),DELUGE_NUM_ROWS,DELUGE_NUM_COLS);
	_deluge->setVisible(false);
	_deluge->setScale(_size.width/_deluge->getWidth(),_size.height/_deluge->getHeight());
	_deluge->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	_deluge->setPosition(0, 0);
	_delugeFrame = 0;

    // Add the logo and button to the scene graph
    addChild(_background);
	addChild(_deluge);
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
    
    
    std::shared_ptr<Texture> tut_tilt  = _assets->get<Texture>("tutorial_tilt");
    _tilt = PolygonNode::allocWithTexture(tut_tilt);
    _tilt->setScale(1.0); // Magic number to rescale asset
    _tilt->setAnchor(Vec2::ANCHOR_TOP_CENTER);
    _tilt->setPosition(_size.width*.5f,_size.height*.9f);
    addChild(_tilt);
    
    
	// Add damage indicators overlay
	for (int i = 0; i < _dmgIndicators.size(); i++) {
		addChild(_dmgIndicators.at(i));
	}

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
		_wall_GREEN = nullptr;
		_wall_YELLOW = nullptr;
		_wall_ORANGE = nullptr;
		_wall_RED = nullptr;
		_oil = nullptr;
        _active = false;
		N_compass = nullptr;
		NE_compass = nullptr;
		NW_compass = nullptr;
		S_compass = nullptr;
		SE_compass = nullptr;
		SW_compass = nullptr;
		_deluge = nullptr;
		_enemyArray.clear();
		_enemiesToFree.clear();
		_dmgFadeOUT = nullptr;
		_dmgIndicators.clear();
        _tilt=nullptr;
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
    
    if (_tiltCount > 0) {
        _tilt->setVisible(false);
    }
    
	//poll damage indicators
	pollDmgIndicators();

	//poll inputs
	if (gameModel.getOilCooldown(direction) == 0 && input.oilTilt()>=TIPPING_POINT && !_oil->isReloading) {
		gameModel.setOilCooldown(direction, OIL_COOLDOWN);
		_oil->isReloading = true;
		//OIL SPILT! Obliterate all enemies!
        if (gameModel.isNetworked() && !gameModel.isServer()) {
            gameModel.setOilPoured(direction);
        }
        _tiltCount+=1;
        CULog("OIL");
		_deluge->setVisible(true);
		_delugeFrame = 0;
		for (std::pair<std::string, std::shared_ptr<EnemyModel>> epair : _enemyArray) {
			std::shared_ptr<EnemyModel> e = epair.second;
			std::string k = epair.first;
			std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>::iterator i =
				gameModel._enemyArrayMaster[direction].find(k);
			if (i != gameModel._enemyArrayMaster[direction].end()) {
				std::shared_ptr<EnemyDataModel> ed = i->second;
				gameModel._enemiesToFreeMaster[direction].push_back(k);
                gameModel.addEnemyChange(k, 0-i->second->getHealth(), direction);
				CULog("destroyed!");
			}
		}
	}
	float cooldown = (float)(gameModel.getOilCooldown(direction)) / (float)(OIL_COOLDOWN);
	float tilt = std::fmin(input.oilTilt(), TIPPING_POINT) / TIPPING_POINT;
  
	//advance frame
	_oil->update(cooldown, tilt);

	if (_deluge!=nullptr && _deluge->isVisible()) {
		_delugeFrame += DELUGE_SPEED;
		if (_delugeFrame >= _deluge->getSize()) {
			_deluge->setVisible(false);
		}
		else {
			_deluge->setFrame(floor(_delugeFrame));
		}
	}

	updateEnemyModels(timestep, direction);

	//crank the physics engine
	_world->update(timestep);
}

void OilScene::pollDmgIndicators() {
	for (int i = 0; i < 6; i++) {
		if (gameModel.getDmgHealth(i) > 0) {
			//turn on damage indicator for that side
			bool succ;
			if (gameModel.getDmgHealth(i) > HVY_DMG) {
				_dmgIndicators.at(i + 6)->setColor(Color4::WHITE);
				succ = input.actions()->activate(DMG_ACT_KEY + i + 6, _dmgFadeOUT, _dmgIndicators.at(i + 6));
			}
			else {
				_dmgIndicators.at(i)->setColor(Color4::WHITE);
				succ = input.actions()->activate(DMG_ACT_KEY + i, _dmgFadeOUT, _dmgIndicators.at(i));
			}
			if (succ) {
				gameModel.resetWallDmg();
			}
		}
	}
}

void OilScene::updateEnemyModels(float timestep, int direction) {
	//add or update enemy models from master array
	for (std::pair<std::string, std::shared_ptr<EnemyDataModel>> epair : gameModel._enemyArrayMaster[direction]) {
		std::string k = epair.first;
		std::shared_ptr<EnemyDataModel> e = epair.second;

		std::unordered_map<std::string, std::shared_ptr<EnemyModel>>::iterator i =
			_enemyArray.find(k);
		if (i == _enemyArray.end()) { //New enemy spotted; create in scene.
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
		else { //Enemy already exists; update it.
			Vec2 pos = Vec2(e->getPos().x, calcY(e->getPos().y));
			i->second->setPosition(pos / DRAW_SCALE);
			float atkProgress = (float)e->getAtkCounter() / (float)e->getAtkSpeed();
			i->second->setAtkProgress(atkProgress);
			i->second->update(timestep);
		}
	}

	//delete enemy models too close or not found in the master 
	for (std::pair<std::string, std::shared_ptr<EnemyModel>> e : _enemyArray) {
		std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>::iterator i =
			gameModel._enemyArrayMaster[direction].find(e.first);
		if (i == gameModel._enemyArrayMaster[direction].end()) {
			if (!e.second->isDying) {
				//remove hit box for enemies playing death animation
				_world->removeObstacle(e.second.get());
				e.second->isDying = true;
			}
			else {
				//move death animation forward
				e.second->update(timestep);
			}
			if (e.second->doneDying) {
				//mark enemies with finished death animation for removal
				_enemiesToFree.insert(e.second);
			}
		}
		else if (!inRange(i->second->getPos().y)) {
			//mark enemy models for deletion
			_enemiesToFree.insert(e.second);
			_world->removeObstacle(e.second.get());
		}
	}
	// Delete the enemies here because you can't remove elements while iterating
	for (auto it = _enemiesToFree.begin(); it != _enemiesToFree.end(); it++) {
		std::shared_ptr<EnemyModel> e = *it;
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
		if (!epair.second->isDying) {
			_world->removeObstacle(e.get());
		}
		removeChild(e->getNode());
	}
	_enemyArray.clear();

	for (auto it = _enemiesToFree.begin(); it != _enemiesToFree.end(); it++) {
		std::shared_ptr<EnemyModel> e = *it;
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
        
        if (gameModel.level==5){
            _tiltTutorial=true;
        }
        else {
            _tiltTutorial=false;
        }
        if (_tiltTutorial && _tiltCount < 1) {
            _tilt->setVisible(true);
        }
        if (!_tiltTutorial) {
            _tilt->setVisible(false);
        }
        
        //Change scenery based on level
		std::shared_ptr<Texture> texture = _assets->get<Texture>("weaponBG_jungle");
		std::shared_ptr<Texture> texture_s = _assets->get<Texture>("weaponBG_snow");
		std::shared_ptr<Texture> texture_d = _assets->get<Texture>("weaponBG_desert");
		if (gameModel.level<JUNGLE) {
			_background->setTexture(texture);
		}
		else if (gameModel.level<SNOW) {
			_background->setTexture(texture_s);
		}
		else {
			_background->setTexture(texture_d);
		}
    }
    else{
		_deluge->setVisible(false);
        _oilTOcastle->deactivate();

		//wipe residual action animations
		for (auto const& it : _dmgIndicators) {
			it->setColor(Color4::CLEAR);
			input.actions()->clearAllActions(it);
		}
    }
}
