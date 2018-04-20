//
//  AmmoScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//


#include <math.h>
#include "AmmoScene.h"


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


/** Define the time settings for animation */
#define DURATION 3.0f
#define DISTANCE 920
#define REPEATS  2
#define ACT_KEY  "current"
#define FONT    _assets->get<Font>("futura")

bool AmmoScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    
    // Allocate the manager and the actions
    _actions = ActionManager::alloc();
    
    _hammerAnimation = Animate::alloc(0,14,DURATION,REPEATS);
    
    _moveup_J = MoveTo::alloc(Vec2(700,650),DURATION/3);
    _moveup_O = MoveTo::alloc(Vec2(700,650),DURATION/3);
    _moveup_B = MoveTo::alloc(Vec2(700,650),DURATION/3);
    

    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("ammo_background");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    addChild(_background);
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    
    std::shared_ptr<Texture> icon  = _assets->get<Texture>("ammo_cursor");
    _cursor = PolygonNode::allocWithTexture(icon);
    _cursor->setScale(.5f); // Magic number to rescale asset
    _cursor->setAnchor(Vec2::ANCHOR_CENTER);
    _cursor->setPosition(_cursor->getWidth()/2,40);
    addChild(_cursor);
    
    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    _ammoTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    _ammoTOcastle->setScale(.6f); // Magic number to rescale asset
    
    // Create a callback function for the back button
    _ammoTOcastle->setName("ammoTOcastle");
    _ammoTOcastle->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
        }
    });
    
    
    // Add the hammer
    _hammerAnim = AnimationNode::alloc(_assets->get<Texture>("hammer_animation"), 3, 5);
    _hammerAnim->setAnchor(Vec2::ANCHOR_CENTER);
    _hammerAnim->setScale(.6f);
    _hammerAnim->setPosition(_size.width/2-70,_size.height/2+120);
    _hammerAnim->setFrame(0);
    

    // Create the fire button.  A button has an up image and a down image
    std::shared_ptr<Texture> tex   = _assets->get<Texture>("ammo_button");
    _hammer = Button::alloc(PolygonNode::allocWithTexture(tex));
    _hammer->setScale(7.0f); // Magic number to rescale asset
    
    // Create a callback function for the back button
    _hammer->setName("hammer");
    _hammer->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down && !_actions->isActive(ACT_KEY+1)) {
//            CULog("NNNNNEEEEEEWWWWWWWW");
//            CULog("Create Arrows!!");
//            CULog("X Position");
//            CULog("%f",_cursor->getPositionX());
            float pos = _cursor->getPositionX();
            
//            CULog("Bounds");
//            CULog("%f",.05*_size.width);
//            CULog("%f",.2*_size.width);
//
//            CULog("Place");
//            CULog("%f",fabs(pos-_size.width/2));
//

            
            if (fabs(pos-_size.width/2)< .05*_size.width) {
                CULog("jackpot");
                std::shared_ptr<Texture> perf  = _assets->get<Texture>("ammo_perfect");
                _jackpot = PolygonNode::allocWithTexture(perf);
                _jackpot->setScale(.5f); // Magic number to rescale asset
                _jackpot->setAnchor(Vec2::ANCHOR_CENTER);
                _jackpot->setPosition(700,300);
                addChild(_jackpot);
                AmmoScene::doMoveUp(_moveup_J, _jackpot);
                CULog("do move");
                gameModel.setArrowAmmo(0,gameModel.getArrowAmmo(0)+50);
            }
            else if (fabs(pos-_size.width/2)< .2*_size.width ) {
                CULog("okay");
                std::shared_ptr<Texture> okay  = _assets->get<Texture>("ammo_okay");
                _jackpot = PolygonNode::allocWithTexture(okay);
                _jackpot->setScale(.5f); // Magic number to rescale asset
                _jackpot->setAnchor(Vec2::ANCHOR_CENTER);
                _jackpot->setPosition(700,300);
                addChild(_jackpot);
                AmmoScene::doMoveUp(_moveup_O, _jackpot);
                gameModel.setArrowAmmo(0,gameModel.getArrowAmmo(0)+30);
            }
            else {
                CULog("bad");
                std::shared_ptr<Texture> bad  = _assets->get<Texture>("ammo_bad");
                _jackpot = PolygonNode::allocWithTexture(bad);
                _jackpot->setScale(.5f); // Magic number to rescale asset
                _jackpot->setAnchor(Vec2::ANCHOR_CENTER);
                _jackpot->setPosition(700,300);
                addChild(_jackpot);
                AmmoScene::doMoveUp(_moveup_B, _jackpot);
                gameModel.setArrowAmmo(0,gameModel.getArrowAmmo(0)+10);
            }
            AmmoScene::doStrip(_hammerAnimation);

            

        }
    });
    
    
    // Position the overworld button in the bottom left
    _ammoTOcastle->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _ammoTOcastle->setPosition(15,_size.height-18);;
    
    _hammer->setAnchor(Vec2::ANCHOR_CENTER);
    _hammer->setPosition(_size.width/2,440);
    
    // Add the logo and button to the scene graph
    addChild(_ammoTOcastle);
    addChild(_hammerAnim);
    addChild(_hammer);
    
    // We can only activate a button AFTER it is added to a scene
    _ammoTOcastle->activate(input.generateKey("ammoTOcastle"));
    _hammer->activate(input.generateKey("hammer"));
    
    
    _moveleft = MoveTo::alloc(Vec2(_size.width-_cursor->getWidth()/2,40),DURATION);
    _moveright = MoveTo::alloc(Vec2(_cursor->getWidth()/2,40),DURATION);

    _ammoText =Label::alloc((std::string) "                                              ", FONT);
    addChild(_ammoText);
    _ammoText->setAnchor(Vec2::ANCHOR_CENTER);
    _ammoText->setPosition(_size.width - _size.width/8, _size.height - _size.height/8);
    _ammoText->setForeground(cugl::Color4(0,0,0,255));
    


    //initialize ammo count
	gameModel.setArrowAmmo(0, 30);

    return true;
}

