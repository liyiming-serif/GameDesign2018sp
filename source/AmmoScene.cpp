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
#define WIN         11
#define LOSE        12


/** Define the time settings for animation */
#define DURATION 3.0f
#define DISTANCE 920
#define REPEATS  3
#define ACT_KEY  "current"
#define DMG_DURATION 1.0f
#define DMG_ACT_KEY "marker"

// Decide when to use heavy damage indicator
#define HVY_DMG 6

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
    
    _hammerAnimation = Animate::alloc(0,14,DURATION/3,REPEATS);
    
    _moveup_J = MoveTo::alloc(Vec2(675,650),DURATION/3);
    _moveup_O = MoveTo::alloc(Vec2(675,650),DURATION/3);
    _moveup_B = MoveTo::alloc(Vec2(675,650),DURATION/3);
    
	// Allocate the damage indicators
	std::shared_ptr<Texture> dmg_img = _assets->get<Texture>("dmg_indicator_n");
	std::shared_ptr<PolygonNode> dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_nw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_sw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_s");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_se");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_ne");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_n");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_nw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_sw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_s");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_se");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_ne");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);

	_dmgFadeOUT = FadeOut::alloc(DMG_DURATION);
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("ammo_background");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    addChild(_background);
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    
    std::shared_ptr<Texture> icon  = _assets->get<Texture>("ammo_cursor");
    _cursor = PolygonNode::allocWithTexture(icon);
    _cursor->setScale(.4f); // Magic number to rescale asset
    _cursor->setAnchor(Vec2::ANCHOR_CENTER);
    _cursor->setPosition(_cursor->getWidth()/2,_size.height/11.5);
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
             _actions->clearAllActions(_jackpot);
            _actions->clearAllActions(_hammerAnim);
            if (_jackpot!=nullptr){
                _jackpot->dispose();
            }
            switchscene = OVERWORLD;
        }
    });
    
    
    // Add the hammer
    _hammerAnim = AnimationNode::alloc(_assets->get<Texture>("hammer_animation"), 3, 5);
    _hammerAnim->setAnchor(Vec2::ANCHOR_CENTER);
    _hammerAnim->setScale(.945f);
    _hammerAnim->setPosition(_size.width/2-60,_size.height/2+130);
    _hammerAnim->setFrame(0);
    

    // Create the fire button.  A button has an up image and a down image
    std::shared_ptr<Texture> tex   = _assets->get<Texture>("ammo_button");
    _hammer = Button::alloc(PolygonNode::allocWithTexture(tex));
    _hammer->setScale(7.0f); // Magic number to rescale asset
    
    _hammer2 = Button::alloc(PolygonNode::allocWithTexture(tex));
    _hammer2->setScale(2.5f); // Magic number to rescale asset
    _hammer2->setAngle(M_PI_2);
    
    // Create a callback function for the hammer button
    _hammer->setName("hammer");
    _hammer->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down && !_actions->isActive(ACT_KEY + 1)) {
            float pos = _cursor->getPositionX();

            if (fabs(pos - _size.width / 2) < .05 * _size.width) {
                _ammoClick+=1;
                std::shared_ptr<Texture> perf = _assets->get<Texture>("ammo_perfect");
                _jackpot = PolygonNode::allocWithTexture(perf);
                _jackpot->setScale(.5f); // Magic number to rescale asset
                _jackpot->setAnchor(Vec2::ANCHOR_CENTER);
                _jackpot->setPosition(675, 300);
                addChild(_jackpot);
                AmmoScene::doMoveUp(_moveup_J, _jackpot);
                _jackpot->setColor(Color4(232, 227, 201, 255));
                gameModel.setArrowAmmo(0, gameModel.getArrowAmmo(0) + 15);
                if (gameModel.isNetworked() && !gameModel.isServer()) {
                    gameModel.addDeltaAmmo(0, 15);
                }
                _ammoText->setText("Ammo " + std::to_string(gameModel.getArrowAmmo(0)));
            } else if (fabs(pos - _size.width / 2) < .2 * _size.width) {
                _ammoClick+=1;
                std::shared_ptr<Texture> okay = _assets->get<Texture>("ammo_okay");
                _jackpot = PolygonNode::allocWithTexture(okay);
                _jackpot->setScale(.5f); // Magic number to rescale asset
                _jackpot->setAnchor(Vec2::ANCHOR_CENTER);
                _jackpot->setPosition(675, 300);
                addChild(_jackpot);
                AmmoScene::doMoveUp(_moveup_O, _jackpot);
                _jackpot->setColor(Color4(212, 179, 84, 255));
                gameModel.setArrowAmmo(0, gameModel.getArrowAmmo(0) + 5);
                if (gameModel.isNetworked() && !gameModel.isServer()) {
                    gameModel.addDeltaAmmo(0, 5);
                }
                _ammoText->setText("Ammo " + std::to_string(gameModel.getArrowAmmo(0)));
            } else {
                CULog("bad");
                std::shared_ptr<Texture> bad = _assets->get<Texture>("ammo_bad");
                _jackpot = PolygonNode::allocWithTexture(bad);
                _jackpot->setScale(.5f); // Magic number to rescale asset
                _jackpot->setAnchor(Vec2::ANCHOR_CENTER);
                _jackpot->setPosition(675, 300);
                addChild(_jackpot);
                AmmoScene::doMoveUp(_moveup_B, _jackpot);
                _jackpot->setColor(Color4(159, 48, 46, 255));
                gameModel.setArrowAmmo(0, gameModel.getArrowAmmo(0) + 2);
                if (gameModel.isNetworked() && !gameModel.isServer()) {
                    gameModel.addDeltaAmmo(0, 2);
                }
                _ammoText->setText("Ammo " + std::to_string(gameModel.getArrowAmmo(0)));
            }
            _actions->clearAllActions(_cursor);
        }
    });
    
    _hammer2->setName("hammer2");
    _hammer2->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down && !_actions->isActive(ACT_KEY+1)) {
            float pos = _cursor->getPositionX();

            if (fabs(pos-_size.width/2)< .05*_size.width) {
                _ammoClick+=1;
                std::shared_ptr<Texture> perf  = _assets->get<Texture>("ammo_perfect");
                _jackpot = PolygonNode::allocWithTexture(perf);
                _jackpot->setScale(.5f); // Magic number to rescale asset
                _jackpot->setAnchor(Vec2::ANCHOR_CENTER);
                _jackpot->setPosition(675,300);
                addChild(_jackpot);
                AmmoScene::doMoveUp(_moveup_J, _jackpot);
                _jackpot->setColor(Color4 (232,227,201,255));
                gameModel.setArrowAmmo(0,gameModel.getArrowAmmo(0)+15);
                if (gameModel.isNetworked() && !gameModel.isServer()) {
                    gameModel.addDeltaAmmo(0,15);
                }
                _ammoText->setText("Ammo "+std::to_string(gameModel.getArrowAmmo(0)));
            }
            else if (fabs(pos-_size.width/2)< .2*_size.width ) {
                _ammoClick+=1;
                std::shared_ptr<Texture> okay  = _assets->get<Texture>("ammo_okay");
                _jackpot = PolygonNode::allocWithTexture(okay);
                _jackpot->setScale(.5f); // Magic number to rescale asset
                _jackpot->setAnchor(Vec2::ANCHOR_CENTER);
                _jackpot->setPosition(675,300);
                addChild(_jackpot);
                _jackpot->setColor(Color4 (212,179,84,255));
                AmmoScene::doMoveUp(_moveup_O, _jackpot);
                gameModel.setArrowAmmo(0,gameModel.getArrowAmmo(0)+5);
                if (gameModel.isNetworked() && !gameModel.isServer()) {
                    gameModel.addDeltaAmmo(0,5);
                }
                _ammoText->setText("Ammo "+std::to_string(gameModel.getArrowAmmo(0)));
            }
            else {
                std::shared_ptr<Texture> bad  = _assets->get<Texture>("ammo_bad");
                _jackpot = PolygonNode::allocWithTexture(bad);
                _jackpot->setScale(.5f); // Magic number to rescale asset
                _jackpot->setAnchor(Vec2::ANCHOR_CENTER);
                _jackpot->setPosition(675,300);
                addChild(_jackpot);
                AmmoScene::doMoveUp(_moveup_B, _jackpot);
                _jackpot->setColor(Color4 (159,48,46,255));
                gameModel.setArrowAmmo(0,gameModel.getArrowAmmo(0)+2);
                if (gameModel.isNetworked() && !gameModel.isServer()) {
                    gameModel.addDeltaAmmo(0,2);
                }
                _ammoText->setText("Ammo "+std::to_string(gameModel.getArrowAmmo(0)));
            }
            _actions->clearAllActions(_cursor);
        }
    });
    
    
    // Position the overworld button in the bottom left
    _ammoTOcastle->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _ammoTOcastle->setPosition(15,_size.height-18);;
    
    _hammer->setAnchor(Vec2::ANCHOR_CENTER);
    _hammer->setPosition(_size.width/2+130,434);
    
    _hammer2->setAnchor(Vec2::ANCHOR_CENTER);
    _hammer2->setPosition(20,_size.height/2-10);
    
    // Add the logo and button to the scene graph
    addChild(_ammoTOcastle);
    addChild(_hammerAnim);
    addChild(_hammer);
    addChild(_hammer2);
    
    // We can only activate a button AFTER it is added to a scene
    _ammoTOcastle->activate(input.generateKey("ammoTOcastle"));
    _hammer->activate(input.generateKey("hammer"));
    _hammer2->activate(input.generateKey("hammer2"));
    
    
    _moveleft = MoveTo::alloc(Vec2(_size.width-_cursor->getWidth()/2,_size.height/11.5),DURATION);
    _moveright = MoveTo::alloc(Vec2(_cursor->getWidth()/2,_size.height/11.5),DURATION);
    
    
    std::shared_ptr<Texture> tut_tap  = _assets->get<Texture>("tutorial_tap");
    _tap = PolygonNode::allocWithTexture(tut_tap);
    _tap->setScale(.8); // Magic number to rescale asset
    _tap->setAnchor(Vec2::ANCHOR_CENTER);
    _tap->setPosition(_size.width*.5f,_size.height*.3f);
    _tap->setAngle(M_PI);
    addChild(_tap);

    _ammoText =Label::alloc((std::string) "                                              ", FONT);
    _ammoText->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
    _ammoText->setPosition(_size.width*1.38, _size.height - _size.height/12);
    _ammoText->setForeground(cugl::Color4(232,227,201,255));
    addChild(_ammoText);
    
	// Add damage indicators overlay
	for (int i = 0; i < _dmgIndicators.size(); i++) {
		addChild(_dmgIndicators.at(i));
	}

    //initialize ammo count
	gameModel.setArrowAmmo(0, 30);

    return true;
}

