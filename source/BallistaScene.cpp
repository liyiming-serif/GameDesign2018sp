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

    _spawnTimer = 180;

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

    // Get the ballista image and attach it to a polygon obj. (no model yet)
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("ballista");
    _ballista = PolygonNode::allocWithTexture(texture);
    _ballista->setScale(.4f); // Magic number to rescale asset
    _ballista->setAnchor(Vec2::ANCHOR_CENTER);
    _ballista->setPosition(_size.width/2,_background->getContentHeight()/10);
    _ballista->setAngle(M_PI/2);


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
    addChild(_ballista);
    addChild(_ballistaTOcastle);

    // Create the arrows set
    _arrows.clear();
	_arrowsToFree.clear();

    // Create the physics world
    _world = ObstacleWorld::alloc(Rect(Vec2::ZERO, _size/DRAW_SCALE),Vec2::ZERO);
	activateWorldCollisions();

    // We can only activate a button AFTER it is added to a scene
     _ballistaTOcastle->activate(25);

    return true;
}

void BallistaScene::dispose() {
    if (_active) {
        if (_world != nullptr) {
            _world->clear();
            _world = nullptr;
        }
        removeAllChildren();
        _arrows.clear();
		_arrowsToFree.clear();
        _active = false;
    }
}

void BallistaScene::update(float deltaTime){
    if(_spawnTimer == 0){
        //testing
        std::shared_ptr<EnemyModel> e = EnemyModel::alloc(Vec2(rand()%(int)(_size.width),_size.height), -M_PI/2, 1, DRAW_SCALE,_assets);
        if(e != nullptr) {
            gameModel._enemyArrayGroundN.insert(e);
            _world->addObstacle(e);
            addChild(e->getNode());
            CULog("enemy added");
        }
        _spawnTimer = 120;
    }
    else{
        _spawnTimer--;
    }

	// Poll inputs
    if(input.isPressed()){
        Vec2 pointdir = _ballista->getPosition() - screenToWorldCoords(input.pointerPos());
        _ballista->setAngle(pointdir.getAngle());
    }
    if(input.justReleased()){
        // Allocate a new arrow in memory
        std::shared_ptr<ArrowModel> a = ArrowModel::alloc(_ballista->getPosition(),_ballista->getAngle(),DRAW_SCALE,_assets);
        
        if(a != nullptr) {
            _arrows.insert(a);
            _world->addObstacle(a);
            addChild(a->getNode());
			CULog("%d\n", _arrows.size());
        }
    }

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
		CULog("%d\n", _arrows.size());
	}
	_arrowsToFree.clear();

	// Update enemies and mark out of bound ones for deletion
	for (auto it = gameModel._enemyArrayGroundN.begin(); it != gameModel._enemyArrayGroundN.end(); it++) {
		std::shared_ptr<EnemyModel> e = *it;
		if (e != nullptr) {
			e->update(deltaTime);
		}
		if (!bounds.contains(e->getPosition())) {
			gameModel._enemiesToFree.insert(e);
		}
	}

	// Delete the enemies here because you can't remove elements while iterating
	for (auto it = gameModel._enemiesToFree.begin(); it != gameModel._enemiesToFree.end(); it++) {
		std::shared_ptr<EnemyModel> e = *it;
		_world->removeObstacle(e.get());
		removeChild(e->getNode());
		gameModel._enemyArrayGroundN.erase(e);
		gameModel.changeWallHealth(5, -9);
		CULog("Num enemies left: %d\n", gameModel._enemyArrayGroundN.size());
	}
	gameModel._enemiesToFree.clear();

    //crank the physics engine
    _world->update(deltaTime);

}

//Pause or Resume
void BallistaScene::setActive(bool active){
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
        _ballistaTOcastle->activate(25);
    }
    else{
        _ballistaTOcastle->deactivate();
    }
}

void BallistaScene::activateWorldCollisions() {
	_world->activateCollisionCallbacks(true);
	_world->onBeginContact = [this](b2Contact* contact) {
		beginContact(contact);
	};
}

void BallistaScene::beginContact(b2Contact* contact) {
	b2Body* body1 = contact->GetFixtureA()->GetBody();
	b2Body* body2 = contact->GetFixtureB()->GetBody();

	for (auto it = _arrows.begin(); it != _arrows.end(); it++) {
		std::shared_ptr<ArrowModel> a = *it;
		if (body1->GetUserData() == a.get() || body2->GetUserData() == a.get()) {
			_arrowsToFree.insert(a);
			break;
		}
	}

	for (auto it = gameModel._enemyArrayGroundN.begin(); it != gameModel._enemyArrayGroundN.end(); it++) {
		std::shared_ptr<EnemyModel> e = *it;
		if (body1->GetUserData() == e.get() || body2->GetUserData() == e.get()) {
			gameModel._enemiesToFree.insert(e);
			break;
		}
	}
}
