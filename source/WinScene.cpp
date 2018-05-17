//
//  WinScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 5/2/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "WinScene.h"

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

bool WinScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }

    replayFlag = false;
    
    // Set background color
    Application::get()->setClearColor(Color4(255,255,255,2555));
    
    switchscene = 0;
    
    _assets = assets;
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("win_BG");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.54f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    addChild(_background);
    

    
    // Create the menu button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("win_menu");
    _winTOmenu = Button::alloc(PolygonNode::allocWithTexture(castle));
    _winTOmenu->setScale(.6f); // Magic number to rescale asset
    
    // Create a callback function
    _winTOmenu->setName("winTOmenu");
    _winTOmenu->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = MENU;
        }
    });
    
    // Create the menu button.  A button has an up image and a down image
    std::shared_ptr<Texture> next   = _assets->get<Texture>("win_next");
    _winNext = Button::alloc(PolygonNode::allocWithTexture(next));
    _winNext->setScale(.6f); // Magic number to rescale asset
    
    // Create a callback function
    _winNext->setName("winNext");
    _winNext->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down && gameModel.level != 10) {
            switchscene = OVERWORLD;
            replayFlag = false;
        }
        else if(!down){
            switchscene = MENU;
        }
    });
    
    // Create the menu button.  A button has an up image and a down image
    std::shared_ptr<Texture> replay   = _assets->get<Texture>("win_replay");
    _winReplay = Button::alloc(PolygonNode::allocWithTexture(replay));
    _winReplay->setScale(.6f); // Magic number to rescale asset
    
    // Create a callback function
    _winReplay->setName("win_replay");
    _winReplay->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
            replayFlag = true;
        }
    });
    
    
    // Position the overworld button in the bottom left
    _winTOmenu->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _winTOmenu->setPosition(15,_size.height/2);
    
    _winReplay->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _winReplay->setPosition(15,_size.height/2.8);
    
    _winNext->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _winNext->setPosition(15,_size.height/5);
    
    // Add the logo and button to the scene graph
    addChild(_winTOmenu);
    addChild(_winNext);
    addChild(_winReplay);
    
    // We can only activate a button AFTER it is added to a scene
    _winTOmenu->activate(input.generateKey("winTOmenu"));
    _winReplay->activate(input.generateKey("winReplay"));
    _winNext->activate(input.generateKey("winNext"));
    
    return true;
}

void WinScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _winTOmenu = nullptr;
        _winReplay = nullptr;
        _winNext = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void WinScene::update(float timestep){
}


//Pause or Resume
void WinScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(255,255,255,255));
        _winTOmenu->activate(input.findKey("winTOmenu"));
        _winReplay->activate(input.findKey("winReplay"));
        _winNext->activate(input.findKey("winNext"));
    }
    else{
        _winTOmenu->deactivate();
        _winReplay->deactivate();
        _winNext->deactivate();
    }
}
