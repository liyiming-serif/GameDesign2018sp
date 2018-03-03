//
// Created by Yiming on 2/25/2018.
//

#include "BallistaScene.h"
#define LISTENER_KEY                1
#define BALLISTA    1
#define OVERWORLD   2
#define LOOKOUT     3
using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

bool BallistaScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    std::shared_ptr<Texture> bktexture  = _assets->get<Texture>("ballista_view");
    _background = PolygonNode::allocWithTexture(bktexture);
    _background->setScale(.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);

    // Get the image and attach it to a polygon obj. (no model yet)
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("ballista");
    _ballista = PolygonNode::allocWithTexture(texture);
    _ballista->setScale(1.0f); // Magic number to rescale asset
    _ballista->setAnchor(Vec2::ANCHOR_CENTER);
    _ballista->setPosition(766,230);

    
    // Create the OVERWORLD button.  A button has an up image and a down image
    std::shared_ptr<Texture> overworld_up   = _assets->get<Texture>("background");
    std::shared_ptr<Texture> overworld_down = _assets->get<Texture>("background");
    
    Size overworld_b_size = overworld_up->getSize();
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

    // We can only activate a button AFTER it is added to a scene
     _overworld_button->activate(5);

    // Input controller
#ifdef CU_TOUCH_SCREEN
    Touchscreen* touch = Input::get<Touchscreen>();

    touch->addMotionListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, const Vec2& prev, bool focus) {
        this->touchDragCB(event,prev,focus);
    });
    touch->addEndListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, bool focus) {
        this->touchReleaseCB(event,focus);
    });
#endif

    return true;
}

void BallistaScene::dispose() {
    if (_active) {
        removeAllChildren();
        _ballista = nullptr;
        _assets = nullptr;
        _overworld_button = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void BallistaScene::update(float timestep){

}

void BallistaScene::touchDragCB(const TouchEvent& event, const Vec2& previous, bool focus) {
    if(_active) {
        Vec2 pointdir = _ballista->getPosition() - screenToWorldCoords(event.position);
        _ballista->setAngle(pointdir.getAngle());
    }
}

void BallistaScene::touchReleaseCB(const cugl::TouchEvent& event, bool focus){

}

//Pause or Resume
void BallistaScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _overworld_button->activate(5);
    }
    else{
        _overworld_button->deactivate();
    }
}
