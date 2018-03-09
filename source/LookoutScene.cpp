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

    // Create the OVERWORLD button.  A button has an up image and a down image
    std::shared_ptr<Texture> overworld_up   = _assets->get<Texture>("ballista_floor");
    std::shared_ptr<Texture> overworld_down = _assets->get<Texture>("ballista_floor");

    Size overworld_b_size = overworld_up->getSize();
    _overworld_button2 = Button::alloc(PolygonNode::allocWithTexture(overworld_up),
                                      PolygonNode::allocWithTexture(overworld_down));
    _overworld_button2->setScale(0.1f); // Magic number to rescale asset

    // Create a callback function for the OVERWORLD button
    _overworld_button2->setName("overworld2");
    _overworld_button2->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("at lookout");
            switchscene = OVERWORLD;
        }
    });


    // Position the overworld button in the bottom left
    _overworld_button2->setAnchor(Vec2::ANCHOR_CENTER);
    _overworld_button2->setPosition(100,80);

    // Add the logo and button to the scene graph
    addChild(_overworld_button2);

    // We can only activate a button AFTER it is added to a scene
    _overworld_button2->activate(26);

    return true;
}

void LookoutScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _overworld_button2 = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void LookoutScene::update(float timestep){

}

//void touchReleaseCB(const cugl::TouchEvent& event, bool focus){
//
//};

//Pause or Resume
void LookoutScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        CULog("over here now");
        _overworld_button2->activate(26);
    }
    else{
        _overworld_button2->deactivate();
    }
}