void AmmoScene::dispose() {
    if (_active) {
        removeAllChildren();
        _cursor = nullptr;
        _hammer = nullptr;
        _assets = nullptr;
        _ammoTOcastle = nullptr;
        _background = nullptr;
        _hammerAnimation= nullptr;
        _hammerAnim = nullptr ;
        _active = false;
    }
}

void AmmoScene::update(float timestep){
    // Animate
    if (move_direction  && !_actions->isActive(ACT_KEY)){
        doMove(_moveright);
        move_direction = !move_direction;
    }
    if (!move_direction && !_actions->isActive(ACT_KEY)){
        doMove(_moveleft);
        move_direction = !move_direction;
    }
    if (!_actions->isActive(ACT_KEY+1)){
        _jackpot=nullptr;
        _hammer->activate(input.findKey("hammer"));
       // _hammer->setDown(false);
    }
    if (_actions->isActive(ACT_KEY+1)){
        _hammer->deactivate();
       // _hammer->setDown(true);
        _hammerAnim->setFrame((_hammerAnim->getFrame()+1) % 15 );
    }
    else {
         _hammerAnim->setFrame(0);
    }
    _actions->update(timestep);
}

void AmmoScene::doMove(const std::shared_ptr<MoveTo>& action) {
        auto fcn = EasingFunction::alloc(EasingFunction::Type::QUINT_IN_OUT);
        _actions->activate(ACT_KEY, action, _cursor, fcn);
}

void AmmoScene::doMoveUp(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> jackpot) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::CUBIC_OUT);
    _actions->activate(ACT_KEY+1, action, jackpot, fcn);
}

void AmmoScene::doStrip(const std::shared_ptr<cugl::Animate>& action) {
    if (_actions->isActive(ACT_KEY+1)) {
        CULog("You must wait for the animation to complete first");
    } else {
        _actions->activate(ACT_KEY+2, action, _hammerAnim);
    }
    CULog("Doing the strip");
}


//Pause or Resume
void AmmoScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _ammoTOcastle->activate(input.findKey("ammoTOcastle"));
        _hammer->activate(input.findKey("hammer"));
    }
    else{
        _ammoTOcastle->deactivate();
        _hammer->deactivate();
    }
}
