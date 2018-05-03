//
//  LoseScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 5/2/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "LoseScene.h"

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
#define WIN         11
#define LOSE        12

bool LoseScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }
    
    // Set background color
    Application::get()->setClearColor(Color4(255,255,255,255));
    
    switchscene = 0;
    
    _assets = assets;
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("lose_BG");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    addChild(_background);
    

    
    // Create the menu button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("lose_menu");
    _loseTOmenu = Button::alloc(PolygonNode::allocWithTexture(castle));
    _loseTOmenu->setScale(.6f); // Magic number to rescale asset
    
    // Create a callback function for the OVERWORLD button
    _loseTOmenu->setName("mageTOcastle");
    _loseTOmenu->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = MENU;
        }
    });
    
    // Create the repaly button.  A button has an up image and a down image
    std::shared_ptr<Texture> replay   = _assets->get<Texture>("lose_replay");
    _loseReplay = Button::alloc(PolygonNode::allocWithTexture(replay));
    _loseReplay->setScale(.6f); // Magic number to rescale asset
    
    // Create a callback function for the OVERWORLD button
    _loseReplay->setName("mageTOcastle");
    _loseReplay->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
            gameModel.level=gameModel.level;
        }
    });
    
    
    // Position the overworld button in the bottom left
    _loseTOmenu->setAnchor(Vec2::ANCHOR_CENTER);
    _loseTOmenu->setPosition(_size.width/2,_size.height/4);
    
    _loseReplay->setAnchor(Vec2::ANCHOR_CENTER);
    _loseReplay->setPosition(_size.width/2,_size.height/7);
    
    // Add the logo and button to the scene graph
    addChild(_loseTOmenu);
    addChild(_loseReplay);
    
    // We can only activate a button AFTER it is added to a scene
    _loseTOmenu->activate(input.generateKey("loseTOmenu"));
    _loseTOmenu->activate(input.generateKey("loseReplay"));
    
    return true;
}

void LoseScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _loseTOmenu = nullptr;
        _loseReplay = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void LoseScene::update(float timestep){
}


//Pause or Resume
void LoseScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _loseTOmenu->activate(input.findKey("loseTOmenu"));
        _loseReplay->activate(input.findKey("loseReplay"));
    }
    else{
        _loseTOmenu->deactivate();
        _loseReplay->deactivate();
    }
}
