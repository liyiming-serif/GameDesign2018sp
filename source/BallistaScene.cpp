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
#define LEVELS      9
#define LOBBY       10
#define DRAW_SCALE 32
#define FONT    _assets->get<Font>("futura")

#define BUTTON_SCALE 1.0f

#define BALLISTA_MIN_POWER 9.0f

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

	// initialize, but don't add input dragging UI assets
	_dpOrange = _assets->get<Texture>("drag-point-short");
	_dlOrange = _assets->get<Texture>("drag-line-short");
	_dpBlue = _assets->get<Texture>("drag-point");
	_dlBlue = _assets->get<Texture>("drag-line");

	_dragStart = AnimationNode::alloc(_dpOrange, 1, 4);
	_dragStart->setFrame(0);
	_dragStart->setAnchor(Vec2::ANCHOR_CENTER);
	_dragLine = PolygonNode::allocWithTexture(_dlOrange);
	_dragLine->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	_dragEnd = AnimationNode::alloc(_dpOrange, 1, 4);
	_dragEnd->setFrame(0);
	_dragEnd->setAnchor(Vec2::ANCHOR_CENTER);

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
	_enemiesToFree.clear();

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
		_dragStart = nullptr;
		_dragLine = nullptr;
		_dragEnd = nullptr;
		_dpOrange = nullptr;
		_dlOrange = nullptr;
		_dpBlue = nullptr;
		_dlBlue = nullptr;
		_ballista = nullptr;
        _arrows.clear();
		_arrowsToFree.clear();
        _active = false;
        _enemyArray.clear();
		_enemiesToFree.clear();
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

	// Poll inputs
	if (input.justPressed()) {
		addChild(_dragStart);
		addChild(_dragLine);
		addChild(_dragEnd);

		_dragStart->setPosition(screenToWorldCoords(input.dTouch()));
		_dragLine->setPosition(screenToWorldCoords(input.dTouch()));
	}
    if(input.isPressed()){
		if (_ballista->isReadyToFire) {
			Vec2 pointdir = screenToWorldCoords(input.dTouch())-screenToWorldCoords(input.pointerPos());
			_ballista->setAngle(pointdir.getAngle());
			_ballista->setPower(2.0f*pointdir.length() / (float)DRAW_SCALE, false);

			//drag UI
			_dragStart->setFrame((_dragStart->getFrame() + 1)%_dragStart->getSize());
			_dragEnd->setFrame((_dragStart->getFrame() + 1) % _dragStart->getSize());

			_dragEnd->setPosition(screenToWorldCoords(input.pointerPos()));
			_dragLine->setAngle(pointdir.getAngle()+M_PI);
			_dragLine->setScale(pointdir.length()/36.0f,1.0f);
			if(_ballista->getPower() < BALLISTA_MIN_POWER) { //not enough power to fire ballista
				_dragStart->setTexture(_dpOrange);
				_dragLine->setTexture(_dlOrange);
				_dragEnd->setTexture(_dpOrange);
			}
			else {
				_dragStart->setTexture(_dpBlue);
				_dragLine->setTexture(_dlBlue);
				_dragEnd->setTexture(_dpBlue);
			}
		}
    }
    if(input.justReleased()){
		removeChild(_dragStart);
		removeChild(_dragLine);
		removeChild(_dragEnd);
		if (_ballista->isReadyToFire) {
            if (gameModel.getArrowAmmo(0)>0 && _ballista->getPower()>=BALLISTA_MIN_POWER) {
                // Allocate a new arrow in memory
                std::shared_ptr<ArrowModel> a = ArrowModel::alloc(_ballista->getPosition(),
					_ballista->getPower(), _ballista->getAngle(), DRAW_SCALE, _assets);

                if (a != nullptr) {
                    _arrows.insert(a);
                    _world->addObstacle(a);
                    addChild(a->getNode());
                    gameModel.setArrowAmmo(0,gameModel.getArrowAmmo(0)-1);
                }
            }
			// Fire ballista
			_ballista->setPower(0.0f, true);
			_ballista->isReadyToFire = false;
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

	//reflect enemy changes in enemy models
	updateEnemyModels(deltaTime,direction);

    //crank the physics engine
    _world->update(deltaTime);

}

void BallistaScene::updateEnemyModels(float deltaTime, int direction) {
	//add or update enemy models from master array
	for (std::pair<std::string, std::shared_ptr<EnemyDataModel>> epair : gameModel._enemyArrayMaster[direction]) {
		std::string k = epair.first;
		std::shared_ptr<EnemyDataModel> e = epair.second;

		std::unordered_map<std::string, std::shared_ptr<EnemyModel>>::iterator i =
			_enemyArray.find(k);
		if (i == _enemyArray.end()) {
			std::shared_ptr<EnemyModel> en = EnemyModel::alloc(k,e->getPos(),-M_PI/2, e->getType(), DRAW_SCALE, _assets);
			if (en != nullptr) {
				_enemyArray[k] = en;
				_world->addObstacle(en);
				addChild(en->getNode());
			}
		}
		else {
			i->second->setPosition(e->getPos());
			i->second->update(deltaTime);
		}
	}

	//delete enemy models not found in the master 
	for (std::pair<std::string, std::shared_ptr<EnemyModel>> e : _enemyArray) {
		std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>::iterator i =
			gameModel._enemyArrayMaster[direction].find(e.first);
		if (i == gameModel._enemyArrayMaster[direction].end()) {
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
void BallistaScene::setActive(bool active, int direction){
    _direction = direction;
    _active = active;
    switchscene = 0;

	//empty the arrow arrays to prevent data leaks
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
        Application::get()->setClearColor(Color4(132,180,113,255));
        _ballistaTOcastle->activate(input.findKey("ballistaTOcastle"));
        BallistaScene::setCompass(direction);
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

	for (std::pair<std::string, std::shared_ptr<EnemyModel>> epair : _enemyArray) {
		std::shared_ptr<EnemyModel> e = epair.second;
		if (body1->GetUserData() == e.get() || body2->GetUserData() == e.get()) {
			std::string k = epair.first;
			std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>::iterator i =
				gameModel._enemyArrayMaster[_direction].find(k);
			if (i != gameModel._enemyArrayMaster[_direction].end()) {
				std::shared_ptr<EnemyDataModel> ed = i->second;
				if (ed->getHealth() <= 1) {
					gameModel._enemiesToFreeMaster[_direction].push_back(k);
				}
				else {
					ed->setHealth(ed->getHealth()-1);
				}
			}
			break;
		}
	}
}
