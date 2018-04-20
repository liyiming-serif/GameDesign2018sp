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

#define DURATION 40.0f
#define DISTANCE 920
#define REPEATS  3
#define ACT_KEY  "current"

bool LobbyScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }
    
    // Set background color
    Application::get()->setClearColor(Color4(255,255,255,255));
    
    // Allocate the manager and the actions
    _actions = ActionManager::alloc();
    
    players = -1;
    switchscene = 0;
    
    _assets = assets;
    
    _lobby = Node::alloc();
    _avatar = Node::alloc();
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("homepage");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    addChild(_background);
    
    
    
    std::shared_ptr<Texture> c_1  = _assets->get<Texture>("cloudL");
    _cloud1 = PolygonNode::allocWithTexture(c_1);
    _cloud1->setScale(0.5625f); // Magic number to rescale asset
    _cloud1->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud1->setPosition(197,447);
    _move1 = MoveTo::alloc(Vec2(1300,447),DURATION);
    
    std::shared_ptr<Texture> c_2  = _assets->get<Texture>("cloudM");
    _cloud2 = PolygonNode::allocWithTexture(c_2);
    _cloud2->setScale(0.5625f); // Magic number to rescale asset
    _cloud2->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud2->setPosition(1013,496);
    _move2 = MoveTo::alloc(Vec2(1300,496),DURATION/5);
    
    std::shared_ptr<Texture> c_3  = _assets->get<Texture>("cloudS");
    _cloud3 = PolygonNode::allocWithTexture(c_3);
    _cloud3->setScale(0.5625f); // Magic number to rescale asset
    _cloud3->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud3->setPosition(596,500);
    _move3 = MoveTo::alloc(Vec2(1200,500),DURATION/4);
    
    
    _cloud4 = PolygonNode::allocWithTexture(c_3);
    _cloud4->setScale(-0.783f, 0.783f); // Magic number to rescale asset
    _cloud4->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud4->setPosition(740,285);
    _move4 = MoveTo::alloc(Vec2(1200,285),DURATION/4);
    
    _cloud5 = PolygonNode::allocWithTexture(c_3);
    _cloud5->setScale(0.43); // Magic number to rescale asset
    _cloud5->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud5->setPosition(464,52);
    _move5 = MoveTo::alloc(Vec2(1150,52),DURATION/3);
    
    _cloud6 = PolygonNode::allocWithTexture(c_2);
    _cloud6->setScale(0.5625f); // Magic number to rescale asset
    _cloud6->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud6->setPosition(-200,185);
    _move6 = MoveTo::alloc(Vec2(1250,185),DURATION/1.5);
    
    
    addChild(_cloud1);
    addChild(_cloud2);
    addChild(_cloud3);
    addChild(_cloud4);
    addChild(_cloud5);
    addChild(_cloud6);

    
    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> menu_tex   = _assets->get<Texture>("back");
    _backButton = Button::alloc(PolygonNode::allocWithTexture(menu_tex));
    _backButton->setScale(.5f); // Magic number to rescale asset

    // Create a callback function for the OVERWORLD button
    _backButton->setName("menu");
    _backButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = MENU;
        }
    });
    
    
    
    std::shared_ptr<Texture> tex_box   = _assets->get<Texture>("lobby_box");
    _box = PolygonNode::allocWithTexture(tex_box);
    _box->setScale(.6f); // Magic number to rescale asset
    _box->setAnchor(Vec2::ANCHOR_CENTER);
    _box->setPosition(_size.width/2, _size.height/2);
    
    
    // Create the HOST button.  A button has an up image and a down image
    std::shared_ptr<Texture> tex_1   = _assets->get<Texture>("create_button");
    _createButton = Button::alloc(PolygonNode::allocWithTexture(tex_1));
    _createButton->setScale(.6f); // Magic number to rescale asset
    
    
    // Create the JOIN button.  A button has an up image and a down image
    std::shared_ptr<Texture> tex_2   = _assets->get<Texture>("enter_button");
    _enterButton = Button::alloc(PolygonNode::allocWithTexture(tex_2));
    _enterButton->setScale(.6f); // Magic number to rescale asset
    

    _enterButtonOne = Button::alloc(PolygonNode::allocWithTexture(tex_2));
    _enterButtonOne->setScale(.6f); // Magic number to rescale asset
    
    _enterButtonTwo = Button::alloc(PolygonNode::allocWithTexture(tex_2));
    _enterButtonTwo->setScale(.6f); // Magic number to rescale asset
    
    
            // Create a callback function for HOST
            _createButton->setName("create");
            _createButton->setListener([=] (const std::string& name, bool down) {
                if (!down) {
                    #if CU_PLATFORM == CU_PLATFORM_ANDROID
                                setupBluetoothServer();
                    #endif
                   LobbyScene::changeCanvas("avatar");
                    CULog("create");
                }
            });
    
    
    
            // Create a callback function for the JOIN button
            _enterButton->setName("enter");
            _enterButton->setListener([=] (const std::string& name, bool down) {
                if (!down) {
                #if CU_PLATFORM == CU_PLATFORM_ANDROID
                            setupBluetoothClient();
                #endif
                  LobbyScene::changeCanvas("avatar");
                     CULog("enter");
                }
            });
    
    
            // Create a callback function for the JOIN button
            _enterButtonOne->setName("enterOne");
            _enterButtonOne->setListener([=] (const std::string& name, bool down) {
                if (!down) {
        #if CU_PLATFORM == CU_PLATFORM_ANDROID
                    setupBluetoothClient();
        #endif
                    LobbyScene::changeCanvas("avatar");
                    CULog("enterOne");
                }
            });
    
            // Create a callback function for the JOIN button
            _enterButtonTwo->setName("enterTwo");
            _enterButtonTwo->setListener([=] (const std::string& name, bool down) {
                if (!down) {
                #if CU_PLATFORM == CU_PLATFORM_ANDROID
                            setupBluetoothClient();
                #endif
                    LobbyScene::changeCanvas("avatar");
                    CULog("enterTwo");
                }
            });
    
    
    _createButton->setAnchor(Vec2::ANCHOR_CENTER);
    _createButton->setPosition(_size.width/2, 70);
    
    _enterButton->setAnchor(Vec2::ANCHOR_CENTER);
    _enterButton->setPosition(_size.width/2, 470);
    
    _enterButtonOne->setAnchor(Vec2::ANCHOR_CENTER);
    _enterButtonOne->setPosition(_size.width/2, 300);
    
    _enterButtonTwo->setAnchor(Vec2::ANCHOR_CENTER);
    _enterButtonTwo->setPosition(_size.width/2, 200);
    
    
    
    
    _lobby->addChild(_box);
    _lobby->addChild(_createButton);
    _lobby->addChild(_enterButton);
    _lobby->addChild(_enterButtonOne);
    _lobby->addChild(_enterButtonTwo);
    
    

    std::shared_ptr<Texture> tex_p1   = _assets->get<Texture>("player_1");
    _player1 = PolygonNode::allocWithTexture(tex_p1);
    _player1->setScale(.75f); // Magic number to rescale asset
    _player1->setAnchor(Vec2::ANCHOR_CENTER);
    _player1->setPosition(_size.width/4, _size.height/4);

    std::shared_ptr<Texture> tex_p2   = _assets->get<Texture>("player_2");
    _player2 = PolygonNode::allocWithTexture(tex_p2);
    _player2->setScale(1.7f); // Magic number to rescale asset
    _player2->setAnchor(Vec2::ANCHOR_CENTER);
    _player2->setPosition(_size.width - _size.width/4, _size.height/4);

    std::shared_ptr<Texture> tex_w   = _assets->get<Texture>("waiting");
    _waiting = PolygonNode::allocWithTexture(tex_w);
    _waiting->setScale(1.0f); // Magic number to rescale asset
    _waiting->setAnchor(Vec2::ANCHOR_CENTER);
    _waiting->setPosition(_size.width - _size.width/4,_size.height - _size.height/6 - _size.height/7);

    std::shared_ptr<Texture> tex_a1   = _assets->get<Texture>("avatar_1");
    _avatar1 = PolygonNode::allocWithTexture(tex_a1);
    _avatar1->setScale(1.0f); // Magic number to rescale asset
    _avatar1->setAnchor(Vec2::ANCHOR_CENTER);
    _avatar1->setPosition(_size.width/4,_size.height - _size.height/3);

    std::shared_ptr<Texture> tex_a2   = _assets->get<Texture>("avatar_2");
    _avatar2 = PolygonNode::allocWithTexture(tex_a2);
    _avatar2->setScale(1.0f); // Magic number to rescale asset
    _avatar2->setAnchor(Vec2::ANCHOR_CENTER);
    _avatar2->setPosition(_size.width - _size.width/4,_size.height - _size.height/6 - _size.height/8);
    
    
    _avatar->addChild(_player1);
    _avatar->addChild(_player2);
    _avatar->addChild(_avatar1);
    _avatar->addChild(_avatar2);
    

    

    
    
    // Create the play button.  A button has an up image and a down image
    std::shared_ptr<Texture> play_up   = _assets->get<Texture>("levels");
    _levelsButton = Button::alloc(PolygonNode::allocWithTexture(play_up));
    _levelsButton->setScale(0.5f); // Magic number to rescale asset
    
    
    // Create a callback function for the play button
    _levelsButton->setName("levels");
    _levelsButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = LEVELS;
            players = 2;
        }
    });
    
    
  
    // Position the overworld button in the bottom left
    _backButton->setAnchor(Vec2::ANCHOR_CENTER);
    _backButton->setPosition(40,530);
    
    _levelsButton->setAnchor(Vec2::ANCHOR_CENTER);
    _levelsButton->setPosition(_size.width-50, 50);
    
    // Add the logo and button to the scene graph
    addChild(_backButton);
    addChild(_lobby);
    addChild(_avatar);
    addChild(_levelsButton);
    
    // We can only activate a button AFTER it is added to a scene
    _backButton->activate(input.generateKey("backMULTIButton"));
    _createButton->activate(input.generateKey("createButton"));
    _enterButton->activate(input.generateKey("enterButtonn"));
    _enterButtonOne->activate(input.generateKey("enterButtonOne"));
    _enterButtonTwo->activate(input.generateKey("enterButtonTwo"));
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
        _enterButtonTwo = nullptr;
        _background = nullptr;
        _cloud1 = nullptr;
        _cloud2 = nullptr;
        _cloud3 = nullptr;
        _cloud4 = nullptr;
        _cloud5 = nullptr;
        _cloud6 = nullptr;
        _active = false;
    }
}

