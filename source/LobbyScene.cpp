//
//  LobbyScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 3/25/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "LobbyScene.h"


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
#define LOBBY       10

bool LobbyScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("lobby_background");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(0,0);
    addChild(_background);
    
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    
    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> menu_tex   = _assets->get<Texture>("lobby_button");
    _menuButton = Button::alloc(PolygonNode::allocWithTexture(menu_tex));
    _menuButton->setScale(.2f); // Magic number to rescale asset

    // Create a callback function for the OVERWORLD button
    _menuButton->setName("menu");
    _menuButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("go to menu");
            switchscene = MENU;
        }
    });
    
    
    // Create the join button.  A button has an up image and a down image
    std::shared_ptr<Texture> tex_1   = _assets->get<Texture>("create_button");
    _createButton = Button::alloc(PolygonNode::allocWithTexture(tex_1));
    _createButton->setScale(.5f); // Magic number to rescale asset
    
    // Create a callback function for the button
    _createButton->setName("create");
    _createButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("create");
        }
    });
    
    
    
    // Create the  button.  A button has an up image and a down image
    std::shared_ptr<Texture> tex_2   = _assets->get<Texture>("enter_button");
    _enterButton = Button::alloc(PolygonNode::allocWithTexture(tex_2));
    _enterButton->setScale(.5f); // Magic number to rescale asset
    
    // Create a callback function for the  button
    _enterButton->setName("enter");
    _enterButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("enter");
        }
    });
    
    
    // Position the overworld button in the bottom left
    _menuButton->setAnchor(Vec2::ANCHOR_CENTER);
    _menuButton->setPosition(80,40);
    
    _createButton->setAnchor(Vec2::ANCHOR_CENTER);
    _createButton->setPosition(300,300);
    
    _enterButton->setAnchor(Vec2::ANCHOR_CENTER);
    _enterButton->setPosition(700,300);
    
    // Add the logo and button to the scene graph
    addChild(_menuButton);
    addChild(_createButton);
    addChild(_enterButton);
    
    // We can only activate a button AFTER it is added to a scene
    _menuButton->activate(26);
    _createButton->activate(30);
    _enterButton->activate(31);
    
    return true;
}

void LobbyScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _menuButton = nullptr;
        _createButton = nullptr;
        _enterButton = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void LobbyScene::update(float timestep){
    
}


//Pause or Resume
void LobbyScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        CULog("over here now");
        _menuButton->activate(29);
        _createButton->activate(30);
        _enterButton->activate(31);
    }
    else{
        _menuButton->deactivate();
        _createButton->deactivate();
        _enterButton->deactivate();
    }
}
