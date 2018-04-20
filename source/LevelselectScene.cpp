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


#define DURATION 40.0f
#define DURATION2 1.0f
#define DISTANCE 920
#define REPEATS  3
#define ACT_KEY  "current"




bool LevelselectScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    
    _moveleft = MoveBy::alloc(Vec2(-_size.width*1.06f,0),DURATION2);
    _moveright = MoveBy::alloc(Vec2(_size.width*1.06f,0),DURATION2);
    
    _single = Node::alloc();
    _multi = Node::alloc();
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("levelpage_background");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _background->setPosition(0,_size.height/2);
    addChild(_background);
    
    std::shared_ptr<Texture> texture_foreground  = _assets->get<Texture>("levelpage_foreground");
    _foreground = PolygonNode::allocWithTexture(texture_foreground);
    _foreground->setScale(0.55f); // Magic number to rescale asset
    _foreground->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _foreground->setPosition(0,_size.height/2-8);
    
    std::shared_ptr<Texture> texture_levels  = _assets->get<Texture>("levelpage_levels");
    _levels = PolygonNode::allocWithTexture(texture_levels);
    _levels->setScale(0.5625f); // Magic number to rescale asset
    _levels->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levels->setPosition(0,_size.height/2);


    
    
    std::shared_ptr<Texture> c_1  = _assets->get<Texture>("cloudL");
    _cloud1 = PolygonNode::allocWithTexture(c_1);
    _cloud1->setScale(0.5625f); // Magic number to rescale asset
    _cloud1->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud1->setPosition(197,447);
    _move1 = MoveTo::alloc(Vec2(1300,447),DURATION);
    _cloud1->setColor(Color4 (180,180,230,190));
    
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
    
    addChild(_foreground);
    _single->addChild(_levels);
    
    
    
    
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
    std::shared_ptr<Texture> back_up   = _assets->get<Texture>("back");
    
    
    _backButtonSINGLE = Button::alloc(PolygonNode::allocWithTexture(back_up));
    _backButtonSINGLE->setScale(0.6f); // Magic number to rescale asset
    
    _backButtonMULTI = Button::alloc(PolygonNode::allocWithTexture(back_up));
    _backButtonMULTI->setScale(0.6f); // Magic number to rescale asset
    
    // Create a callback function for the lobby button
    _backButtonSINGLE->setName("lvlTOmenu");
    _backButtonSINGLE->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = MENU;
        }
    });
    
    // Create a callback function for the lobby button
    _backButtonMULTI->setName("lvlTOlobby");
    _backButtonMULTI->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = LOBBY;
        }
    });
    
    // Position the play button in the bottom left
    _playButton->setAnchor(Vec2::ANCHOR_CENTER);
    _playButton->setPosition(115,130);
    
    
    
    // Position the lobby button in the bottom right
    _backButtonSINGLE->setAnchor(Vec2::ANCHOR_CENTER);
    _backButtonSINGLE->setPosition(100,500);
    
    _backButtonMULTI->setAnchor(Vec2::ANCHOR_CENTER);
    _backButtonMULTI->setPosition(400,500);
    
    // Add the logo and button to the scene graph
    _single->addChild(_backButtonSINGLE);
    _multi->addChild(_backButtonMULTI);
    addChild(_single);
    addChild(_multi);
    addChild(_playButton);
    
    // We can only activate a button AFTER it is added to a scene
    _backButtonSINGLE->activate(input.generateKey("backButtonSINGLE"));
    _backButtonMULTI->activate(input.generateKey("backButtonMULTI"));
    _playButton->activate(input.generateKey("playButton"));
    return true;
}

void LevelselectScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _playButton = nullptr;
        _backButtonSINGLE = nullptr;
        _background = nullptr;
        _cloud1 = nullptr;
        _cloud2 = nullptr;
        _cloud3 = nullptr;
        _cloud4 = nullptr;
        _cloud5 = nullptr;
        _cloud6 = nullptr;
        _active = false;
        _single = nullptr;
        _multi = nullptr;
    }
}


void LevelselectScene::setCanvas(int direction){
    _single->setVisible(false);
    _multi->setVisible(false);
    if (direction == 1) {
        _single->setVisible(true);
    }
    else {
        _multi->setVisible(true);
    }
}

void LevelselectScene::setButtonActive ( std::shared_ptr<cugl::Button> button, std::string actKey){
    if (button->getParent()->isVisible()) {
        button->activate(input.findKey(actKey));
    }
    else {
        button->deactivate();
    }
}

void LevelselectScene::update(float timestep){
    
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
        _cloud3->setPosition(-200,512);
        _move3 = MoveTo::alloc(Vec2(1200,512),DURATION/1.5);
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
    
    if (input.vScrolling() < 0 && !_actions->isActive(ACT_KEY+7)) {
        LevelselectScene::doScroll(_moveleft);
    }
    else if (input.vScrolling() > 0 && !_actions->isActive(ACT_KEY+7)) {
        LevelselectScene::doScroll(_moveright);
    }
    
    
    
    
    
    _actions->update(timestep);
}

void LevelselectScene::doMove(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY, action, object, fcn);
}

void LevelselectScene::doMove2(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+2, action, object, fcn);
}

void LevelselectScene::doMove3(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+3, action, object, fcn);
}

void LevelselectScene::doMove4(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+4, action, object, fcn);
}

void LevelselectScene::doMove5(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+5, action, object, fcn);
}

void LevelselectScene::doMove6(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+6, action, object, fcn);
}

void LevelselectScene::doScroll(const std::shared_ptr<MoveBy>& action) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+7, action, _background, fcn);
    _actions->activate(ACT_KEY+8, action, _foreground, fcn);
    _actions->activate(ACT_KEY+9, action, _levels, fcn);
}



//Pause or Resume
void LevelselectScene::setActive(bool active, int players){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(255,255,255,255));
        setCanvas(players);
        setButtonActive(_backButtonSINGLE, "backButtonSINGLE");
        setButtonActive(_backButtonMULTI, "backButtonMULTI");
        _playButton->activate(input.findKey("playButton"));
    }
    else{
        _playButton->deactivate();
        _backButtonSINGLE->deactivate();
        _backButtonMULTI->deactivate();
    }
}
