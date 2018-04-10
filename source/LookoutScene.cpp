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
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(0,0);
    addChild(_background);

    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);

	//allocate an enemy icon, but don't add it yet
	_enemyIcon = _assets->get<Texture>("skeletonIcon");
	//initiallize lanes for displaying enemies.
	for (int i = 0; i < 6; i++) {
		std::shared_ptr<cugl::Node> ecanvas = Node::allocWithBounds(_size);
		ecanvas->setAnchor(Vec2(0.5f,-0.3f));
		ecanvas->setPosition(_size.width / 2, _size.height / 2);
		ecanvas->setAngle(i * 2 * M_PI / 6);
		ecanvas->setScale(0.1f,0.4f);
		addChild(ecanvas);
		_enemyMarkers.push_back(ecanvas);
	}

    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    _lookoutTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    _lookoutTOcastle->setScale(.8f); // Magic number to rescale asset

    // Create a callback function for the OVERWORLD button
    _lookoutTOcastle->setName("lookoutTOcastle");
    _lookoutTOcastle->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
        }
    });


    // Position the overworld button in the bottom left
    _lookoutTOcastle->setAnchor(Vec2::ANCHOR_CENTER);
    _lookoutTOcastle->setPosition(100,80);

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
	//moves enemies and marks out of bounds ones for deletion
	for (int i = 0; i<gameModel._enemyArrayMaster.size(); i++) {
		for (int j = 0; j<gameModel._enemyArrayMaster[i].size(); j++) {
			if (gameModel._enemyArrayMaster[i][j][1] < 85) {
				//remove
				gameModel._enemiesToFreeMaster[i].push_back(j);
				gameModel.changeWallHealth(i, -9);
			}
			else {
				gameModel._enemyArrayMaster[i][j][1] -= 0.5;
			}
		}
	}
	//delete enemies here to not disrupt iterator
	for (int i = 0; i<gameModel._enemiesToFreeMaster.size(); i++) {
		for (int j = 0; j < gameModel._enemiesToFreeMaster[i].size(); j++) {
			if (j<gameModel._enemyArrayMaster[i].size()) {
				gameModel._enemyArrayMaster[i].erase(gameModel._enemyArrayMaster[i].begin() + gameModel._enemiesToFreeMaster[i][j]);
			}
		}
		gameModel._enemiesToFreeMaster[i].clear();
	}
	updateEnemyMarkers();
}

void LookoutScene::updateEnemyMarkers() {
	//clear enemy lanes
	for (int i = 0; i < _enemyMarkers.size(); i++) {
		_enemyMarkers[i]->removeAllChildren();
	}

	for (int i = 0; i < gameModel._enemyArrayMaster.size(); i++) {
		//add enemies by direction
		for (int j = 0; j < gameModel._enemyArrayMaster[i].size(); j++) {
			std::shared_ptr<PolygonNode> e = PolygonNode::allocWithTexture(_enemyIcon);
			e->setAnchor(Vec2::ANCHOR_CENTER);
			e->setPosition(gameModel._enemyArrayMaster[i][j][0],gameModel._enemyArrayMaster[i][j][1]);
			_enemyMarkers[i]->addChild(e);
		}
	}
}

//Pause or Resume
void LookoutScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _lookoutTOcastle->activate(input.findKey("lookoutTOcastle"));
    }
    else{
        _lookoutTOcastle->deactivate();

		for (int i = 0; i < _enemyMarkers.size(); i++) {
			_enemyMarkers[i]->removeAllChildren();
		}
    }
}
