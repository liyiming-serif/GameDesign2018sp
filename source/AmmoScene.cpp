//
//  AmmoScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "AmmoScene.h"

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

bool AmmoScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("ammo_background");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(0,0);
    addChild(_background);
    
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    
    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    _ammoTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    _ammoTOcastle->setScale(.8f); // Magic number to rescale asset
    
    // Create a callback function for the OVERWORLD button
    _ammoTOcastle->setName("ammoTOcastle");
    _ammoTOcastle->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("at ammo room");
            switchscene = OVERWORLD;
        }
    });
    
    
    // Position the overworld button in the bottom left
    _ammoTOcastle->setAnchor(Vec2::ANCHOR_CENTER);
    _ammoTOcastle->setPosition(100,80);
    
    // Add the logo and button to the scene graph
    addChild(_ammoTOcastle);
    
    // We can only activate a button AFTER it is added to a scene
    _ammoTOcastle->activate(input.generateKey("ammoTOcastle"));
    
    return true;
}

void AmmoScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _ammoTOcastle = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void AmmoScene::update(float timestep){
    
}


//Pause or Resume
void AmmoScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        CULog("over here now");
        _ammoTOcastle->activate(input.findKey("ammoTOcastle"));
    }
    else{
        _ammoTOcastle->deactivate();
    }
}