void LobbyScene::update(float timestep){
    
    // Animate
    if (!_actions->isActive(ACT_KEY) && move1 ){
        doMove(_move1, _cloud1);
        move1=false;
    }
    else if (!_actions->isActive(ACT_KEY) && !move1 ){
        _cloud1->setPosition(-500,447);
        _move1 = MoveTo::alloc(Vec2(1300,447),1.5*DURATION);
        doMove(_move1, _cloud1);
    }


    if (!_actions->isActive(ACT_KEY+2) && move2 ){
        CULog("test");
        doMove2(_move2, _cloud2);
        move2=false;
    }
    else if (!_actions->isActive(ACT_KEY+2) && !move2 ){
        _cloud2->setPosition(-400,496);
        _move2 = MoveTo::alloc(Vec2(1300,496),DURATION);
        doMove2(_move2, _cloud2);
    }


    if (!_actions->isActive(ACT_KEY+3) && move3 ){
        doMove3(_move3, _cloud3);
        move3=false;
    }
    else if (!_actions->isActive(ACT_KEY+3) && !move3 ){
        _cloud3->setPosition(-200,500);
        _move3 = MoveTo::alloc(Vec2(1200,500),DURATION/1.5);
        doMove3(_move3, _cloud3);
    }


    if (!_actions->isActive(ACT_KEY+4) && move4 ){
        doMove4(_move4, _cloud4);
        move4=false;
    }
    else if (!_actions->isActive(ACT_KEY+4) && !move4 ){
        _cloud4->setPosition(-150,285);
        _move4 = MoveTo::alloc(Vec2(1200,285),DURATION/2);
        doMove4(_move4, _cloud4);
    }


    if (!_actions->isActive(ACT_KEY+5) && move5 ){
        doMove5(_move5, _cloud5);
        move5=false;
    }
    else if (!_actions->isActive(ACT_KEY+5) && !move5 ){
        _cloud5->setPosition(-75,52);
        _move5 = MoveTo::alloc(Vec2(1150,52),DURATION/3);
        doMove5(_move5, _cloud5);
    }

    if (!_actions->isActive(ACT_KEY+6) && move6 ){
        doMove6(_move6, _cloud6);
        move6=false;
    }
    else if (!_actions->isActive(ACT_KEY+6) && !move6 ){
        _cloud6->setPosition(-200,185);
        _move6 = MoveTo::alloc(Vec2(1250,185),DURATION/1.5);
        doMove6(_move6, _cloud6);
    }
    
    setButtonActive(_createButton,"createButton");
    setButtonActive(_enterButton,"enterButton");
    setButtonActive(_enterButtonOne,"enterButtonOne");
    setButtonActive(_enterButtonTwo,"enterButtonTwo");

    _actions->update(timestep);
}

