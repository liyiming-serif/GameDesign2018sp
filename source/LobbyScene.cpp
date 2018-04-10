//
//  LobbyScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 3/25/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "LobbyScene.h"
#include "GameModel.h"


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
    std::shared_ptr<Texture> menu_tex   = _assets->get<Texture>("back");
    _backButton = Button::alloc(PolygonNode::allocWithTexture(menu_tex));
    _backButton->setScale(.6f); // Magic number to rescale asset

    // Create a callback function for the OVERWORLD button
    _backButton->setName("menu");
    _backButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = MENU;
        }
    });

    std::shared_ptr<Texture> tex_p1   = _assets->get<Texture>("player_1");
    _player1 = PolygonNode::allocWithTexture(tex_p1);
    _player1->setScale(.75f); // Magic number to rescale asset
    _player1->setAnchor(Vec2::ANCHOR_CENTER);
    _player1->setPosition(_size.width/4, _size.height/4);
    addChild(_player1);
    _player1->setVisible(false);

    std::shared_ptr<Texture> tex_p2   = _assets->get<Texture>("player_2");
    _player2 = PolygonNode::allocWithTexture(tex_p2);
    _player2->setScale(1.7f); // Magic number to rescale asset
    _player2->setAnchor(Vec2::ANCHOR_CENTER);
    _player2->setPosition(_size.width - _size.width/4, _size.height/4);
    addChild(_player2);
    _player2->setVisible(false);

    std::shared_ptr<Texture> tex_w   = _assets->get<Texture>("waiting");
    _waiting = PolygonNode::allocWithTexture(tex_w);
    _waiting->setScale(1.0f); // Magic number to rescale asset
    _waiting->setAnchor(Vec2::ANCHOR_CENTER);
    _waiting->setPosition(_size.width - _size.width/4,_size.height - _size.height/6 - _size.height/7);
    addChild(_waiting);
    _waiting->setVisible(false);

    std::shared_ptr<Texture> tex_a1   = _assets->get<Texture>("avatar_1");
    _avatar1 = PolygonNode::allocWithTexture(tex_a1);
    _avatar1->setScale(1.0f); // Magic number to rescale asset
    _avatar1->setAnchor(Vec2::ANCHOR_CENTER);
    _avatar1->setPosition(_size.width/4,_size.height - _size.height/3);
    addChild(_avatar1);
    _avatar1->setVisible(false);

    std::shared_ptr<Texture> tex_a2   = _assets->get<Texture>("avatar_2");
    _avatar2 = PolygonNode::allocWithTexture(tex_a2);
    _avatar2->setScale(1.0f); // Magic number to rescale asset
    _avatar2->setAnchor(Vec2::ANCHOR_CENTER);
    _avatar2->setPosition(_size.width - _size.width/4,_size.height - _size.height/6 - _size.height/8);
    addChild(_avatar2);
    _avatar2->setVisible(false);
    
    //markers to deactivate the networking buttons
	_deactivateCreate = false;
	_deactivateEnter = false;

    // Create the HOST button.  A button has an up image and a down image
    std::shared_ptr<Texture> tex_1   = _assets->get<Texture>("create_button");
    _createButton = Button::alloc(PolygonNode::allocWithTexture(tex_1));
    _createButton->setScale(.5f); // Magic number to rescale asset
    
    
    // Create the play button.  A button has an up image and a down image
    std::shared_ptr<Texture> play_up   = _assets->get<Texture>("levels");
    
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
    
    
    // Create a callback function for HOST
    _createButton->setName("create");
    _createButton->setListener([=] (const std::string& name, bool down) {
        if (!down) {
#if CU_PLATFORM == CU_PLATFORM_ANDROID
			setupBluetoothServer();
#endif
			_deactivateCreate = true;
			_deactivateEnter = true;
            _createButton->setVisible(false);
            _enterButton->setVisible(false);
            _player1->setVisible(true);
            _player2->setVisible(true);
            _waiting->setVisible(true);
            _avatar1->setVisible(true);
        }
    });
    
    
    // Create the JOIN button.  A button has an up image and a down image
    std::shared_ptr<Texture> tex_2   = _assets->get<Texture>("enter_button");
    _enterButton = Button::alloc(PolygonNode::allocWithTexture(tex_2));
    _enterButton->setScale(.5f); // Magic number to rescale asset
    
    // Create a callback function for the JOIN button
    _enterButton->setName("enter");
    _enterButton->setListener([=] (const std::string& name, bool down) {
        if (!down) {
#if CU_PLATFORM == CU_PLATFORM_ANDROID
            setupBluetoothClient();
#endif
			_deactivateCreate = true;
			_deactivateEnter = true;
            _createButton->setVisible(false);
            _enterButton->setVisible(false);
            _player1->setVisible(true);
            _player2->setVisible(true);
            _avatar1->setVisible(true);
            _avatar2->setVisible(true);
        }
    });
    
    
    // Position the overworld button in the bottom left
    _backButton->setAnchor(Vec2::ANCHOR_CENTER);
    _backButton->setPosition(100,500);
    
    _createButton->setAnchor(Vec2::ANCHOR_CENTER);
    _createButton->setPosition(300,300);
    
    _enterButton->setAnchor(Vec2::ANCHOR_CENTER);
    _enterButton->setPosition(700,300);
    
    _levelsButton->setAnchor(Vec2::ANCHOR_CENTER);
    _levelsButton->setPosition(700,130);
    
    // Add the logo and button to the scene graph
    addChild(_backButton);
    addChild(_createButton);
    addChild(_enterButton);
    addChild(_levelsButton);
    
    // We can only activate a button AFTER it is added to a scene
    _backButton->activate(input.generateKey("backMULTIButton"));
    _createButton->activate(input.generateKey("createButton"));
    _enterButton->activate(input.generateKey("enterButton"));
    _levelsButton->activate(input.generateKey("levelsMULTIButton"));
    
    return true;
}

void LobbyScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _backButton = nullptr;
        _createButton = nullptr;
        _enterButton = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void LobbyScene::update(float timestep){
	if (_deactivateCreate) {
		_createButton->deactivate();
	}
	if (_deactivateEnter) {
		_enterButton->deactivate();
	}
}


//Pause or Resume
void LobbyScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _backButton->activate(input.findKey("backMULTIButton"));
        _levelsButton->activate(input.findKey("levelsMULTIButton"));
		if (!_deactivateCreate) {
			_createButton->activate(input.findKey("createButton"));
		}
		if (!_deactivateEnter) {
			_enterButton->activate(input.findKey("enterButton"));
		}
    }
    else{
        _backButton->deactivate();
        _createButton->deactivate();
        _enterButton->deactivate();
        _levelsButton->deactivate();
    }
}
