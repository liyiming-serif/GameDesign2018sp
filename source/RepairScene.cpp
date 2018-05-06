//
//  RepairScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 3/9/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "RepairScene.h"
#include <string>

using namespace cugl;
using namespace std;

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

#define FLOOR_SCALE .54f
#define BUTTON_SCALE 1.255f
#define BUTTON_SCALE2 1.0f

/** Define the time settings for animation */
#define DURATION .4f
#define DISTANCE 200
#define REPEATS  3
#define ACT_KEY  "current"

#define UP  _assets->get<Texture>("ammo_icon")

#define FONT    _assets->get<Font>("futura")

bool RepairScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }
    
    // Set background color
    //Application::get()->setClearColor(Color4(132,180,113,255));
    Application::get()->setClearColor(Color4(0,0,0,255));
    
    switchscene = 0;
    
    _assets = assets;
    
    _curr_wall = "";
    _new_wall = "";
    
    // Allocate the manager and the actions
    _actions = ActionManager::alloc();
    
    _wallFadeIN = FadeIn::alloc(DURATION);
    _wallFadeOUT = FadeOut::alloc(DURATION);
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("repair_background");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(FLOOR_SCALE); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    addChild(_background);

    // Buttons
        std::shared_ptr<Texture> image_up   = _assets->get<Texture>("repair_transparent");
        _northWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
        _northeastWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
        _southeastWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
        _southWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
        _southwestWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
        _northwestWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
    
    // Create a callback function for the Ballista buttons
        _northWallButton->setName("fade in N");
        _northWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                _new_wall = "N";
                if (gameModel.isNetworked()) {
                    if (gameModel.isServer()) {
						CULog("networked SERVER in repair scene");
                        gameModel.changeWallHealth(0, 1);
                        _northText->setText(std::to_string(gameModel.getWallHealth(0))+"%");
                    }
                    else {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(0,1);
                        gameModel.addDeltaHealth(0,1);
                        _northText->setText(std::to_string(gameModel.getWallHealth(0))+"%");
                    }

                }
                else {
                    gameModel.changeWallHealth(0, 1);
                    _northText->setText(std::to_string(gameModel.getWallHealth(0))+"%");
                }
            }
        });
    
        _northeastWallButton->setName("fade in NE");
        _northeastWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                _new_wall = "NE";
                if (gameModel.isNetworked()) {
                    if (gameModel.isServer()) {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(5, 1);
                        _northeastText->setText(std::to_string(gameModel.getWallHealth(5))+"%");
                    }
                    else {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(5,1);
                        gameModel.addDeltaHealth(5,1);
                        _northeastText->setText(std::to_string(gameModel.getWallHealth(5))+"%");
                    }

                }
                else {
                    gameModel.changeWallHealth(5, 1);
                    _northeastText->setText(std::to_string(gameModel.getWallHealth(5))+"%");
                }
            }
        });
    
    
        _southeastWallButton->setName("fade in SE");
        _southeastWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                _new_wall = "SE";
                if (gameModel.isNetworked()) {
                    if (gameModel.isServer()) {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(4, 1);
                        _southeastText->setText(std::to_string(gameModel.getWallHealth(4))+"%");
                    }
                    else {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(4,1);
                        gameModel.addDeltaHealth(4,1);
                        _southeastText->setText(std::to_string(gameModel.getWallHealth(4))+"%");
                    }

                }
                else {
                    gameModel.changeWallHealth(4, 1);
                    _southeastText->setText(std::to_string(gameModel.getWallHealth(4))+"%");
                }
            }
        });

        _southWallButton->setName("fade in S");
        _southWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                _new_wall = "S";
                if (gameModel.isNetworked()) {
                    if (gameModel.isServer()) {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(3, 1);
                        _southText->setText(std::to_string(gameModel.getWallHealth(3))+"%");
                    }
                    else {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(3,1);
                        gameModel.addDeltaHealth(3,1);
                        _southText->setText(std::to_string(gameModel.getWallHealth(3))+"%");
                    }

                }
                else {
                    gameModel.changeWallHealth(3, 1);
                    _southText->setText(std::to_string(gameModel.getWallHealth(3))+"%");
                }
            }
        });
        _southwestWallButton->setName("fade in SW");
        _southwestWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
				_new_wall = "SW";
                if (gameModel.isNetworked()) {
                    if (gameModel.isServer()) {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(2, 1);
                        _southwestText->setText(std::to_string(gameModel.getWallHealth(2))+"%");
                    }
                    else {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(2,1);
                        gameModel.addDeltaHealth(2,1);
                        _southwestText->setText(std::to_string(gameModel.getWallHealth(2))+"%");
                    }

                }
                else {
                    gameModel.changeWallHealth(2, 1);
                    _southwestText->setText(std::to_string(gameModel.getWallHealth(2))+"%");
                }
            }
        });
        _northwestWallButton->setName("fade in NW");
        _northwestWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
				_new_wall = "NW";
                if (gameModel.isNetworked()) {
                    if (gameModel.isServer()) {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(1, 1);
                        _northwestText->setText(std::to_string(gameModel.getWallHealth(1))+"%");
                    }
                    else {
						CULog("networked CLIENT in repair scene");
                        gameModel.changeWallHealth(1,1);
                        gameModel.addDeltaHealth(1,1);
                        _northwestText->setText(std::to_string(gameModel.getWallHealth(1))+"%");
                    }

                }
                else {
                    gameModel.changeWallHealth(1, 1);
                    _northwestText->setText(std::to_string(gameModel.getWallHealth(1))+"%");
                }
            }
        });
    
    
        std::shared_ptr<Texture> repair_100  = _assets->get<Texture>("repair_100");
        std::shared_ptr<Texture> repair_75  = _assets->get<Texture>("repair_75");
        std::shared_ptr<Texture> repair_50  = _assets->get<Texture>("repair_50");
        std::shared_ptr<Texture> repair_25  = _assets->get<Texture>("repair_25");

        float centerX = _background->getContentSize().width/2;
        float centerY = _background->getContentSize().height/2;
        _northWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northWallButton->setPosition(centerX,centerY+.22*_background->getContentHeight());

                N_100 = PolygonNode::allocWithTexture(repair_100);
                N_100->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                N_100->setAnchor(Vec2::ANCHOR_CENTER);
                N_100->setPosition(_northWallButton->getContentWidth()/2,_northWallButton->getContentHeight()/2);
                _northWallButton->addChild(N_100);
                N_100->setVisible(false);
    
                N_75 = PolygonNode::allocWithTexture(repair_75);
                N_75->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                N_75->setAnchor(Vec2::ANCHOR_CENTER);
                N_75->setPosition(_northWallButton->getContentWidth()/2,_northWallButton->getContentHeight()/2);
                _northWallButton->addChild(N_75);
                N_75->setVisible(false);
    
                N_50 = PolygonNode::allocWithTexture(repair_50);
                N_50->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                N_50->setAnchor(Vec2::ANCHOR_CENTER);
                N_50->setPosition(_northWallButton->getContentWidth()/2,_northWallButton->getContentHeight()/2);
                _northWallButton->addChild(N_50);
                N_50->setVisible(false);
    
                N_25 = PolygonNode::allocWithTexture(repair_25);
                N_25->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                N_25->setAnchor(Vec2::ANCHOR_CENTER);
                N_25->setPosition(_northWallButton->getContentWidth()/2,_northWallButton->getContentHeight()/2);
                _northWallButton->addChild(N_25);
                N_25->setVisible(false);
    
            _northText =Label::alloc((std::string) "                         ", FONT);
            _northWallButton->addChild(_northText);
            _northText->setAnchor(Vec2::ANCHOR_CENTER);
            _northText->setPosition(_northWallButton->getContentWidth()/2,_northWallButton->getContentHeight()/2);
            _northText->setForeground(cugl::Color4(233,225,212,255));
    
        _northeastWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northeastWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northeastWallButton->setPosition(centerX+.11*_background->getContentWidth(),centerY+.112*_background->getContentHeight());
        _northeastWallButton->setAngle(-M_PI/3);
    
                NE_100 = PolygonNode::allocWithTexture(repair_100);
                NE_100->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                NE_100->setAnchor(Vec2::ANCHOR_CENTER);
                NE_100->setPosition(_northeastWallButton->getContentWidth()/2,_northeastWallButton->getContentHeight()/2);
                _northeastWallButton->addChild(NE_100);
                NE_100->setVisible(false);
    
                NE_75 = PolygonNode::allocWithTexture(repair_75);
                NE_75->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                NE_75->setAnchor(Vec2::ANCHOR_CENTER);
                NE_75->setPosition(_northeastWallButton->getContentWidth()/2,_northeastWallButton->getContentHeight()/2);
                _northeastWallButton->addChild(NE_75);
                NE_75->setVisible(false);
    
                NE_50 = PolygonNode::allocWithTexture(repair_50);
                NE_50->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                NE_50->setAnchor(Vec2::ANCHOR_CENTER);
                NE_50->setPosition(_northeastWallButton->getContentWidth()/2,_northeastWallButton->getContentHeight()/2);
                _northeastWallButton->addChild(NE_50);
                NE_50->setVisible(false);
    
                NE_25 = PolygonNode::allocWithTexture(repair_25);
                NE_25->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                NE_25->setAnchor(Vec2::ANCHOR_CENTER);
                NE_25->setPosition(_northeastWallButton->getContentWidth()/2,_northeastWallButton->getContentHeight()/2);
                _northeastWallButton->addChild(NE_25);
                NE_25->setVisible(false);
    
            _northeastText =Label::alloc( (std::string) "                         ", FONT);
            _northeastWallButton->addChild(_northeastText);
            _northeastText->setAnchor(Vec2::ANCHOR_CENTER);
            _northeastText->setPosition(_northeastWallButton->getContentWidth()/2,_northeastWallButton->getContentHeight()/2);
            _northeastText->setForeground(cugl::Color4(233,225,212,255));
    
        _southeastWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _southeastWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _southeastWallButton->setPosition(centerX+.11*_background->getContentWidth(),centerY-.111*_background->getContentHeight());
        _southeastWallButton->setAngle(-2*M_PI/3);
    
                SE_100 = PolygonNode::allocWithTexture(repair_100);
                SE_100->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                SE_100->setAnchor(Vec2::ANCHOR_CENTER);
                SE_100->setPosition(_southeastWallButton->getContentWidth()/2,_southeastWallButton->getContentHeight()/2);
                _southeastWallButton->addChild(SE_100);
                SE_100->setVisible(false);
    
                SE_75 = PolygonNode::allocWithTexture(repair_75);
                SE_75->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                SE_75->setAnchor(Vec2::ANCHOR_CENTER);
                SE_75->setPosition(_southeastWallButton->getContentWidth()/2,_southeastWallButton->getContentHeight()/2);
                _southeastWallButton->addChild(SE_75);
                SE_75->setVisible(false);
    
                SE_50 = PolygonNode::allocWithTexture(repair_50);
                SE_50->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                SE_50->setAnchor(Vec2::ANCHOR_CENTER);
                SE_50->setPosition(_southeastWallButton->getContentWidth()/2,_southeastWallButton->getContentHeight()/2);
                _southeastWallButton->addChild(SE_50);
                SE_50->setVisible(false);
    
                SE_25 = PolygonNode::allocWithTexture(repair_25);
                SE_25->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                SE_25->setAnchor(Vec2::ANCHOR_CENTER);
                SE_25->setPosition(_southeastWallButton->getContentWidth()/2,_southeastWallButton->getContentHeight()/2);
                _southeastWallButton->addChild(SE_25);
                SE_25->setVisible(false);
    
            _southeastText =Label::alloc((std::string) "                         ", FONT);
            _southeastWallButton->addChild(_southeastText);
            _southeastText->setAnchor(Vec2::ANCHOR_CENTER);
            _southeastText->setPosition(_southeastWallButton->getContentWidth()/2,_southeastWallButton->getContentHeight()/2);
            _southeastText->setAngle(M_PI);
            _southeastText->setForeground(cugl::Color4(233,225,212,255));
    
        _southWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _southWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _southWallButton->setPosition(centerX,centerY-.22*_background->getContentHeight());
        _southWallButton->setAngle(-3*M_PI/3);
    
                S_100 = PolygonNode::allocWithTexture(repair_100);
                S_100->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                S_100->setAnchor(Vec2::ANCHOR_CENTER);
                S_100->setPosition(_southWallButton->getContentWidth()/2,_southWallButton->getContentHeight()/2);
                _southWallButton->addChild(S_100);
                S_100->setVisible(false);
    
                S_75 = PolygonNode::allocWithTexture(repair_75);
                S_75->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                S_75->setAnchor(Vec2::ANCHOR_CENTER);
                S_75->setPosition(_southWallButton->getContentWidth()/2,_southWallButton->getContentHeight()/2);
                _southWallButton->addChild(S_75);
                S_75->setVisible(false);
    
                S_50 = PolygonNode::allocWithTexture(repair_50);
                S_50->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                S_50->setAnchor(Vec2::ANCHOR_CENTER);
                S_50->setPosition(_southWallButton->getContentWidth()/2,_southWallButton->getContentHeight()/2);
                _southWallButton->addChild(S_50);
                S_50->setVisible(false);
    
                S_25 = PolygonNode::allocWithTexture(repair_25);
                S_25->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                S_25->setAnchor(Vec2::ANCHOR_CENTER);
                S_25->setPosition(_southWallButton->getContentWidth()/2,_southWallButton->getContentHeight()/2);
                _southWallButton->addChild(S_25);
                S_25->setVisible(false);
    
            _southText =Label::alloc((std::string) "                         ", FONT);
            _southWallButton->addChild(_southText);
            _southText->setAnchor(Vec2::ANCHOR_CENTER);
            _southText->setPosition(_southWallButton->getContentWidth()/2,_southWallButton->getContentHeight()/2);
            _southText->setAngle(M_PI);
            _southText->setForeground(cugl::Color4(233,225,212,255));
    
        _southwestWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _southwestWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _southwestWallButton->setPosition(centerX-.11*_background->getContentWidth(),centerY-.113*_background->getContentHeight());
        _southwestWallButton->setAngle(-4*M_PI/3);
    
                SW_100 = PolygonNode::allocWithTexture(repair_100);
                SW_100->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                SW_100->setAnchor(Vec2::ANCHOR_CENTER);
                SW_100->setPosition(_southwestWallButton->getContentWidth()/2,_southwestWallButton->getContentHeight()/2);
                _southwestWallButton->addChild(SW_100);
                SW_100->setVisible(false);
    
                SW_75 = PolygonNode::allocWithTexture(repair_75);
                SW_75->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                SW_75->setAnchor(Vec2::ANCHOR_CENTER);
                SW_75->setPosition(_southwestWallButton->getContentWidth()/2,_southwestWallButton->getContentHeight()/2);
                _southwestWallButton->addChild(SW_75);
                SW_75->setVisible(false);
    
                SW_50 = PolygonNode::allocWithTexture(repair_50);
                SW_50->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                SW_50->setAnchor(Vec2::ANCHOR_CENTER);
                SW_50->setPosition(_southwestWallButton->getContentWidth()/2,_southwestWallButton->getContentHeight()/2);
                _southwestWallButton->addChild(SW_50);
                SW_50->setVisible(false);
    
                SW_25 = PolygonNode::allocWithTexture(repair_25);
                SW_25->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                SW_25->setAnchor(Vec2::ANCHOR_CENTER);
                SW_25->setPosition(_southwestWallButton->getContentWidth()/2,_southwestWallButton->getContentHeight()/2);
                _southwestWallButton->addChild(SW_25);
                SW_25->setVisible(false);
    
            _southwestText =Label::alloc((std::string) "                         ", FONT);
            _southwestWallButton->addChild(_southwestText);
            _southwestText->setAnchor(Vec2::ANCHOR_CENTER);
            _southwestText->setPosition(_southwestWallButton->getContentWidth()/2,_southwestWallButton->getContentHeight()/2);
            _southwestText->setAngle(M_PI);
            _southwestText->setForeground(cugl::Color4(233,225,212,255));
    
        _northwestWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northwestWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northwestWallButton->setPosition(centerX-.11*_background->getContentWidth(),centerY+.11*_background->getContentHeight());
        _northwestWallButton->setAngle(M_PI/3);
    
                NW_100 = PolygonNode::allocWithTexture(repair_100);
                NW_100->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                NW_100->setAnchor(Vec2::ANCHOR_CENTER);
                NW_100->setPosition(_northwestWallButton->getContentWidth()/2,_northwestWallButton->getContentHeight()/2);
                _northwestWallButton->addChild(NW_100);
                NW_100->setVisible(false);
    
                NW_75 = PolygonNode::allocWithTexture(repair_75);
                NW_75->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                NW_75->setAnchor(Vec2::ANCHOR_CENTER);
                NW_75->setPosition(_northwestWallButton->getContentWidth()/2,_northwestWallButton->getContentHeight()/2);
                _northwestWallButton->addChild(NW_75);
                NW_75->setVisible(false);
    
                NW_50 = PolygonNode::allocWithTexture(repair_50);
                NW_50->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                NW_50->setAnchor(Vec2::ANCHOR_CENTER);
                NW_50->setPosition(_northwestWallButton->getContentWidth()/2,_northwestWallButton->getContentHeight()/2);
                _northwestWallButton->addChild(NW_50);
                NW_50->setVisible(false);
    
                NW_25 = PolygonNode::allocWithTexture(repair_25);
                NW_25->setScale(BUTTON_SCALE2); // Magic number to rescale asset
                NW_25->setAnchor(Vec2::ANCHOR_CENTER);
                NW_25->setPosition(_northwestWallButton->getContentWidth()/2,_northwestWallButton->getContentHeight()/2);
                _northwestWallButton->addChild(NW_25);
                NW_25->setVisible(false);
    
            _northwestText =Label::alloc((std::string) "                         ", FONT);
            _northwestWallButton->addChild(_northwestText);
            _northwestText->setAnchor(Vec2::ANCHOR_CENTER);
            _northwestText->setPosition(_northwestWallButton->getContentWidth()/2,_northwestWallButton->getContentHeight()/2);
            _northwestText->setForeground(cugl::Color4(233,225,212,255));
    
        std::vector<cugl::Vec2> vec;
        vec.push_back(cugl::Vec2(0,158));
        vec.push_back(cugl::Vec2(90,0));
        vec.push_back(cugl::Vec2(220,0));
        vec.push_back(cugl::Vec2(309,153));
        
        
        _northWallButton->setPushable(vec);
        _northeastWallButton->setPushable(vec);
        _northwestWallButton->setPushable(vec);
        _southWallButton->setPushable(vec);
        _southeastWallButton->setPushable(vec);
        _southwestWallButton->setPushable(vec);
    
    
    
        _buttons = Node::alloc();
        _buttons->setScale(.5); // Magic number to rescale asset
        _buttons->setAnchor(Vec2::ANCHOR_CENTER);
        _buttons->setPosition(30,2);
        addChild(_buttons);
    
    
        //Adds the buttons to the Scene Graph
        _buttons->addChild(_northWallButton);
        _buttons->addChild(_northeastWallButton);
        _buttons->addChild(_southeastWallButton);
        _buttons->addChild(_southWallButton);
        _buttons->addChild(_southwestWallButton);
        _buttons->addChild(_northwestWallButton);
        //_northWallButton = Button::alloc(PolygonNode::allocWithTexture(UP));
    
	_northWallButton->activate(input.generateKey("northWallButton"));
	_northeastWallButton->activate(input.generateKey("northeastWallButton"));
	_southeastWallButton->activate(input.generateKey("southeastWallButton"));
	_southWallButton->activate(input.generateKey("southWallButton"));
	_southwestWallButton->activate(input.generateKey("southwestWallButton"));
	_northwestWallButton->activate(input.generateKey("northwestWallButton"));
    

    
    

    
    
    
    // Create the back button.
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    

    _repairTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    _repairTOcastle->setScale(0.6f); // Magic number to rescale asset
    
    // Create a callback function for the back button
    _repairTOcastle->setName("repairTOcastle");
    _repairTOcastle->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {

            switchscene = OVERWORLD;
        }
    });
    
    
    // Position the overworld button in the bottom right
    //Size bsize = overworld_up->getSize();
    _repairTOcastle->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _repairTOcastle->setPosition(15,_size.height-18);
    
    // Add the logo and button to the scene graph
    addChild(_repairTOcastle);
    
    // We can only activate a button AFTER it is added to a scene
    _repairTOcastle->activate(input.generateKey("repairTOcastle"));

    return true;
}