void AmmoScene::dispose() {
    if (_active) {
        removeAllChildren();
        _cursor = nullptr;
        _hammer = nullptr;
        _hammer2 = nullptr;
        _assets = nullptr;
		_actions = nullptr;
        _ammoTOcastle = nullptr;
        _background = nullptr;
        _hammerAnimation= nullptr;
        _hammerAnim = nullptr ;
        _active = false;
		_dmgFadeOUT = nullptr;
		_dmgIndicators.clear();
        _tap=nullptr;
    }
}

void AmmoScene::update(float timestep){

    if (gameModel.getWallHealth(0) == 0 || gameModel.getWallHealth(1) == 0 || gameModel.getWallHealth(2) == 0 ||
        gameModel.getWallHealth(3) == 0 || gameModel.getWallHealth(4) == 0 || gameModel.getWallHealth(5) == 0) {
        switchscene = LOSE;
    }
    if (gameModel._currentTime > gameModel._endTime){
        if (gameModel._enemyArrayMaster[0].size()== 0 && gameModel._enemyArrayMaster[1].size()== 0 && gameModel._enemyArrayMaster[2].size()== 0 && gameModel._enemyArrayMaster[3].size()== 0 && gameModel._enemyArrayMaster[4].size()== 0 && gameModel._enemyArrayMaster[5].size()== 0) {
            switchscene = WIN;
        }
    }
    
    if (_ammoClick > 1) {
        _tap->setVisible(false);
    }

	//poll damage indicators
	pollDmgIndicators();

	//Animate
    if (!_actions->isActive(ACT_KEY+1)){
        if (_jackpot != nullptr) {
            _jackpot->dispose();
        }
        _hammer->activate(input.findKey("hammer"));
        _hammer2->activate(input.findKey("hammer2"));
    }
    if (_actions->isActive(ACT_KEY+1)){
           /// _jackpot->setColor(Color4 (255,255,0,255));
        _hammer->deactivate();
        _hammer2->deactivate();
        AmmoScene::doStrip(_hammerAnimation);
    }
    
    
    if (move_direction  && !_actions->isActive(ACT_KEY) && !_actions->isActive(ACT_KEY+2)){
        CULog("move 1");
        //_cursor->setPosition(Vec2(_cursor->getWidth()/2,40));
        _cursor->setPosition(Vec2(_size.width-_cursor->getWidth()/2,_size.height/11.5));
        doMove(_moveright);
        move_direction = !move_direction;
    }
    if (!move_direction && !_actions->isActive(ACT_KEY) && !_actions->isActive(ACT_KEY+2)){
        CULog("move 2");
        //_cursor->setPosition(Vec2(_size.width-_cursor->getWidth()/2,40));
        _cursor->setPosition(Vec2(_cursor->getWidth()/2,_size.height/11.5));
        doMove(_moveleft);
        move_direction = !move_direction;
        
    }
    _actions->update(timestep);
}

