//
// Created by Yiming on 3/2/2018.
//

#include "LookoutScene.h"
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


#define JUNGLE  2
#define SNOW  3

bool LookoutScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("lookout_view");
    std::shared_ptr<Texture> texture_s  = _assets->get<Texture>("lookout_view_s");
    std::shared_ptr<Texture> texture_d  = _assets->get<Texture>("lookout_view_d");
    if (gameModel.level<JUNGLE) {
        _background = PolygonNode::allocWithTexture(texture);
    }
    else if (gameModel.level<SNOW) {
         _background = PolygonNode::allocWithTexture(texture_s);
    }
    else {
         _background = PolygonNode::allocWithTexture(texture_d);
    }
    _background->setScale(0.5342f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    addChild(_background);
    
    
    

    
    // Set the background image
    std::shared_ptr<Texture> texture2  = _assets->get<Texture>("lookout_progressBar");
    _progressBar = PolygonNode::allocWithTexture(texture2);
    _progressBar->setScale(0.525f); // Magic number to rescale asset
    _progressBar->setAnchor(Vec2::ANCHOR_TOP_CENTER);
    _progressBar->setPosition(_size.width-(_progressBar->getWidth()/2),_size.height*.09f);
    addChild(_progressBar);
    
    _distance=.85f*_size.height/gameModel.getEndTime();
    
    
	//allocate an enemy icon, but don't add it yet
	_enemyIcon = _assets->get<Texture>("skeletonIcon");
	//initialize lanes for displaying enemies.
	for (int i = 0; i < 6; i++) {
		std::shared_ptr<cugl::Node> ecanvas = Node::allocWithBounds(_size);
		ecanvas->setAnchor(Vec2(0.5f,-0.5f));
		ecanvas->setPosition(_size.width / 2, _size.height / 2);
		ecanvas->setAngle(i * 2 * M_PI / 6);
		ecanvas->setScale(0.1f,0.4f);
		addChild(ecanvas);
		_enemyMarkers.push_back(ecanvas);
	}

    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    _lookoutTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    _lookoutTOcastle->setScale(.6f); // Magic number to rescale asset

    // Create a callback function for the OVERWORLD button
    _lookoutTOcastle->setName("lookoutTOcastle");
    _lookoutTOcastle->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
        }
    });


    // Position the overworld button in the bottom left
    _lookoutTOcastle->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _lookoutTOcastle->setPosition(15,_size.height-18);

    // Add the logo and button to the scene graph
    addChild(_lookoutTOcastle);

    // We can only activate a button AFTER it is added to a scene
    _lookoutTOcastle->activate(input.generateKey("lookoutTOcastle"));

    return true;
}

void LookoutScene::dispose() {
    if (_active) {
		for (int i = 0; i < _enemyMarkers.size(); i++) {
			_enemyMarkers[i]->removeAllChildren();
		}
        removeAllChildren();
        _assets = nullptr;
        _lookoutTOcastle = nullptr;
        _background = nullptr;
        _active = false;
		_enemyIcon = nullptr;
		_enemyMarkers.clear();
    }
}

void LookoutScene::update(float timestep){
    
    CULog("end time %f", gameModel.getEndTime());
    CULog("current time %f", gameModel.getCurrentTime());
    CULog("delta distacne %f", _distance);
    CULog("height %f", _progressBar->getPositionY());
    
    if (gameModel.getWallHealth(0) == 0 || gameModel.getWallHealth(1) == 0 || gameModel.getWallHealth(2) == 0 ||
        gameModel.getWallHealth(3) == 0 || gameModel.getWallHealth(4) == 0 || gameModel.getWallHealth(5) == 0) {
        switchscene = LOSE;
    }
    if (gameModel._currentTime > gameModel._endTime){
        if (gameModel._enemyArrayMaster[0].size()== 0 && gameModel._enemyArrayMaster[1].size()== 0 && gameModel._enemyArrayMaster[2].size()== 0 && gameModel._enemyArrayMaster[3].size()== 0 && gameModel._enemyArrayMaster[4].size()== 0 && gameModel._enemyArrayMaster[5].size()== 0) {
                switchscene = WIN;
        }
    }
    if (_progressBar->getPositionY()<_size.height) {
        _progressBar->setPosition(_progressBar->getPositionX(),_progressBar->getPositionY()+_distance);
    }

	//UPDATE ENEMY MARKERS
	//clear enemy lanes
	for (int i = 0; i < _enemyMarkers.size(); i++) {
		_enemyMarkers[i]->removeAllChildren();
	}
	//redraw them
	for (int wall = 0; wall<gameModel._enemyArrayMaster.size(); wall++) {
		for (std::pair<std::string, std::shared_ptr<EnemyDataModel>> enemy : gameModel._enemyArrayMaster[wall]) {
			std::shared_ptr<PolygonNode> e = PolygonNode::allocWithTexture(_enemyIcon);
			e->setAnchor(Vec2::ANCHOR_CENTER);
			e->setPosition(enemy.second->getPos());
			_enemyMarkers[wall]->addChild(e);
		}
	}
}


//Pause or Resume
void LookoutScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(255,255,255,255));
        _lookoutTOcastle->activate(input.findKey("lookoutTOcastle"));
        _progressBar->setPosition(_progressBar->getPositionX(),
            std::min((_size.height*.09f+(gameModel.getCurrentTime()*_distance)),_size.height));
    }
    
    else{
        _lookoutTOcastle->deactivate();
		for (int i = 0; i < _enemyMarkers.size(); i++) {
			_enemyMarkers[i]->removeAllChildren();
		}
    }
}