std::shared_ptr<cugl::Button> RepairScene::getWall(std::string direction) {
    if (direction=="N"){
        return _northWallButton;
    }
    else if (direction=="NE"){
        return _northeastWallButton;
    }
    else if (direction=="SE"){
        return _southwestWallButton;
    }
    else if (direction=="S"){
        return _southWallButton;
    }
    else if (direction=="SW"){
        return _southwestWallButton;
    }
    else {
        return _northwestWallButton;
    }
}

void RepairScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _repairTOcastle = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void RepairScene::update(float timestep){

    // Animate
    _actions->update(timestep);
    int _curr_wall_health;
    
    if (gameModel.getWallHealth(0) == 0 || gameModel.getWallHealth(1) == 0 || gameModel.getWallHealth(2) == 0 ||
        gameModel.getWallHealth(3) == 0 || gameModel.getWallHealth(4) == 0 || gameModel.getWallHealth(5) == 0) {
        switchscene = LOSE;
    }
    if (gameModel._currentTime > gameModel._endTime){
        if (gameModel._enemyArrayMaster[0].size()== 0 && gameModel._enemyArrayMaster[1].size()== 0 && gameModel._enemyArrayMaster[2].size()== 0 && gameModel._enemyArrayMaster[3].size()== 0 && gameModel._enemyArrayMaster[4].size()== 0 && gameModel._enemyArrayMaster[5].size()== 0) {
            switchscene = WIN;
        }
    }

    _northText->setText(std::to_string(gameModel.getWallHealth(0))+"%");
    _northeastText->setText(std::to_string(gameModel.getWallHealth(5))+"%");
    _southeastText->setText(std::to_string(gameModel.getWallHealth(4))+"%");
    _southText->setText(std::to_string(gameModel.getWallHealth(3))+"%");
    _southwestText->setText(std::to_string(gameModel.getWallHealth(2))+"%");
    _northwestText->setText(std::to_string(gameModel.getWallHealth(1))+"%");




    std::shared_ptr<Texture> image   = _assets->get<Texture>("castle");


        _curr_wall_health = gameModel.getWallHealth(0);
        if (_curr_wall_health < 25){
            _northText->setPosition(275,80);
            N_25->setVisible(true);
            N_50->setVisible(false);
            N_75->setVisible(false);
            N_100->setVisible(false);
            if (_curr_wall_health < 10){
                _northText->setPosition(290,80);
            }
        }
        else if (_curr_wall_health < 75){
            _northText->setPosition(275,80);
            N_25->setVisible(false);
            N_50->setVisible(false);
            N_75->setVisible(false);
            N_100->setVisible(false);
            if (_curr_wall_health < 50){
                N_50->setVisible(true);
            }
            else {
                N_75->setVisible(true);
            }
        }
        else {
            N_25->setVisible(false);
            N_50->setVisible(false);
            N_75->setVisible(false);
            N_100->setVisible(true);
            _northText->setPosition(275,80);
            if (_curr_wall_health == 100){
                 _northText->setPosition(260,80);
            }
        }
    

    
        _curr_wall_health = gameModel.getWallHealth(5);
        if (_curr_wall_health < 25){
            _northeastText->setPosition(275,80);
            NE_25->setVisible(true);
            NE_50->setVisible(false);
            NE_75->setVisible(false);
            NE_100->setVisible(false);
            if (_curr_wall_health < 10){
                _northeastText->setPosition(290,80);
            }
        }
        else if (_curr_wall_health < 75){
            _northeastText->setPosition(275,80);
            NE_25->setVisible(false);
            NE_50->setVisible(false);
            NE_75->setVisible(false);
            NE_100->setVisible(false);
            if (_curr_wall_health < 50){
                NE_50->setVisible(true);
            }
            else {
                NE_75->setVisible(true);
            }
        }
        else {
            NE_25->setVisible(false);
            NE_50->setVisible(false);
            NE_75->setVisible(false);
            NE_100->setVisible(true);
            _northeastText->setPosition(275,80);
            if (_curr_wall_health == 100){
                _northeastText->setPosition(260,80);
            }
        }
    

    
        _curr_wall_health = gameModel.getWallHealth(4);
        if (_curr_wall_health < 25){
            _southeastText->setPosition(35,80);
            SE_25->setVisible(true);
            SE_50->setVisible(false);
            SE_75->setVisible(false);
            SE_100->setVisible(false);
            if (_curr_wall_health < 10){
                _southeastText->setPosition(20,80);
            }
        }
        else if (_curr_wall_health < 75){
            _southeastText->setPosition(35,80);
            SE_25->setVisible(false);
            SE_50->setVisible(false);
            SE_75->setVisible(false);
            SE_100->setVisible(false);
            if (_curr_wall_health < 50){
                SE_50->setVisible(true);
            }
            else {
                SE_75->setVisible(true);
            }
        }
        else {
            SE_25->setVisible(false);
            SE_50->setVisible(false);
            SE_75->setVisible(false);
            SE_100->setVisible(true);
            _southeastText->setPosition(35,80);
            if (_curr_wall_health == 100){
                _southeastText->setPosition(50,80);
            }
        }

        _curr_wall_health = gameModel.getWallHealth(3);
        if (_curr_wall_health < 25){
            _southText->setPosition(35,80);
            S_25->setVisible(true);
            S_50->setVisible(false);
            S_75->setVisible(false);
            S_100->setVisible(false);
            if (_curr_wall_health < 10){
                _southText->setPosition(20,80);
            }
        }
        else if (_curr_wall_health < 75){
            _southText->setPosition(35,80);
            S_25->setVisible(false);
            S_50->setVisible(false);
            S_75->setVisible(false);
            S_100->setVisible(false);
            if (_curr_wall_health < 50){
                S_50->setVisible(true);
            }
            else {
                S_75->setVisible(true);
            }
        }
        else {
            S_25->setVisible(false);
            S_50->setVisible(false);
            S_75->setVisible(false);
            S_100->setVisible(true);
            _southText->setPosition(35,80);
            if (_curr_wall_health == 100){
                _southText->setPosition(50,80);
            }
        }


        _curr_wall_health = gameModel.getWallHealth(2);
        if (_curr_wall_health < 25){
            _southwestText->setPosition(35,80);
            SW_25->setVisible(true);
            SW_50->setVisible(false);
            SW_75->setVisible(false);
            SW_100->setVisible(false);
            if (_curr_wall_health < 10){
                _southwestText->setPosition(20,80);
            }
        }
        else if (_curr_wall_health < 75){
            _southwestText->setPosition(35,80);
            SW_25->setVisible(false);
            SW_50->setVisible(false);
            SW_75->setVisible(false);
            SW_100->setVisible(false);
            if (_curr_wall_health < 50){
                SW_50->setVisible(true);
            }
            else {
                SW_75->setVisible(true);
            }
        }
        else {
            SW_25->setVisible(false);
            SW_50->setVisible(false);
            SW_75->setVisible(false);
            SW_100->setVisible(true);
            _southwestText->setPosition(35,80);
            if (_curr_wall_health == 100){
                _southwestText->setPosition(50,80);
            }
        }

        _curr_wall_health = gameModel.getWallHealth(1);
        if (_curr_wall_health < 25){
            _northwestText->setPosition(275,80);
            NW_25->setVisible(true);
            NW_50->setVisible(false);
            NW_75->setVisible(false);
            NW_100->setVisible(false);
            if (_curr_wall_health < 10){
                _northwestText->setPosition(290,80);
            }
        }
        else if (_curr_wall_health < 75){
            _northwestText->setPosition(275,80);
            NW_25->setVisible(false);
            NW_50->setVisible(false);
            NW_75->setVisible(false);
            NW_100->setVisible(false);
            if (_curr_wall_health < 50){
                NW_50->setVisible(true);
            }
            else {
                NW_75->setVisible(true);
            }
        }
        else {
            NW_25->setVisible(false);
            NW_50->setVisible(false);
            NW_75->setVisible(false);
            NW_100->setVisible(true);
            _northwestText->setPosition(275,80);
            if (_curr_wall_health == 100){
                _northwestText->setPosition(260,80);
            }
        }
}



