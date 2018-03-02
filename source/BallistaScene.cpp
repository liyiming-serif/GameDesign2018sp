//
// Created by Yiming on 2/25/2018.
//

#include "BallistaScene.h"
#define LISTENER_KEY                1

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
    Application::get()->setClearColor(Color4(225,229,170,255));

    _assets = assets;

    // Get the image and attach it to a polygon obj. (no model yet)
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("ballista");
    _ballista = PolygonNode::allocWithTexture(texture);
    _ballista->setScale(0.8f); // Magic number to rescale asset
    _ballista->setAnchor(Vec2::ANCHOR_CENTER);
    _ballista->setPosition(_size.width/2,_size.height/2);
    
    
    
    // Create the OVERWORLD button.  A button has an up image and a down image
    std::shared_ptr<Texture> overworld_up   = _assets->get<Texture>("background");
    std::shared_ptr<Texture> overworld_down = _assets->get<Texture>("background");
    
    Size overworld_b_size = overworld_up->getSize();
    std::shared_ptr<Button> overworld_button = Button::alloc(PolygonNode::allocWithTexture(overworld_up),
                                                            PolygonNode::allocWithTexture(overworld_down));
    overworld_button->setScale(0.1f); // Magic number to rescale asset

        // Create a callback function for the OVERWORLD button
        overworld_button->setName("close");
        overworld_button->setListener([=] (const std::string& name, bool down) {
            // Only quit when the button is released
            if (!down) {
                CULog("Goodbye!");
                Application::get()->quit();
            }
        });
    
    
    // Position the LOOKOUT button in the center
    overworld_button->setAnchor(Vec2::ANCHOR_CENTER);
    overworld_button->setPosition(100,80);
    
    // Add the logo and button to the scene graph

    
    addChild(_ballista);
    addChild(overworld_button);

    // We can only activate a button AFTER it is added to a scene
    
     overworld_button->activate(1);

    CULog("Ballista position: %s\n", _ballista->getPosition().toString().c_str());

    // Input controller
#ifdef CU_TOUCH_SCREEN
    Touchscreen* touch = Input::get<Touchscreen>();

    touch->addMotionListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, const Vec2& prev, bool focus) {
        this->touchDragCB(event,prev,focus);
    });
//    touch->addEndListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, bool focus) {
//        this->touchReleaseCB(event,focus);
//    });
#endif

    return true;
}

void BallistaScene::dispose() {
    if (_active) {
        removeAllChildren();
        _ballista = nullptr;
        _active = false;
    }
}

void BallistaScene::update(float timestep){

}

void BallistaScene::touchDragCB(const TouchEvent& event, const Vec2& previous, bool focus) {
    Vec2 pointdir = _ballista->getPosition() - screenToWorldCoords(event.position);
    _ballista->setAngle(pointdir.getAngle());
}

//void touchReleaseCB(const cugl::TouchEvent& event, bool focus){
//
//};
