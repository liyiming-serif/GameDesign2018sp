//
// Created by Yiming on 2/25/2018.
//

#include "BallistaScene.h"
#define BALLISTA    1
#define OVERWORLD   2
#define LOOKOUT     3
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

    // Set background color
    Application::get()->setClearColor(Color4(132,180,113,255));

    // Set the Input Controller
    _input.init();

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
    _ballista->setScale(1.0f); // Magic number to rescale asset
    _ballista->setAnchor(Vec2::ANCHOR_CENTER);
    _ballista->setPosition(760,225);


    // Create the OVERWORLD button.  A button has an up image and a down image
    std::shared_ptr<Texture> overworld_up   = _assets->get<Texture>("ballista_floor");
    std::shared_ptr<Texture> overworld_down = _assets->get<Texture>("ballista_floor");
    
  
    _overworld_button = Button::alloc(PolygonNode::allocWithTexture(overworld_up),
                                      PolygonNode::allocWithTexture(overworld_down));
    _overworld_button->setScale(0.1f); // Magic number to rescale asset

    // Create a callback function for the OVERWORLD button
    _overworld_button->setName("overworld");
    _overworld_button->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
        }
    });

    // Position the OVERWORLD button in the bottom left
    _overworld_button->setAnchor(Vec2::ANCHOR_CENTER);
    _overworld_button->setPosition(100,80);
    

    // Add children to the scene graph
    addChild(_background);
    addChild(_ballista);
    addChild(_overworld_button);

    // Create the arrows set
    _arrows.clear();
	_arrowsToFree.clear();

    // Create the physics world
    _world = ObstacleWorld::alloc(Rect(Vec2::ZERO, _size/DRAW_SCALE),Vec2::ZERO);

    // We can only activate a button AFTER it is added to a scene
     _overworld_button->activate(25);

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
        _input.dispose();
        _active = false;
    }
}

void BallistaScene::update(float deltaTime){
    // Poll inputs
    if(_input.isPressed()){
        Vec2 pointdir = _ballista->getPosition() - screenToWorldCoords(_input.pointerPos());
        _ballista->setAngle(pointdir.getAngle());
    }
    if(_input.justReleased()){
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
            _world->removeObstacle(a.get());
			removeChild(a->getNode());
        }
    }

	// Delete the arrows here because you can't remove elements while iterating
	for (auto it = _arrowsToFree.begin(); it != _arrowsToFree.end(); it++) {
		std::shared_ptr<ArrowModel> a = *it;
		_arrows.erase(a);
		CULog("%d\n", _arrows.size());
	}
	_arrowsToFree.clear();

    //crank the physics engine
    _world->update(deltaTime);

    //refresh the input controller
    _input.update(deltaTime);
}

//Pause or Resume
void BallistaScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    _arrows.clear();

    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _overworld_button->activate(25);
        //reactivate input listener
        _input.init();
    }
    else{
        _overworld_button->deactivate();
        //deactivate the input listener
        _input.dispose();
    }
}