//Pause or Resume
void RepairScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        //Application::get()->setClearColor(Color4(132,180,113,255));
        Application::get()->setClearColor(Color4(0,0,0,255));
        _repairTOcastle->activate(input.findKey("repairTOcastle"));
        _northWallButton->activate(input.findKey("northWallButton"));
        _southeastWallButton->activate(input.findKey("southeastWallButton"));
        _southwestWallButton->activate(input.findKey("southwestWallButton"));
        
        
        if (gameModel._gamePlayers > 1) {
            _northeastWallButton->activate(input.findKey("northeastWallButton"));
            _southWallButton->activate(input.findKey("southWallButton"));
            _northwestWallButton->activate(input.findKey("northwestWallButton"));
        }
        else {
            _northeastText->setVisible(false);
            _northeastWallButton->setColor(Color4(10,10,10,255));
            _southText->setVisible(false);
            _southWallButton->setColor(Color4(10,10,10,255));
            _northwestText->setVisible(false);
            _northwestWallButton->setColor(Color4(10,10,10,255));
        }
        
        
    }
    else{
        _repairTOcastle->deactivate();
        _northWallButton->deactivate();
        _northeastWallButton->deactivate();
        _southeastWallButton->deactivate();
        _southWallButton->deactivate();
        _southwestWallButton->deactivate();
        _northwestWallButton->deactivate();
    }
}
