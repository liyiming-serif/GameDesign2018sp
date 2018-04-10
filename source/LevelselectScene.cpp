//
//  LevelselectScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 4/10/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelselectScene.h"


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

bool LevelselectScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    
    // Create the play button.  A button has an up image and a down image
    std::shared_ptr<Texture> play_up   = _assets->get<Texture>("play");
    
    
    _playButton = Button::alloc(PolygonNode::allocWithTexture(play_up));
    _playButton->setScale(0.6f); // Magic number to rescale asset
    
    // Create a callback function for the play button
    _playButton->setName("play");
    _playButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
        }
    });
    
    
    // Create the lobby button.  A button has an up image and a down image
    std::shared_ptr<Texture> lobby_up   = _assets->get<Texture>("back");
    
    
    _backButton = Button::alloc(PolygonNode::allocWithTexture(lobby_up));
    _backButton->setScale(0.6f); // Magic number to rescale asset
    
    // Create a callback function for the lobby button
    _backButton->setName("menu");
    _backButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = MENU;
        }
    });
    
    // Position the play button in the bottom left
    _playButton->setAnchor(Vec2::ANCHOR_CENTER);
    _playButton->setPosition(115,130);
    
    
    
    // Position the lobby button in the bottom right
    _backButton->setAnchor(Vec2::ANCHOR_CENTER);
    _backButton->setPosition(100,500);
    
    // Add the logo and button to the scene graph
    addChild(_backButton);
    addChild(_playButton);
    
    // We can only activate a button AFTER it is added to a scene
    _backButton->activate(input.generateKey("backButton"));
    _playButton->activate(input.generateKey("playButton"));
    return true;
}

void LevelselectScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _playButton = nullptr;
        _backButton = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void LevelselectScene::update(float timestep){
    
}



//Pause or Resume
void LevelselectScene::setActive(bool active, int mode){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _playButton->activate(input.findKey("playButton"));
        _backButton->activate(input.findKey("backButton"));
    }
    else{
        _playButton->deactivate();
        _backButton->deactivate();
    }
}
