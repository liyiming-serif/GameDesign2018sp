
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


#define DURATION 40.0f
#define DISTANCE 920
#define REPEATS  3
#define ACT_KEY  "current"

bool MenuScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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

    switchscene = 0;
    
    _assets = assets;
    

    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("homepage");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);

    

    // Set the background image
    std::shared_ptr<Texture> texture_title  = _assets->get<Texture>("m_title");
    _title = PolygonNode::allocWithTexture(texture_title);
    _title->setScale(0.5625f); // Magic number to rescale asset
    _title->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _title->setPosition(5,_size.height-3);

    
    
    // Set the background image
    std::shared_ptr<Texture> texture_castle  = _assets->get<Texture>("m_castle");
    _castle = PolygonNode::allocWithTexture(texture_castle);
    _castle->setScale(0.5625f); // Magic number to rescale asset
    _castle->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _castle->setPosition(_size.width/2+2,-16);
    
    // Set the background image
    std::shared_ptr<Texture> texture_flag  = _assets->get<Texture>("m_flag");
    _flag = PolygonNode::allocWithTexture(texture_flag);
    _flag->setScale(-0.5625f,0.5625f); // Magic number to rescale asset
    _flag->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
    _flag->setPosition(_size.width-60.5,_castle->getHeight()-57);

    
    
    std::shared_ptr<Texture> c_1  = _assets->get<Texture>("cloudL");
    _cloud1 = PolygonNode::allocWithTexture(c_1);
    _cloud1->setScale(0.5625f); // Magic number to rescale asset
    _cloud1->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud1->setPosition(197,447);
    _move1 = MoveTo::alloc(Vec2(1300,447.2),DURATION);
    
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
    _cloud3->setPosition(596,512);
    _move3 = MoveTo::alloc(Vec2(1200,512),DURATION/4);
    
    
    _cloud4 = PolygonNode::allocWithTexture(c_3);
    _cloud4->setScale(-0.783f, 0.783f); // Magic number to rescale asset
    _cloud4->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud4->setPosition(740,285);
    _move4 = MoveTo::alloc(Vec2(950,285.5),DURATION/4);
    
    _cloud5 = PolygonNode::allocWithTexture(c_3);
    _cloud5->setScale(0.43); // Magic number to rescale asset
    _cloud5->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud5->setPosition(464,52);
    _move5 = MoveTo::alloc(Vec2(700,52),DURATION/3);

    
    
    
    addChild(_background);
    addChild(_cloud1);
    addChild(_cloud2);
    addChild(_cloud3);
    addChild(_cloud4);
    addChild(_cloud5);
    addChild(_title);
    addChild(_flag);
    addChild(_castle);



    
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
    _levelsButton->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levelsButton->setPosition(15,200);
    
    
    
    // Position the lobby button in the bottom right
    _lobbyButton->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _lobbyButton->setPosition(15,100);
    
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
        _cloud1 = nullptr;
        _cloud2 = nullptr;
        _cloud3 = nullptr;
        _cloud4 = nullptr;
        _cloud5 = nullptr;
        _title = nullptr;
        _flag = nullptr;
        _castle = nullptr;
        _active = false;
    }
}

void MenuScene::update(float timestep){
    
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
        _cloud3->setPosition(-200,512);
        _move3 = MoveTo::alloc(Vec2(1200,512),DURATION/2);
        doMove3(_move3, _cloud3);
    }


    if (!_actions->isActive(ACT_KEY+4) && move4 ){
        doMove4(_move4, _cloud4);
        move4=false;
    }
    else if (!_actions->isActive(ACT_KEY+4) && !move4 ){
        _cloud4->setPosition(-150,285);
        _move4 = MoveTo::alloc(Vec2(900,285),DURATION/2.5);
        doMove4(_move4, _cloud4);
    }


    if (!_actions->isActive(ACT_KEY+5) && move5 ){
        doMove5(_move5, _cloud5);
        move5=false;
    }
    else if (!_actions->isActive(ACT_KEY+5) && !move5 ){
        _cloud5->setPosition(-75,52);
        _move5 = MoveTo::alloc(Vec2(700,52),DURATION/4);
        doMove5(_move5, _cloud5);
    }

    _actions->update(timestep);
}


void MenuScene::doMove(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY, action, object, fcn);
}

void MenuScene::doMove2(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+2, action, object, fcn);
}

void MenuScene::doMove3(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+3, action, object, fcn);
}

void MenuScene::doMove4(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+4, action, object, fcn);
}

void MenuScene::doMove5(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+5, action, object, fcn);
}



//Pause or Resume
void MenuScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(255,255,255,255));
        _levelsButton->activate(input.findKey("levelsButton"));
        _lobbyButton->activate(input.findKey("lobbyButton"));
    }
    else{
        _levelsButton->deactivate();
        _lobbyButton->deactivate();
    }
}
