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

    // Create the physics world
    _world = ObstacleWorld::alloc(Rect(Vec2::ZERO, _size/DRAW_SCALE),Vec2::ZERO);
	activateWorldCollisions();

    // We can only activate a button AFTER it is added to a scene
     _ballistaTOcastle->activate(input.generateKey("ballistaTOcastle"));

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
        _enemiesToFree.clear();
    }
}

void BallistaScene::update(float deltaTime, int direction){
    _direction = direction;
    //moves enemies
    for(int i = 0; i<gameModel._enemyArrayMaster.size(); i++){
		for(int j = 0; j<gameModel._enemyArrayMaster[i].size(); j++){
			if(gameModel._enemyArrayMaster[i][j][1] < 85){
				//remove
				_enemiesToFree.push_back(j);
				gameModel._enemiesToFreeMaster[i].push_back(j);
				gameModel.changeWallHealth(i, -9);
			}
			else{
				gameModel._enemyArrayMaster[i][j][1] -= 0.5;
			}
		}
	}

    if(gameModel._newSpawn.size()>0 && (int)(gameModel._newSpawn[4]) == direction){
        std::shared_ptr<EnemyModel> e = EnemyModel::alloc(Vec2(gameModel._newSpawn[0], gameModel._newSpawn[1]),
                                      -M_PI/2, gameModel._newSpawn[2], gameModel._newSpawn[3], DRAW_SCALE,_assets);
        if(e != nullptr) {
            _enemyArray.push_back(e);
            _world->addObstacle(e);
            addChild(e->getNode());
        }
    }
    gameModel._newSpawn.clear();
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

	// Update enemies and mark out of bound ones for deletion
	for (int it = 0; it < _enemyArray.size(); it++) {
		std::shared_ptr<EnemyModel> e = _enemyArray[it];
		if (e != nullptr) {
			e->update(deltaTime);
		}
		if (!bounds.contains(e->getPosition())) {
			_enemiesToFree.push_back(it);
			gameModel._enemiesToFreeMaster[(direction)].push_back(it);
			gameModel.changeWallHealth((int)(direction), -9);
		}
	}
	// Delete the enemies here because you can't remove elements while iterating
	for (int i = 0; i<_enemiesToFree.size(); i++) {
		std::shared_ptr<EnemyModel> e = _enemyArray[_enemiesToFree[i]];
		_world->removeObstacle(e.get());
		removeChild(e->getNode());
		_enemyArray.erase(_enemyArray.begin() + _enemiesToFree[i]);
	}
	_enemiesToFree.clear();
	for (int i = 0; i<gameModel._enemiesToFreeMaster[(direction)].size(); i++){
	    gameModel._enemyArrayMaster[(direction)].erase(gameModel._enemyArrayMaster[(direction)].begin() + gameModel._enemiesToFreeMaster[(direction)][i]);
	}
	gameModel._enemiesToFreeMaster[(direction)].clear();

    //crank the physics engine
    _world->update(deltaTime);

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
        _enemyArray.clear();
		//create all the enemies here from gameModel
		for (int i = 0; i<gameModel._enemyArrayMaster[(direction)].size(); i++) {
			std::shared_ptr<EnemyModel> e = EnemyModel::alloc(Vec2(gameModel._enemyArrayMaster[(direction)][i][0], gameModel._enemyArrayMaster[(direction)][i][1]),
				-M_PI/2, gameModel._enemyArrayMaster[(direction)][i][2], gameModel._enemyArrayMaster[(direction)][i][3], DRAW_SCALE, _assets);
			if (e != nullptr) {
				_enemyArray.push_back(e);
				_world->addObstacle(e);
				addChild(e->getNode());
			}
		}
    }
    else{
        for(int i = 0; i<_enemyArray.size(); i++){
            removeChild(_enemyArray[i]->getNode());
            _world->removeObstacle(_enemyArray[i].get());
        }
        _ballistaTOcastle->deactivate();
		_enemyArray.clear();
		_enemiesToFree.clear();

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
				_enemiesToFree.push_back(it);
				gameModel._enemiesToFreeMaster[(_direction)].push_back(it);
				break;
			}
			else{
				gameModel._enemyArrayMaster[_direction][it][3]--;
			}

		}
	}
}
