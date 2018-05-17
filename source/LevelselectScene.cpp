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

#define BUTTON_SCALE .95f
#define FONT    _assets->get<Font>("futura_levels")




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
    
    _movejungle = MoveTo::alloc(Vec2(-20,_size.height/2),DURATION2);
    _movesnow = MoveTo::alloc(Vec2(-_size.width*1.09,_size.height/2),DURATION2);
    _movedesert = MoveTo::alloc(Vec2(-_size.width*2.12,_size.height/2),DURATION2);
    _single = Node::alloc();
    _multi = Node::alloc();
    
    campaign=1;
    


    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("levelpage_background");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _background->setPosition(-20,_size.height/2);
    addChild(_background);
    
    std::shared_ptr<Texture> texture_foreground  = _assets->get<Texture>("levelpage_foreground");
    _foreground = PolygonNode::allocWithTexture(texture_foreground);
    _foreground->setScale(0.55f); // Magic number to rescale asset
    _foreground->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _foreground->setPosition(-2,_size.height/2-8);
    
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
//    addChild(_cloud5);
//    addChild(_cloud6);
    
    addChild(_foreground);
    //_single->addChild(_levels);
   addChild(_levels);
    
    
    
    
    // Create the play button.  A button has an up image and a down image
    std::shared_ptr<Texture> play   = _assets->get<Texture>("levelpage_button");
    _level1 = Button::alloc(PolygonNode::allocWithTexture(play));
    _level1->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    _lvl1T =Label::alloc((std::string) "1", FONT);
    _lvl1T->setAnchor(Vec2::ANCHOR_CENTER);
    _lvl1T->setPosition(_level1->getContentWidth()/2, _level1->getContentHeight()/2);
    _lvl1T->setForeground(cugl::Color4(159,48,46,255));
    _level1->addChild(_lvl1T);
    

    // Create a callback function for the play button
    _level1->setName("play");
    _level1->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            if (click){
                CULog("ONE!!!");
                switchscene = OVERWORLD;
                gameModel.setLevel(1);
                if (gameModel.isServer() && gameModel.isNetworked()) {
                    int message = sendState(return_buffer(produceACKServer()));
                    (message == 0) ? CULog("Level write success"):CULog("Level write failure");
                }
            }
        }
    });
    
    // Create the play button.  A button has an up image and a down image
    _level2 = Button::alloc(PolygonNode::allocWithTexture(play));
    _level2->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    _lvl2T =Label::alloc((std::string) "2", FONT);
    _lvl2T->setAnchor(Vec2::ANCHOR_CENTER);
    _lvl2T->setPosition(_level2->getContentWidth()/2, _level2->getContentHeight()/2);
    _lvl2T->setForeground(cugl::Color4(159,48,46,255));
    _level2->addChild(_lvl2T);

    // Create a callback function for the play button
    _level2->setName("play");
    _level2->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            if (click){
                switchscene = OVERWORLD;
                gameModel.setLevel(2);
                if (gameModel.isServer() && gameModel.isNetworked()) {
                    int message = sendState(return_buffer(produceACKServer()));
                    (message == 0) ? CULog("Level write success"):CULog("Level write failure");
                }
            }
        }
    });
    
    // Create the play button.  A button has an up image and a down image
    _level3 = Button::alloc(PolygonNode::allocWithTexture(play));
    _level3->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    _lvl3T =Label::alloc((std::string) "3", FONT);
    _lvl3T->setAnchor(Vec2::ANCHOR_CENTER);
    _lvl3T->setPosition(_level3->getContentWidth()/2, _level3->getContentHeight()/2);
    _lvl3T->setForeground(cugl::Color4(159,48,46,255));
    _level3->addChild(_lvl3T);

    // Create a callback function for the play button
    _level3->setName("play");
    _level3->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            if (click){
                switchscene = OVERWORLD;
                gameModel.setLevel(3);
                if (gameModel.isServer() && gameModel.isNetworked()) {
                    int message = sendState(return_buffer(produceACKServer()));
                    (message == 0) ? CULog("Level write success"):CULog("Level write failure");
                }
            }
        }
    });
    
    // Create the play button.  A button has an up image and a down image
    _level4 = Button::alloc(PolygonNode::allocWithTexture(play));
    _level4->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    _lvl4T =Label::alloc((std::string) "4", FONT);
    _lvl4T->setAnchor(Vec2::ANCHOR_CENTER);
    _lvl4T->setPosition(_level4->getContentWidth()/2, _level4->getContentHeight()/2);
    _lvl4T->setForeground(cugl::Color4(159,48,46,255));
    _level4->addChild(_lvl4T);

    // Create a callback function for the play button
    _level4->setName("play");
    _level4->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            if (click){
                switchscene = OVERWORLD;
                gameModel.setLevel(4);
                if (gameModel.isServer() && gameModel.isNetworked()) {
                    int message = sendState(return_buffer(produceACKServer()));
                    (message == 0) ? CULog("Level write success"):CULog("Level write failure");
                }
            }
        }
    });
    
    // Create the play button.  A button has an up image and a down image
    _level5 = Button::alloc(PolygonNode::allocWithTexture(play));
    _level5->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    _lvl5T =Label::alloc((std::string) "5", FONT);
    _lvl5T->setAnchor(Vec2::ANCHOR_CENTER);
    _lvl5T->setPosition(_level5->getContentWidth()/2, _level5->getContentHeight()/2);
    _lvl5T->setForeground(cugl::Color4(159,48,46,255));
    _level5->addChild(_lvl5T);
    
    // Create a callback function for the play button
    _level5->setName("play");
    _level5->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            if (click){
                switchscene = OVERWORLD;
                gameModel.setLevel(5);
                if (gameModel.isServer() && gameModel.isNetworked()) {
                    int message = sendState(return_buffer(produceACKServer()));
                    (message == 0) ? CULog("Level write success"):CULog("Level write failure");
                }
            }
        }
    });

    
    // Create the play button.  A button has an up image and a down image
    _level6 = Button::alloc(PolygonNode::allocWithTexture(play));
    _level6->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    _lvl6T =Label::alloc((std::string) "6", FONT);
    _lvl6T->setAnchor(Vec2::ANCHOR_CENTER);
    _lvl6T->setPosition(_level6->getContentWidth()/2, _level6->getContentHeight()/2);
    _lvl6T->setForeground(cugl::Color4(159,48,46,255));
    _level6->addChild(_lvl6T);
    
    // Create a callback function for the play button
    _level6->setName("play");
    _level6->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            if (click){
                switchscene = OVERWORLD;
                gameModel.setLevel(6);
                if (gameModel.isServer() && gameModel.isNetworked()) {
                    int message = sendState(return_buffer(produceACKServer()));
                    (message == 0) ? CULog("Level write success"):CULog("Level write failure");
                }
            }
        }
    });

    
    // Create the play button.  A button has an up image and a down image
    _level7 = Button::alloc(PolygonNode::allocWithTexture(play));
    _level7->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    _lvl7T =Label::alloc((std::string) "7", FONT);
    _lvl7T->setAnchor(Vec2::ANCHOR_CENTER);
    _lvl7T->setPosition(_level7->getContentWidth()/2, _level7->getContentHeight()/2);
    _lvl7T->setForeground(cugl::Color4(159,48,46,255));
    _level7->addChild(_lvl7T);
    
    // Create a callback function for the play button
    _level7->setName("play");
    _level7->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            if (click){
                switchscene = OVERWORLD;
                gameModel.setLevel(7);
                if (gameModel.isServer() && gameModel.isNetworked()) {
                    int message = sendState(return_buffer(produceACKServer()));
                    (message == 0) ? CULog("Level write success"):CULog("Level write failure");
                }

            }
        }
    });
    
    // Create the play button.  A button has an up image and a down image
    _level8 = Button::alloc(PolygonNode::allocWithTexture(play));
    _level8->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    _lvl8T =Label::alloc((std::string) "8", FONT);
    _lvl8T->setAnchor(Vec2::ANCHOR_CENTER);
    _lvl8T->setPosition(_level8->getContentWidth()/2, _level8->getContentHeight()/2);
    _lvl8T->setForeground(cugl::Color4(159,48,46,255));
    _level8->addChild(_lvl8T);
    
    // Create a callback function for the play button
    _level8->setName("play");
    _level8->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            if (click){
                switchscene = OVERWORLD;
                gameModel.setLevel(8);
                if (gameModel.isServer() && gameModel.isNetworked()) {
                    int message = sendState(return_buffer(produceACKServer()));
                    (message == 0) ? CULog("Level write success"):CULog("Level write failure");
                }
            }
        }
    });
    
    // Create the play button.  A button has an up image and a down image
    _level9 = Button::alloc(PolygonNode::allocWithTexture(play));
    _level9->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    _lvl9T =Label::alloc((std::string) "9", FONT);
    _lvl9T->setAnchor(Vec2::ANCHOR_CENTER);
    _lvl9T->setPosition(_level9->getContentWidth()/2, _level9->getContentHeight()/2);
    _lvl9T->setForeground(cugl::Color4(159,48,46,255));
    _level9->addChild(_lvl9T);
    
    // Create a callback function for the play button
    _level9->setName("play");
    _level9->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            if (click){
                switchscene = OVERWORLD;
                gameModel.setLevel(9);
                if (gameModel.isServer() && gameModel.isNetworked()) {
                    int message = sendState(return_buffer(produceACKServer()));
                    (message == 0) ? CULog("Level write success"):CULog("Level write failure");
                }
            }
        }
    });
    
    // Create the play button.  A button has an up image and a down image
    _level10 = Button::alloc(PolygonNode::allocWithTexture(play));
    _level10->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    _lvl10T =Label::alloc((std::string) "10", FONT);
    _lvl10T->setAnchor(Vec2::ANCHOR_CENTER);
    _lvl10T->setPosition(_level10->getContentWidth()/2, _level10->getContentHeight()/2);
    _lvl10T->setForeground(cugl::Color4(159,48,46,255));
    _level10->addChild(_lvl10T);
    
    // Create a callback function for the play button
    _level10->setName("play");
    _level10->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            if (click){
                switchscene = OVERWORLD;
                gameModel.setLevel(10);
                if (gameModel.isServer() && gameModel.isNetworked()) {
                    int message = sendState(return_buffer(produceACKServer()));
                    (message == 0) ? CULog("Level write success"):CULog("Level write failure");
                }
            }
        }
    });

    
    _levels->addChild(_level1);
    _levels->addChild(_level2);
    _levels->addChild(_level3);
    _levels->addChild(_level4);
    _levels->addChild(_level5);
    _levels->addChild(_level6);
    _levels->addChild(_level7);
    _levels->addChild(_level8);
    _levels->addChild(_level9);
    _levels->addChild(_level10);
    
    // Position the play buttons
    _level1->setAnchor(Vec2::ANCHOR_CENTER);
    _level1->setPosition(385,345);
    
    _level2->setAnchor(Vec2::ANCHOR_CENTER);
    _level2->setPosition(710,630);
    
    _level3->setAnchor(Vec2::ANCHOR_CENTER);
    _level3->setPosition(1180,720);
    
    _level4->setAnchor(Vec2::ANCHOR_CENTER);
    _level4->setPosition(1640,900);
    
    _level5->setAnchor(Vec2::ANCHOR_CENTER);
    _level5->setPosition(2125,720);
    
    _level6->setAnchor(Vec2::ANCHOR_CENTER);
    _level6->setPosition(2500,323);
    
    _level7->setAnchor(Vec2::ANCHOR_CENTER);
    _level7->setPosition(3140,330);
    
    _level8->setAnchor(Vec2::ANCHOR_CENTER);
    _level8->setPosition(4740,330);
    
    _level9->setAnchor(Vec2::ANCHOR_CENTER);
    _level9->setPosition(5060,645);
    
    _level10->setAnchor(Vec2::ANCHOR_CENTER);
    _level10->setPosition(5390,220);
    
    // Create the lobby button.  A button has an up image and a down image
    std::shared_ptr<Texture> back_up   = _assets->get<Texture>("back");
    
    
    _backButtonSINGLE = Button::alloc(PolygonNode::allocWithTexture(back_up));
    _backButtonSINGLE->setScale(0.5f); // Magic number to rescale asset
    
    _backButtonMULTI = Button::alloc(PolygonNode::allocWithTexture(back_up));
    _backButtonMULTI->setScale(0.5f); // Magic number to rescale asset
    
    // Create a callback function for the lobby button
    _backButtonSINGLE->setName("backButtonSINGLE");
    _backButtonSINGLE->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = MENU;
        }
    });
    
    // Create a callback function for the lobby button
    _backButtonMULTI->setName("backButtonMULTI");
    _backButtonMULTI->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = LOBBY;
        }
    });
    

    
    
    
    // Position the lobby button in the bottom right
    _backButtonSINGLE->setAnchor(Vec2::ANCHOR_CENTER);
    _backButtonSINGLE->setPosition(40,530);
    
    _backButtonMULTI->setAnchor(Vec2::ANCHOR_CENTER);
    _backButtonMULTI->setPosition(40,530);
    
    // Add the logo and button to the scene graph
    _single->addChild(_backButtonSINGLE);
    _multi->addChild(_backButtonMULTI);
    addChild(_single);
    addChild(_multi);

    
    // We can only activate a button AFTER it is added to a scene
    _backButtonSINGLE->activate(input.generateKey("backButtonSINGLE"));
    _backButtonMULTI->activate(input.generateKey("backButtonMULTI"));
    return true;
}

void LevelselectScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _level1 = nullptr;
        _level2 = nullptr;
        _level3 = nullptr;
        _level4 = nullptr;
        _level5 = nullptr;
        _level6 = nullptr;
        _level7 = nullptr;
        _level8 = nullptr;
        _level9 = nullptr;
        _level10 = nullptr;
        _backButtonSINGLE = nullptr;
        _background = nullptr;
        _foreground = nullptr;
        _levels = nullptr;
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


void LevelselectScene::setCanvas(int players){
    _single->setVisible(false);
   // _levels->setVisible(false);
    _multi->setVisible(false);
    if (players == 1) {
        _single->setVisible(true);
       // _levels->setVisible(true);
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

void LevelselectScene::disableButtons() {
    _level1->deactivate();
    _level2->deactivate();
    _level3->deactivate();
    _level4->deactivate();
    _level5->deactivate();
    _level6->deactivate();
    _level7->deactivate();
    _level8->deactivate();
    _level9->deactivate();
    _level10->deactivate();
}

void LevelselectScene::enableButtons() {
    setButtonActive(_level1, "lvl1");
    setButtonActive(_level2, "lvl2");
    setButtonActive(_level3, "lvl3");
    setButtonActive(_level4, "lvl4");
    setButtonActive(_level5, "lvl5");
    setButtonActive(_level6, "lvl6");
    setButtonActive(_level7, "lvl7");
    setButtonActive(_level8, "lvl8");
    setButtonActive(_level9, "lvl9");
    setButtonActive(_level10, "lvl10");
}

void LevelselectScene::update(float timestep){
    
    if (!click) {
        _level1->setDown(false);
        _level2->setDown(false);
        _level3->setDown(false);
        _level4->setDown(false);
        _level5->setDown(false);
        _level6->setDown(false);
        _level7->setDown(false);
        _level8->setDown(false);
        _level9->setDown(false);
        _level10->setDown(false);
    }
    
    animateClouds();
    
    if (cooldown>0) {
        cooldown-=1;
    }
    
    if (input.hScrolling() < 0 && campaign < 3 && cooldown==0) {
        if (_actions->isActive(ACT_KEY+7)) {
            _actions->clearAllActions(_background);
            _actions->clearAllActions(_foreground);
            _actions->clearAllActions(_levels);
        }
        if (!_actions->isActive(ACT_KEY+7)) {
            campaign += 1;
            if (campaign==1) {
                LevelselectScene::doScroll(_movejungle);
            }
            if (campaign==2) {
                LevelselectScene::doScroll(_movesnow);
            }
            if (campaign==3) {
                LevelselectScene::doScroll(_movedesert);
            }
            lerp = 0;
            click=false;
            disableButtons();
        }
        cooldown=20;
    }
    else if (input.hScrolling() > 0 && campaign > 1 && cooldown==0) {
        if (_actions->isActive(ACT_KEY+7)) {
            _actions->clearAllActions(_background);
            _actions->clearAllActions(_foreground);
            _actions->clearAllActions(_levels);
        }
        if (!_actions->isActive(ACT_KEY+7)) {
            campaign -= 1;
            if (campaign==1) {
                LevelselectScene::doScroll(_movejungle);
            }
            if (campaign==2) {
                LevelselectScene::doScroll(_movesnow);
            }
            if (campaign==3) {
                LevelselectScene::doScroll(_movedesert);
            }
            lerp = 0;
            click=false;
            disableButtons();
        }
        cooldown=20;
    }
    


    if (!_actions->isActive(ACT_KEY+7)){
        click=true;
        enableButtons();

    }
    
    if (campaign == 1 && lerp < 1) {
        if (lerp == 0) {
            cloudC = _cloud1->getColor();
        }
        cloudC.lerp(Color4 (255,255,255,255), lerp);
       // CULog("color %f %f %f %f" , cloudC.rgba);
        _cloud1->setColor(cloudC);
        _cloud2->setColor(cloudC);
        _cloud3->setColor(cloudC);
        _cloud4->setColor(cloudC);
        lerp +=.002;
      //  CULog("lerp %f",lerp);
    }

    if (campaign == 2 && lerp < 1) {
        if (lerp == 0) {
            cloudC = _cloud1->getColor();
        }
        cloudC.lerp(Color4 (180,180,230,190), lerp);
        _cloud1->setColor(cloudC);
        _cloud2->setColor(cloudC);
        _cloud3->setColor(cloudC);
        _cloud4->setColor(cloudC);
        lerp +=.002;
       // CULog("lerp %f",lerp);
        
    }
    
    if (campaign == 3 && lerp < 1) {
        if (lerp == 0) {
            cloudC = _cloud1->getColor();
        }
        cloudC.lerp(Color4 (240,220,160,130), lerp);
        _cloud1->setColor(cloudC);
        _cloud2->setColor(cloudC);
        _cloud3->setColor(cloudC);
        _cloud4->setColor(cloudC);
        lerp +=.002;
       // CULog("lerp %f",lerp);
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

void LevelselectScene::doScroll(const std::shared_ptr<MoveTo>& action) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+7, action, _background, fcn);
    _actions->activate(ACT_KEY+8, action, _foreground, fcn);
    _actions->activate(ACT_KEY+9, action, _levels, fcn);
}

void LevelselectScene::setLevel(int level){
    gameModel.setLevel(level);
}

int LevelselectScene::getLevel(){
    return gameModel.getLevel();
}

void LevelselectScene::animateClouds() {
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
}

std::string LevelselectScene::produceACKServer() {
    return to_string(gameModel.getNoPlayers())+"|1|" + to_string(level);
}

char* LevelselectScene::return_buffer(const std::string &string) {
    char* return_string = new char[string.length() + 1];
    strcpy(return_string, string.c_str());
    return return_string;
}

//Pause or Resume
void LevelselectScene::setActive(bool active, int players){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(255,255,255,255));
        CULog("players %i", players);
        setCanvas(players);
        setButtonActive(_backButtonMULTI, "backButtonMULTI");
        setButtonActive(_backButtonSINGLE, "backButtonSINGLE");
    }
    else{
        _backButtonSINGLE->deactivate();
        _backButtonMULTI->deactivate();
        disableButtons();
    }
}
