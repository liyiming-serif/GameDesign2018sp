
//
//  MenuScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 3/14/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuScene.h"

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

bool MenuScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("homepage");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(0,0);
    addChild(_background);
    
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    
    // Create the play button.  A button has an up image and a down image
    std::shared_ptr<Texture> play_up   = _assets->get<Texture>("single");
    
    _levelsButton = Button::alloc(PolygonNode::allocWithTexture(play_up));
    _levelsButton->setScale(0.6f); // Magic number to rescale asset
    
    // Create a callback function for the play button
    _levelsButton->setName("play");
    _levelsButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = LEVELS;

        }
    });
    
    
    // Create the lobby button.  A button has an up image and a down image
    std::shared_ptr<Texture> lobby_up   = _assets->get<Texture>("multi");
    
    
    _lobbyButton = Button::alloc(PolygonNode::allocWithTexture(lobby_up));
    _lobbyButton->setScale(0.6f); // Magic number to rescale asset
    
    // Create a callback function for the lobby button
    _lobbyButton->setName("lobby");
    _lobbyButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = LOBBY;
        }
    });
    
    
    // Position the play button in the bottom left
    _levelsButton->setAnchor(Vec2::ANCHOR_CENTER);
    _levelsButton->setPosition(115,130);
    
    
    
    // Position the lobby button in the bottom right
    _lobbyButton->setAnchor(Vec2::ANCHOR_CENTER);
    _lobbyButton->setPosition(350,130);
    
    // Add the logo and button to the scene graph
    addChild(_lobbyButton);
    addChild(_levelsButton);
    
    // We can only activate a button AFTER it is added to a scene
    _lobbyButton->activate(input.generateKey("lobbyButton"));
    _levelsButton->activate(input.generateKey("levelsButton"));
    return true;
}

void MenuScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _levelsButton = nullptr;
        _lobbyButton = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void MenuScene::update(float timestep){
    
}



//Pause or Resume
void MenuScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _levelsButton->activate(input.findKey("levelsButton"));
        _lobbyButton->activate(input.findKey("lobbyButton"));
    }
    else{
        _levelsButton->deactivate();
        _lobbyButton->deactivate();
    }
}
