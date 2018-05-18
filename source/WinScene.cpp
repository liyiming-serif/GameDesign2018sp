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


#define DURATION 40.0f
#define DURATION2 2.0f
#define DISTANCE 920
#define REPEATS  3
#define ACT_KEY  "current"

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
    
    // Allocate the manager and the actions
    _actions = ActionManager::alloc();
    _flagAnimation = Animate::alloc(0,3,DURATION2,REPEATS);
    
    // Set the background image
    std::shared_ptr<Texture> texture_BG  = _assets->get<Texture>("homepage");
    _background = PolygonNode::allocWithTexture(texture_BG);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    
    
    std::shared_ptr<Texture> texture_FG  = _assets->get<Texture>("win_FG");
    _foreground = PolygonNode::allocWithTexture(texture_FG);
    _foreground->setScale(0.54f); // Magic number to rescale asset
    _foreground->setAnchor(Vec2::ANCHOR_CENTER);
    _foreground->setPosition(_size.width/2,_size.height/2);
    
    // Add the flag
    _flag = AnimationNode::alloc(_assets->get<Texture>("m_flag"), 1, 4);
    _flag->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
    _flag->setScale(0.7f);
    _flag->setPosition(_size.width*.82f,_size.height*.4f);
    _flag->setFrame(0);

    
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
    _move4 = MoveTo::alloc(Vec2(1200,285.5),DURATION/4);
    
    _cloud5 = PolygonNode::allocWithTexture(c_3);
    _cloud5->setScale(0.43); // Magic number to rescale asset
    _cloud5->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud5->setPosition(464,52);
    _move5 = MoveTo::alloc(Vec2(1300,52),DURATION/3);
    
    
    


    

    
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
    addChild(_background);
    addChild(_cloud1);
    addChild(_cloud2);
    addChild(_cloud3);
    addChild(_cloud4);
    addChild(_cloud5);
    addChild(_flag);
    addChild(_foreground);
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
        _cloud1 = nullptr;
        _cloud2 = nullptr;
        _cloud3 = nullptr;
        _cloud4 = nullptr;
        _cloud5 = nullptr;
        _flag = nullptr;
        _flagAnimation = nullptr ;
        _foreground=nullptr;
    }
}

void WinScene::update(float timestep){
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
        _move4 = MoveTo::alloc(Vec2(1100,285),DURATION/2);
        doMove4(_move4, _cloud4);
    }


    if (!_actions->isActive(ACT_KEY+5) && move5 ){
        doMove5(_move5, _cloud5);
        move5=false;
    }
    else if (!_actions->isActive(ACT_KEY+5) && !move5 ){
        _cloud5->setPosition(-75,52);
        _move5 = MoveTo::alloc(Vec2(1300,52),DURATION/2.5);
        doMove5(_move5, _cloud5);
    }

    if (!_actions->isActive(ACT_KEY+6)){
        doStrip(_flagAnimation);
    }

    _actions->update(timestep);
}


void WinScene::doMove(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY, action, object, fcn);
}

void WinScene::doMove2(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+2, action, object, fcn);
}

void WinScene::doMove3(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+3, action, object, fcn);
}

void WinScene::doMove4(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+4, action, object, fcn);
}

void WinScene::doMove5(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+5, action, object, fcn);
}

void WinScene::doStrip(const std::shared_ptr<cugl::Animate>& action) {
    _actions->activate(ACT_KEY+6, action, _flag);
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