void LobbyScene::setButtonActive ( std::shared_ptr<cugl::Button> button, std::string actKey){
    if (button->getParent()->isVisible()) {
        if (!button->isActive()) {
            button->activate(input.findKey(actKey));
        }
    }
    else {
        if (button->isActive()){
            button->deactivate();
        }
    }
}

void LobbyScene::changeCanvas(std::string canvas) {
    if (canvas=="avatar"){
        _lobby->setVisible(false);
        _avatar->setVisible(true);
       // _createButton->deactivate();
       // _enterButton->deactivate();
//        _enterButtonOne->deactivate();
//        _enterButtonTwo->deactivate();
        CULog("go to avatar");
    }
    else {
        return;
    }
        
}

void LobbyScene::doMove(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY, action, object, fcn);
}

void LobbyScene::doMove2(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+2, action, object, fcn);
}

void LobbyScene::doMove3(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+3, action, object, fcn);
}

void LobbyScene::doMove4(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+4, action, object, fcn);
}

void LobbyScene::doMove5(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+5, action, object, fcn);
}

void LobbyScene::doMove6(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+6, action, object, fcn);
}

int LobbyScene::getNumPlayers(){
    //TODO: RETURN NUMBER OF PLAYERS
    return 0;
}


//Pause or Resume
void LobbyScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(255,255,255,255));
        _backButton->activate(input.findKey("backMULTIButton"));
        _levelsButton->activate(input.findKey("levelsMULTIButton"));
        _lobby->setVisible(true);
        _avatar->setVisible(false);
        _createButton->activate(input.findKey("createButton"));
        _enterButton->activate(input.findKey("enterButtonn"));
        _enterButtonOne->activate(input.findKey("enterButtonOne"));
        _enterButtonTwo->activate(input.findKey("enterButtonTwo"));
    }
    else{
        _backButton->deactivate();
        _createButton->deactivate();
        _enterButton->deactivate();
        _enterButtonTwo->deactivate();
        _levelsButton->deactivate();
    }
}