void AmmoScene::doMove(const std::shared_ptr<MoveTo>& action) {
        auto fcn = EasingFunction::alloc(EasingFunction::Type::QUART_IN_OUT);
        _actions->activate(ACT_KEY, action, _cursor, fcn);
}

void AmmoScene::doMoveUp(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> jackpot) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::CUBIC_OUT);
    _actions->activate(ACT_KEY+1, action, jackpot, fcn);
}

void AmmoScene::doStrip(const std::shared_ptr<cugl::Animate>& action) {
    _actions->activate(ACT_KEY+2, action, _hammerAnim);
}

void AmmoScene::pollDmgIndicators() {
	for (int i = 0; i < 6; i++) {
		if (gameModel.getDmgHealth(i) > 0) {
			//turn on damage indicator for that side
			bool succ;
			if (gameModel.getDmgHealth(i) > HVY_DMG) {
				_dmgIndicators.at(i + 6)->setColor(Color4::WHITE);
				succ = _actions->activate(DMG_ACT_KEY + i + 6, _dmgFadeOUT, _dmgIndicators.at(i + 6));
			}
			else {
				_dmgIndicators.at(i)->setColor(Color4::WHITE);
				succ = _actions->activate(DMG_ACT_KEY + i, _dmgFadeOUT, _dmgIndicators.at(i));
			}
			if (succ) {
				gameModel.resetWallDmg();
			}
		}
	}
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
        _hammer2->activate(input.findKey("hammer2"));
        _ammoText->setText("Ammo "+std::to_string(gameModel.getArrowAmmo(0)));
        
        
        if (gameModel.level==3){
            _tapTutorial=true;
        }
        else {
            _tapTutorial=false;
        }
        if (_tapTutorial && _ammoClick < 1) {
            CULog("true");
            _tap->setVisible(true);
        }
        if (!_tapTutorial) {
            CULog("false");
            _tap->setVisible(false);
        }
        
        if (_actions->isActive(ACT_KEY+2)){
            CULog("ACTIVE");
            _actions->pause(ACT_KEY);

        }
        _hammerAnim->setFrame(0);
    }
    else{
        _ammoTOcastle->deactivate();
        _hammer->deactivate();
        _hammer2->deactivate();

		//wipe residual action animations
		for (auto const& it : _dmgIndicators) {
			it->setColor(Color4::CLEAR);
			_actions->clearAllActions(it);
		}
    }
}
