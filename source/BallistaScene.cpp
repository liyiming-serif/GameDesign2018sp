//
// Created by Yiming on 2/25/2018.
//
#include <stdlib.h>
#include "BallistaScene.h"
#define BALLISTA    1
#define OVERWORLD   2
#define LOOKOUT     3
#define REPAIR      4
#define MENU        5
#define AMMO        6
#define MAGE        7
#define OIL         8
#define DRAW_SCALE 32
#define FONT    _assets->get<Font>("langdon")

#define BUTTON_SCALE 1.0f

using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

bool BallistaScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Set display size
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;

    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }

    _direction = -1;

    // Set background color
    Application::get()->setClearColor(Color4(132,180,113,255));

    // Set the assets
    switchscene = 0;
    _assets = assets;


    // Set the background image
    std::shared_ptr<Texture> bktexture  = _assets->get<Texture>("ballista_view");
    _background = PolygonNode::allocWithTexture(bktexture);
    _background->setScale(.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);

    // Get the ballista image and attach it to the animated model
	_ballista = BallistaModel::alloc(Vec2(_size.width / 2, _background->getContentHeight() / 10),_assets);

    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    _ballistaTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    _ballistaTOcastle->setScale(.8f); // Magic number to rescale asset

    // Create a callback function for the OVERWORLD button
    _ballistaTOcastle->setName("ballistaTOcastle");
    _ballistaTOcastle->setListener([=] (const std::string& name, bool down) {
        if (!down) {
            switchscene = OVERWORLD;
        }
    });

    // Position the OVERWORLD button in the bottom left
    _ballistaTOcastle->setAnchor(Vec2::ANCHOR_CENTER);
    _ballistaTOcastle->setPosition(100,80);
    

    // Add children to the scene graph
    addChild(_background);
	if (_ballista != nullptr) {
		addChild(_ballista->getNode());
	}
    addChild(_ballistaTOcastle);

    // Create the arrows set
    _arrows.clear();
	_arrowsToFree.clear();

	// Create the enemy set
	_enemyArray.clear();

    // Create the physics world
    _world = ObstacleWorld::alloc(Rect(Vec2::ZERO, _size/DRAW_SCALE),Vec2::ZERO);
	activateWorldCollisions();

    // We can only activate a button AFTER it is added to a scene
     _ballistaTOcastle->activate(input.generateKey("ballistaTOcastle"));
    
    
    std::shared_ptr<Texture> north_compass  = _assets->get<Texture>("N_compass");
    N_compass = PolygonNode::allocWithTexture(north_compass);
    N_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    N_compass->setAnchor(Vec2::ANCHOR_CENTER);
    N_compass->setPosition(900,80);
    addChild(N_compass);
    N_compass->setVisible(false);
    
    std::shared_ptr<Texture> northeast_compass  = _assets->get<Texture>("NE_compass");
    NE_compass = PolygonNode::allocWithTexture(northeast_compass);
    NE_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    NE_compass->setAnchor(Vec2::ANCHOR_CENTER);
    NE_compass->setPosition(900,80);
    addChild(NE_compass);
    NE_compass->setVisible(false);
    
    std::shared_ptr<Texture> northwest_compass  = _assets->get<Texture>("NW_compass");
    NW_compass = PolygonNode::allocWithTexture(northwest_compass);
    NW_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    NW_compass->setAnchor(Vec2::ANCHOR_CENTER);
    NW_compass->setPosition(900,80);
    addChild(NW_compass);
    NW_compass->setVisible(false);
    
    std::shared_ptr<Texture> south_compass  = _assets->get<Texture>("S_compass");
    S_compass = PolygonNode::allocWithTexture(south_compass);
    S_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    S_compass->setAnchor(Vec2::ANCHOR_CENTER);
    S_compass->setPosition(900,80);
    addChild(S_compass);
    S_compass->setVisible(false);
    
    std::shared_ptr<Texture> southeast_compass  = _assets->get<Texture>("SE_compass");
    SE_compass = PolygonNode::allocWithTexture(southeast_compass);
    SE_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    SE_compass->setAnchor(Vec2::ANCHOR_CENTER);
    SE_compass->setPosition(900,80);
    addChild(SE_compass);
    SE_compass->setVisible(false);
    
    std::shared_ptr<Texture> southwest_compass  = _assets->get<Texture>("SW_compass");
    SW_compass = PolygonNode::allocWithTexture(southwest_compass);
    SW_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    SW_compass->setAnchor(Vec2::ANCHOR_CENTER);
    SW_compass->setPosition(900,80);
    addChild(SW_compass);
    SW_compass->setVisible(false);
    


    _ammoText =Label::alloc((std::string) "                                              ", FONT);
    addChild(_ammoText);
    _ammoText->setAnchor(Vec2::ANCHOR_CENTER);
    _ammoText->setPosition(700, 400);
    _ammoText->setForeground(cugl::Color4(0,0,0,255));

    return true;
}

void BallistaScene::dispose() {
    if (_active) {
        if (_world != nullptr) {
            _world->clear();
            _world = nullptr;
        }
        removeAllChildren();
		_ballista = nullptr;
        _arrows.clear();
		_arrowsToFree.clear();
        _active = false;
        _enemyArray.clear();
    }
}

void BallistaScene::setCompass(int direction){
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

void BallistaScene::update(float deltaTime, int direction){
    _direction = direction;
	_ammoText->setText("Ammo "+ std::to_string(gameModel.getArrowAmmo(0)));

    //moves enemies
    for(int i = 0; i<gameModel._enemyArrayMaster.size(); i++){
		for(int j = 0; j<gameModel._enemyArrayMaster[i].size(); j++){
			if(gameModel._enemyArrayMaster[i][j][1] < 85){
				//remove
				gameModel._enemiesToFreeMaster[i].push_back(j);
				gameModel.changeWallHealth(i, -9);
			}
			else{
				gameModel._enemyArrayMaster[i][j][1] -= 0.5;
			}
		}
	}

	// Poll inputs
    if(input.isPressed()){
		if (_ballista->isReadyToFire) {
			Vec2 pointdir = screenToWorldCoords(input.dTouch())-screenToWorldCoords(input.pointerPos());
			_ballista->setAngle(pointdir.getAngle());
			_ballista->setPower(2.0f*pointdir.length() / (float)DRAW_SCALE, false);
		}
    }
    if(input.justReleased()){
		if (_ballista->isReadyToFire) {
			// Fire ballista
			_ballista->setPower(0.0f, true);
			_ballista->isReadyToFire = false;

            if (gameModel.getArrowAmmo(0)>0) {
                // Allocate a new arrow in memory
                std::shared_ptr<ArrowModel> a = ArrowModel::alloc(_ballista->getPosition(), _ballista->getAngle(), DRAW_SCALE, _assets);

                if (a != nullptr) {
                    _arrows.insert(a);
                    _world->addObstacle(a);
                    addChild(a->getNode());
                    gameModel.setArrowAmmo(0,gameModel.getArrowAmmo(0)-1);
                }
            }
		}
    }

	_ballista->update(deltaTime);

    // Update arrows and mark out of bound ones for deletion
    Rect bounds(Vec2::ZERO, _size/DRAW_SCALE);
    for(auto it = _arrows.begin(); it != _arrows.end(); it++){
        std::shared_ptr<ArrowModel> a = *it;
        if(a != nullptr) {
            a->update(deltaTime);
        }
        if(!bounds.contains(a->getPosition())){
			_arrowsToFree.insert(a);
        }
    }

	// Delete the arrows here because you can't remove elements while iterating
	for (auto it = _arrowsToFree.begin(); it != _arrowsToFree.end(); it++) {
		std::shared_ptr<ArrowModel> a = *it;
		_world->removeObstacle(a.get());
		removeChild(a->getNode());
		_arrows.erase(a);
	}
	_arrowsToFree.clear();

	//delete enemies here to not disrupt iterator
	for (int i = 0; i<gameModel._enemiesToFreeMaster.size(); i++) {
		for (int j = 0; j < gameModel._enemiesToFreeMaster[i].size(); j++) {
			if(j<gameModel._enemyArrayMaster[i].size()){
				gameModel._enemyArrayMaster[i].erase(gameModel._enemyArrayMaster[i].begin() + gameModel._enemiesToFreeMaster[i][j]);
			}
		}
		gameModel._enemiesToFreeMaster[i].clear();
	}

	//reflect enemy changes in enemy models
	updateEnemyModels(direction);

    //crank the physics engine
    _world->update(deltaTime);

}

void BallistaScene::updateEnemyModels(int direction) {
	//clear enemy models
	for (int i = 0; i < _enemyArray.size(); i++) {
		std::shared_ptr<EnemyModel> e = _enemyArray[i];
		_world->removeObstacle(e.get());
		removeChild(e->getNode());
	}
	_enemyArray.clear();

	//refresh enemy models from the master array
	for (int i = 0; i<gameModel._enemyArrayMaster[(direction)].size(); i++) {
		std::shared_ptr<EnemyModel> e = EnemyModel::alloc(Vec2(gameModel._enemyArrayMaster[(direction)][i][0], gameModel._enemyArrayMaster[(direction)][i][1]),
			-M_PI / 2, gameModel._enemyArrayMaster[(direction)][i][2], gameModel._enemyArrayMaster[(direction)][i][3], DRAW_SCALE, _assets);
		if (e != nullptr) {
			_enemyArray.push_back(e);
			_world->addObstacle(e);
			addChild(e->getNode());
		}
	}
}

//Pause or Resume
void BallistaScene::setActive(bool active, int direction){
    _direction = direction;
    _active = active;
    switchscene = 0;

	//empty the arrow arrays to prevent data leeks
	for (auto it = _arrows.begin(); it != _arrows.end(); it++) {
		std::shared_ptr<ArrowModel> a = *it;
		_world->removeObstacle(a.get());
		removeChild(a->getNode());
		//refund their arrows
		gameModel.setArrowAmmo(0, gameModel.getArrowAmmo(0) + 1);
	}
    _arrows.clear();
	for (auto it = _arrowsToFree.begin(); it != _arrowsToFree.end(); it++) {
		std::shared_ptr<ArrowModel> a = *it;
		_world->removeObstacle(a.get());
		removeChild(a->getNode());
	}
	_arrowsToFree.clear();

    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _ballistaTOcastle->activate(input.findKey("ballistaTOcastle"));
        BallistaScene::setCompass(direction);
    }
    else{
		//clear enemy models
		for (int i = 0; i < _enemyArray.size(); i++) {
			std::shared_ptr<EnemyModel> e = _enemyArray[i];
			_world->removeObstacle(e.get());
			removeChild(e->getNode());
		}
		_enemyArray.clear();

        _ballistaTOcastle->deactivate();
    }
}

void BallistaScene::activateWorldCollisions() {
	_world->activateCollisionCallbacks(true);
	_world->onBeginContact = [this](b2Contact* contact) {
		beginContact(contact);
	};
}

//TODO: DISABLE PHYSICS ENGINE DURING COLLISIONS
void BallistaScene::beginContact(b2Contact* contact) {
	b2Body* body1 = contact->GetFixtureA()->GetBody();
	b2Body* body2 = contact->GetFixtureB()->GetBody();

	for (auto it = _arrows.begin(); it != _arrows.end(); it++) {
		std::shared_ptr<ArrowModel> a = *it;
		if ((body1->GetUserData() == a.get() || body2->GetUserData() == a.get())) {
			_arrowsToFree.insert(a);
			break;
		}
	}

	for (int it = 0; it<_enemyArray.size(); it++) {
		std::shared_ptr<EnemyModel> e = _enemyArray[it];
		if (body1->GetUserData() == e.get() || body2->GetUserData() == e.get()) {
			if(gameModel._enemyArrayMaster[_direction][it][3] <= 1){
				gameModel._enemiesToFreeMaster[(_direction)].push_back(it);
				break;
			}
			else{
				gameModel._enemyArrayMaster[_direction][it][3]--;
			}

		}
	}
}
