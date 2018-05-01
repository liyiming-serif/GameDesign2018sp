//
//  OverworldScene.cpp
//  HelloWorld
//
//  Created by Noah Sterling on 3/1/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "OverworldScene.h"

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
#define DRAW_SCALE 32


#define BUTTON_SCALE .9f
#define TOWER_SCALE .55f
#define FLOOR_SCALEx .52f
#define FLOOR_SCALEy .534f


using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

/** Define the time settings for animation */
#define DURATION .7f
#define DURATION2 20.0f
#define DISTANCE 200
#define REPEATS  3
#define ACT_KEY  "current"

#define JUNGLE  2
#define SNOW  3


bool OverworldScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    
    Size _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    
    
    CULog("level %i", gameModel.level);

    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }
    
    _assets = assets;

    direction = -1;
    switchscene = 0;
    
    // Set background color
    Application::get()->setClearColor(Color4(132,180,113,255));
    
    // Allocate the manager and the actions
    _actions = ActionManager::alloc();
    
    _moveup = MoveBy::alloc(Vec2(0,-_size.height),DURATION);
    _movedn = MoveBy::alloc(Vec2(0,_size.height),DURATION);
    _castleFadeIN = FadeIn::alloc(DURATION);
    _castleFadeOUT = FadeOut::alloc(DURATION);

    
    // Creates the Scene Graph
    _background = Node::alloc();
    _levels = Node::alloc();
    
    //Creates the tower views
    // Background
    std::shared_ptr<Texture> ca_back  = _assets->get<Texture>("castle_background");
    _castle_background = PolygonNode::allocWithTexture(ca_back);
    _castle_background->setScale(TOWER_SCALE); // Magic number to rescale asset
    _castle_background->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _castle_background->setPosition(-_size.width/2,0);
    // Flag
    std::shared_ptr<Texture> ca_flag  = _assets->get<Texture>("castle_flag_overworld");
    _castle_flag = PolygonNode::allocWithTexture(ca_flag);
    _castle_flag->setScale(-TOWER_SCALE,TOWER_SCALE); // Magic number to rescale asset
    _castle_flag->setAnchor(Vec2::ANCHOR_CENTER);
    _castle_flag->setPosition(-170,190);
    // Black
    std::shared_ptr<Texture> ca_black  = _assets->get<Texture>("castle_black");
    _castle_black = PolygonNode::allocWithTexture(ca_black);
    _castle_black->setScale(TOWER_SCALE); // Magic number to rescale asset
    _castle_black->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _castle_black->setPosition(-_size.width/2,0);
    // Ballista Floor
    std::shared_ptr<Texture> ca_texture_ballista  = _assets->get<Texture>("castle_ballista");
    _castle_ballista = PolygonNode::allocWithTexture(ca_texture_ballista);
    _castle_ballista->setScale(TOWER_SCALE); // Magic number to rescale asset
    _castle_ballista->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _castle_ballista->setPosition(-_size.width/2,0);
    // Oil Floor
    std::shared_ptr<Texture> ca_texture_oil  = _assets->get<Texture>("castle_oil");
    _castle_oil = PolygonNode::allocWithTexture(ca_texture_oil);
    _castle_oil->setScale(TOWER_SCALE); // Magic number to rescale asset
    _castle_oil->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _castle_oil->setPosition(-_size.width/2,0);
    // Lookout Floor
    std::shared_ptr<Texture> ca_texture_lookout  = _assets->get<Texture>("castle_lookout");
    _castle_lookout = PolygonNode::allocWithTexture(ca_texture_lookout);
    _castle_lookout->setScale(TOWER_SCALE); // Magic number to rescale asset
    _castle_lookout->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _castle_lookout->setPosition(-_size.width/2,0);
    // Basement Floor
    std::shared_ptr<Texture> ca_texture_basement  = _assets->get<Texture>("castle_basement");
    _castle_basement = PolygonNode::allocWithTexture(ca_texture_basement);
    _castle_basement->setScale(TOWER_SCALE); // Magic number to rescale asset
    _castle_basement->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _castle_basement->setPosition(-_size.width/2,0);
    //Adds Nodes to background and sets transparency
    
    
     std::shared_ptr<Texture> c_1  = _assets->get<Texture>("cloudS");
    _cloud1 = PolygonNode::allocWithTexture(c_1);
    _cloud1->setScale(0.5625f); // Magic number to rescale asset
    _cloud1->setAnchor(Vec2::ANCHOR_CENTER);
    
    _cloud2 = PolygonNode::allocWithTexture(c_1);
    _cloud2->setScale(0.5625f); // Magic number to rescale asset
    _cloud2->setAnchor(Vec2::ANCHOR_CENTER);
    
    _cloud3 = PolygonNode::allocWithTexture(c_1);
    _cloud3->setScale(0.5625f); // Magic number to rescale asset
    _cloud3->setAnchor(Vec2::ANCHOR_CENTER);
    
    

    _background->addChild(_castle_background);
    
    _background->addChild(_cloud1);
    _background->addChild(_cloud2);
    _background->addChild(_cloud3);
    
    _background->addChild(_castle_flag);
    _background->addChild(_castle_black);
    
    _background->addChild(_castle_basement);
    _background->addChild(_castle_oil);
    _background->addChild(_castle_lookout);
    _background->addChild(_castle_ballista);
    _castle_ballista->setColor(Color4(255,255,255,0));
    _castle_basement->setColor(Color4(255,255,255,0));
    _castle_oil->setColor(Color4(255,255,255,0));
    
    
    
    
    currentCastleFloor = 3;
    
// CREATES THE FLOORS
    
    // Basement Floor
        std::shared_ptr<Texture> basementFloor_texture  = _assets->get<Texture>("basement_floor");
        std::shared_ptr<PolygonNode> basement_floor = PolygonNode::allocWithTexture(basementFloor_texture);
        basement_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
        basement_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        basement_floor->setPosition(_size.width/2.14,-3*_size.height);
    
    
        // Create the Basement Buttons
        std::shared_ptr<Texture> repair_up   = _assets->get<Texture>("repair_icon");
        _repair_button = Button::alloc(PolygonNode::allocWithTexture(repair_up));
        std::shared_ptr<Texture> ammo_up   = _assets->get<Texture>("ammo_icon");
        _ammo_button = Button::alloc(PolygonNode::allocWithTexture(ammo_up));
        std::shared_ptr<Texture> mage_up   = _assets->get<Texture>("mage_icon");
        _mage_button = Button::alloc(PolygonNode::allocWithTexture(mage_up));
    
        // Create a callback function for the Basement buttons
            _repair_button->setName("repair");
            _repair_button->setListener([=] (const std::string& name, bool down) {
                // Only go to lookout when the button is released
                if (!down) {
                    if (click){
                        switchscene = REPAIR;
                    }
                    
                }
            });
            _ammo_button->setName("ammo");
            _ammo_button->setListener([=] (const std::string& name, bool down) {
                // Only go to lookout when the button is released
                if (!down) {
                    if (click){
                        switchscene = AMMO;
                    }
                }
            });
            _mage_button->setName("mage");
            _mage_button->setListener([=] (const std::string& name, bool down) {
                // Only go to lookout when the button is released
                if (!down) {
                    if (click){
                        switchscene = MAGE;
                    }
                }
            });
    
        //Positions the Basement Buttons
        //Basement Floor Center
        float centerX = basement_floor->getContentSize().width/2;
        float centerY = basement_floor->getContentSize().height/2;
        _repair_button->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _repair_button->setAnchor(Vec2::ANCHOR_CENTER);
        _repair_button->setPosition(centerX-.23*basement_floor->getContentWidth(),centerY);
    
        _ammo_button->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _ammo_button->setAnchor(Vec2::ANCHOR_CENTER);
        _ammo_button->setPosition(centerX+.12*basement_floor->getContentWidth(),centerY+.19*basement_floor->getContentHeight());
    
        _mage_button->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _mage_button->setAnchor(Vec2::ANCHOR_CENTER);
        _mage_button->setPosition(centerX+.12*basement_floor->getContentWidth(),centerY-.19*basement_floor->getContentHeight());

    
    
    
        //Adds the button to the Scene Graph
        basement_floor->addChild(_repair_button);
        basement_floor->addChild(_ammo_button);
        basement_floor->addChild(_mage_button);
    
        //Adds the Basement Floor to the castle
        _levels->addChild(basement_floor);

    
    
    
    // Oil Floor
        std::shared_ptr<Texture> oilFloor_texture  = _assets->get<Texture>("oil_floor");
        std::shared_ptr<Texture> oilFloor_texture_s  = _assets->get<Texture>("oil_floor_s");
        std::shared_ptr<Texture> oilFloor_texture_d  = _assets->get<Texture>("oil_floor_d");
        std::shared_ptr<PolygonNode> oil_floor;
        if (gameModel.level<JUNGLE) {
            oil_floor = PolygonNode::allocWithTexture(oilFloor_texture);
        }
        else if (gameModel.level<SNOW) {
            oil_floor = PolygonNode::allocWithTexture(oilFloor_texture_s);
        }
        else {
            oil_floor = PolygonNode::allocWithTexture(oilFloor_texture_d);
        }
        oil_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
        oil_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        oil_floor->setPosition(_size.width/2.14,-_size.height);
    
    
        // Create the Oil Buttons

            std::shared_ptr<Texture> image_up   = _assets->get<Texture>("oil_icon");
            _oilNorth = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _oilNorthEast = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _oilEast = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _oilSouthEast = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _oilSouth = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _oilSouthWest = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _oilWest = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _oilNorthWest = Button::alloc(PolygonNode::allocWithTexture(image_up));
    
        // Create a callback function for the Oil button
            _oilNorth->setName("oilNorth");
            _oilNorth->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = OIL;
                        direction = 0;
                    }
                }
            });
            _oilNorthWest->setName("oilNorthwest");
            _oilNorthWest->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = OIL;
                        direction = 1;
                    }
                }
            });
            _oilSouthWest->setName("oilSouthwest");
            _oilSouthWest->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = OIL;
                        direction = 2;
                    }
                }
            });
            _oilSouth->setName("oilSouth");
            _oilSouth->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = OIL;
                        direction = 3;
                    }
                }
            });
            _oilSouthEast->setName("oilSoutheast");
            _oilSouthEast->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = OIL;
                        direction = 4;
                    }
                }
            });
            _oilNorthEast->setName("oilNortheast");
            _oilNorthEast->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = OIL;
                        direction = 5;
                    }
                }
            });
    
    
        //Positions the Oil Buttons
            //Oil Floor Center
            centerX = oil_floor->getContentSize().width/2;
            centerY = oil_floor->getContentSize().height/2;
    
            _oilNorth->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilNorth->setAnchor(Vec2::ANCHOR_CENTER);
            _oilNorth->setPosition(centerX,centerY+.27*oil_floor->getContentHeight());
    
            _oilNorthEast->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilNorthEast->setAnchor(Vec2::ANCHOR_CENTER);
            _oilNorthEast->setPosition(centerX+.24*oil_floor->getContentWidth(),centerY+.13*oil_floor->getContentHeight());
            _oilNorthEast->setAngle(-M_PI/3);
    
            _oilSouthEast->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilSouthEast->setAnchor(Vec2::ANCHOR_CENTER);
            _oilSouthEast->setPosition(centerX+.24*oil_floor->getContentWidth(),centerY-.13*oil_floor->getContentHeight());
            _oilSouthEast->setAngle(-2*M_PI/3);
    
            _oilSouth->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilSouth->setAnchor(Vec2::ANCHOR_CENTER);
            _oilSouth->setPosition(centerX,centerY-.27*oil_floor->getContentHeight());
            _oilSouth->setAngle(-3*M_PI/3);
    
            _oilSouthWest->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilSouthWest->setAnchor(Vec2::ANCHOR_CENTER);
            _oilSouthWest->setPosition(centerX-.24*oil_floor->getContentWidth(),centerY-.13*oil_floor->getContentHeight());
            _oilSouthWest->setAngle(-4*M_PI/3);
    
            _oilNorthWest->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilNorthWest->setAnchor(Vec2::ANCHOR_CENTER);
            _oilNorthWest->setPosition(centerX-.24*oil_floor->getContentWidth(),centerY+.13*oil_floor->getContentHeight());
            _oilNorthWest->setAngle(M_PI/3);
    
        //Adds the buttons to the Scene Graph
        oil_floor->addChild(_oilNorth);
        oil_floor->addChild(_oilNorthEast);
        oil_floor->addChild(_oilSouthEast);
        oil_floor->addChild(_oilSouth);
        oil_floor->addChild(_oilSouthWest);
        oil_floor->addChild(_oilNorthWest);

    //Adds the Oil Floor to the castle
    _levels->addChild(oil_floor);
    
    
    
    
    
    //Ballista Floor
        std::shared_ptr<Texture> ballistaFloor_texture  = _assets->get<Texture>("ballista_floor");
        std::shared_ptr<Texture> ballistaFloor_texture_s  = _assets->get<Texture>("ballista_floor_s");
        std::shared_ptr<Texture> ballistaFloor_texture_d  = _assets->get<Texture>("ballista_floor_d");
        std::shared_ptr<PolygonNode> ballista_floor;
        if (gameModel.level<JUNGLE) {
           ballista_floor = PolygonNode::allocWithTexture(ballistaFloor_texture);
        }
        else if (gameModel.level<SNOW) {
            ballista_floor = PolygonNode::allocWithTexture(ballistaFloor_texture_s);
        }
        else {
            ballista_floor = PolygonNode::allocWithTexture(ballistaFloor_texture_d);
        }
        ballista_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
        ballista_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        ballista_floor->setPosition(_size.width/2.14,-2*_size.height);
    
        //Creates Ballista buttons
            image_up   = _assets->get<Texture>("ballista_icon");
            _ballistaNorth = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _ballistaNorthEast = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _ballistaEast = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _ballistaSouthEast = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _ballistaSouth = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _ballistaSouthWest = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _ballistaWest = Button::alloc(PolygonNode::allocWithTexture(image_up));
            _ballistaNorthWest = Button::alloc(PolygonNode::allocWithTexture(image_up));
    
        // Create a callback function for the Ballista buttons
            _ballistaNorth->setName("ballistaNorth");
            _ballistaNorth->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = BALLISTA;
                        direction = 0;
                    }
                }
            });
            _ballistaNorthWest->setName("ballistaNorthwest");
            _ballistaNorthWest->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = BALLISTA;
                        direction = 1;
                    }
                }
            });
            _ballistaSouthWest->setName("ballistaSouthwest");
            _ballistaSouthWest->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = BALLISTA;
                        direction = 2;
                    }
                }
            });
            _ballistaSouth->setName("ballistaSouth");
            _ballistaSouth->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = BALLISTA;
                        direction = 3;
                    }
                }
            });
            _ballistaSouthEast->setName("ballistaSoutheast");
            _ballistaSouthEast->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = BALLISTA;
                        direction = 4;
                    }
                }
            });
            _ballistaNorthEast->setName("ballistaNortheast");
            _ballistaNorthEast->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        switchscene = BALLISTA;
                        direction = 5;
                    }
                }
            });



    
        //Positions the Ballista buttons
            //Ballista Floor Center
            centerX = ballista_floor->getContentSize().width/2;
            centerY = ballista_floor->getContentSize().height/2;
    
            _ballistaNorth->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaNorth->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaNorth->setPosition(centerX,centerY+.27*ballista_floor->getContentHeight());
    
            _ballistaNorthEast->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaNorthEast->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaNorthEast->setPosition(centerX+.24*ballista_floor->getContentWidth(),centerY+.13*ballista_floor->getContentHeight());
            _ballistaNorthEast->setAngle(-M_PI/3);
    
            _ballistaSouthEast->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaSouthEast->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaSouthEast->setPosition(centerX+.24*ballista_floor->getContentWidth(),centerY-.13*ballista_floor->getContentHeight());
            _ballistaSouthEast->setAngle(-2*M_PI/3);
    
            _ballistaSouth->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaSouth->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaSouth->setPosition(centerX,centerY-.27*ballista_floor->getContentHeight());
            _ballistaSouth->setAngle(-3*M_PI/3);
    
            _ballistaSouthWest->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaSouthWest->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaSouthWest->setPosition(centerX-.24*ballista_floor->getContentWidth(),centerY-.13*ballista_floor->getContentHeight());
            _ballistaSouthWest->setAngle(-4*M_PI/3);
    
            _ballistaNorthWest->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaNorthWest->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaNorthWest->setPosition(centerX-.24*ballista_floor->getContentWidth(),centerY+.13*ballista_floor->getContentHeight());
            _ballistaNorthWest->setAngle(M_PI/3);
    
        //Adds the buttons to the Scene Graph
            ballista_floor->addChild(_ballistaNorth);
            ballista_floor->addChild(_ballistaNorthEast);
            ballista_floor->addChild(_ballistaSouthEast);
            ballista_floor->addChild(_ballistaSouth);
            ballista_floor->addChild(_ballistaSouthWest);
            ballista_floor->addChild(_ballistaNorthWest);
    
    //Adds the Ballista Floor to the castle
    _levels->addChild(ballista_floor);
    
    
    
    
    
    // Lookout Floor
    std::shared_ptr<Texture> lookoutFloor_texture  = _assets->get<Texture>("lookout_floor");
    std::shared_ptr<Texture> lookoutFloor_texture_s  = _assets->get<Texture>("lookout_floor_s");
    std::shared_ptr<Texture> lookoutFloor_texture_d  = _assets->get<Texture>("lookout_floor_d");
    std::shared_ptr<PolygonNode> lookout_floor;
    if (gameModel.level<JUNGLE) {
        lookout_floor = PolygonNode::allocWithTexture(lookoutFloor_texture);
    }
    else if (gameModel.level<SNOW) {
        lookout_floor = PolygonNode::allocWithTexture(lookoutFloor_texture_s);
    }
    else {
        lookout_floor = PolygonNode::allocWithTexture(lookoutFloor_texture_d);
    }
    lookout_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
    lookout_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
    lookout_floor->setPosition(_size.width/2.14,0);
    
    
    // Create the Lookout button
    std::shared_ptr<Texture> lookout_up   = _assets->get<Texture>("lookout_icon");
    _lookout_button = Button::alloc(PolygonNode::allocWithTexture(lookout_up));
    _lookout_button->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    // Create a callback function for the lookout button
    _lookout_button->setName("lookout");
    _lookout_button->setListener([=] (const std::string& name, bool down) {
        // Only go to lookout when the button is released
        if (!down) {
            if (click){
                switchscene = LOOKOUT;
            }
        }
    });
    
    
    //Lookout Floor Center
    centerX = lookout_floor->getContentSize().width/2;
    centerY = lookout_floor->getContentSize().height/2;
    
    // Position the LOOKOUT button in the center
    _lookout_button->setAnchor(Vec2::ANCHOR_CENTER);
    _lookout_button->setPosition(centerX,centerY);
    
    //Adds the button to the Scene Graph
    lookout_floor->addChild(_lookout_button);
    
    //Adds the Lookout Floor to the castle
    _levels->addChild(lookout_floor);
    
    
    
    
    
    
    //Completes the Background layout
    _levels->setAnchor(Vec2::ANCHOR_CENTER);
    _levels->setPosition(_size.width/30,0);
    castleOrigin = Vec2 (_size.width/30,0);
    
    
    _background->addChild(_levels);
    
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    

    
    
    std::shared_ptr<Texture> menu_tex   = _assets->get<Texture>("menu");
    _menuButton = Button::alloc(PolygonNode::allocWithTexture(menu_tex));
    
    // Position the button in the bottom right corner
    _menuButton->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _menuButton->setPosition(0+5,_size.height-5);
    _menuButton->setScale(.5f);
    
    
    // Create a callback function for the button
    _menuButton->setName("menu");
    _menuButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
                switchscene = MENU;
        }
    });
    
    
    
    
    
    
    // Add the background to the scene graph
    addChild(_background);
    addChild(_menuButton);

    
    
    
    
    
    // We can only activate a button AFTER it is added to a scene
    _menuButton->activate(input.generateKey("menuButton"));
    
    _lookout_button->activate(input.generateKey("lookout_button"));
    
    _oilNorth->activate(input.generateKey("oilNorth"));
    _oilNorthEast->activate(input.generateKey("oilNorthEast"));
    _oilSouthEast->activate(input.generateKey("oilSouthEast"));
    _oilSouth->activate(input.generateKey("oilSouth"));
    _oilSouthWest->activate(input.generateKey("oilSouthWest"));
    _oilNorthWest->activate(input.generateKey("oilNorthWest"));
    
    _ballistaNorth->activate(input.generateKey("ballistaNorth"));
	_ballistaNorthEast->activate(input.generateKey("ballistaNorthEast"));
	_ballistaSouthEast->activate(input.generateKey("ballistaSouthEast"));
    _ballistaSouth->activate(input.generateKey("ballistaSouth"));
	_ballistaSouthWest->activate(input.generateKey("ballistaSouthWest"));
	_ballistaNorthWest->activate(input.generateKey("ballistaNorthWest"));

    _repair_button->activate(input.generateKey("repair_button"));
    _ammo_button->activate(input.generateKey("ammo_button"));
    _mage_button->activate(input.generateKey("mage_button"));
    

    return true;
}

void OverworldScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _background = nullptr;
        _active = false;
        _moveup = nullptr;
        _movedn = nullptr;
        _actions = nullptr;
        _castleFadeIN = nullptr;
        _castleFadeOUT = nullptr;
    }
}

void OverworldScene::resetCastle(){
    _levels->setPosition(castleOrigin);
    OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
    currentCastleFloor = 4;
    OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor);
}


void OverworldScene::doMove(const std::shared_ptr<MoveBy>& action, int floor) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::CUBIC_OUT);
    _actions->activate(ACT_KEY, action, _levels, fcn);
}


std::shared_ptr<cugl::PolygonNode> OverworldScene::getTowerView(int floor) {
    if (floor==0){
        return _castle_basement;
    }
    else if (floor==1){
        return _castle_ballista;
    }
    else if (floor==2){
        return _castle_oil;
    }
    else {
        return _castle_lookout;
    }
}

void OverworldScene::doFadeIn(const std::shared_ptr<FadeIn>& action, int floor) {
    std::shared_ptr<cugl::PolygonNode> _curr_castle_floor;
    _curr_castle_floor = OverworldScene::getTowerView(floor);
    auto fcn = EasingFunction::alloc(EasingFunction::Type::CUBIC_OUT);
    _actions->activate(ACT_KEY+1, action, _curr_castle_floor, fcn);
}

void OverworldScene::doFadeOut(const std::shared_ptr<FadeOut>& action, int floor) {
    std::shared_ptr<cugl::PolygonNode> _curr_castle_floor;
    _curr_castle_floor = OverworldScene::getTowerView(floor);
    auto fcn = EasingFunction::alloc(EasingFunction::Type::CUBIC_OUT);
    _actions->activate(ACT_KEY+2, action, _curr_castle_floor, fcn);
}

void OverworldScene::doMove1(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+3, action, object, fcn);
}

void OverworldScene::doMove2(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+4, action, object, fcn);
}

void OverworldScene::doMove3(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+5, action, object, fcn);
}


void OverworldScene::disableButtons() {

    _lookout_button->deactivate();
    
    _oilNorth->deactivate();
    _oilNorthEast->deactivate();
    _oilNorthWest->deactivate();
    _oilSouth->deactivate();
    _oilSouthEast->deactivate();
    _oilSouthWest->deactivate();
    
    
    _ballistaNorth->deactivate();
    _ballistaNorthEast->deactivate();
    _ballistaNorthWest->deactivate();
    _ballistaSouth->deactivate();
    _ballistaSouthEast->deactivate();
    _ballistaSouthWest->deactivate();
    
    _repair_button->deactivate();
    _mage_button->deactivate();
    _ammo_button->deactivate();

}

void OverworldScene::enableButtons() {
    _menuButton->activate(input.findKey("menuButton"));
    
    _lookout_button->activate(input.findKey("lookout_button"));
    
    _oilNorth->activate(input.findKey("oilNorth"));
    _oilSouthEast->activate(input.findKey("oilSouthEast"));
    _oilSouthWest->activate(input.findKey("oilSouthWest"));
    
    _ballistaNorth->activate(input.findKey("ballistaNorth"));

    _ballistaSouthEast->activate(input.findKey("ballistaSouthEast"));
    _ballistaSouthWest->activate(input.findKey("ballistaSouthWest"));
    
    _repair_button->activate(input.findKey("repair_button"));
    _mage_button->activate(input.findKey("mage_button"));
    _ammo_button->activate(input.findKey("ammo_button"));
    
    if (player_TEST > 1) {
        _oilNorthEast->activate(input.findKey("oilNorthEast"));
        _oilNorthWest->activate(input.findKey("oilNorthWest"));
        _oilSouth->activate(input.findKey("oilSouth"));

        _ballistaNorthEast->activate(input.findKey("ballistaNorthEast"));
        _ballistaNorthWest->activate(input.findKey("ballistaNorthWest"));
        _ballistaSouth->activate(input.findKey("ballistaSouth"));

    }
    else {
        _oilNorthEast->setVisible(false);
        _oilNorthWest->setVisible(false);
        _oilSouth->setVisible(false);
        _ballistaNorthEast->setVisible(false);
        _ballistaNorthWest->setVisible(false);
        _ballistaSouth->setVisible(false);
    }
}

void OverworldScene::update(float timestep){
	//poll inputs
    
    if (!click) {
        _lookout_button->setDown(false);
        
        _oilNorth->setDown(false);
        _oilSouthEast->setDown(false);
        _oilSouthWest->setDown(false);

        _ballistaNorth->setDown(false);
        _ballistaSouthEast->setDown(false);
        _ballistaSouthWest->setDown(false);
        
        _repair_button->setDown(false);
        _mage_button->setDown(false);
        _ammo_button->setDown(false);

        _oilNorthEast->setDown(false);
        _oilNorthWest->setDown(false);
        _oilSouth->setDown(false);

        _ballistaNorthEast->setDown(false);
        _ballistaNorthWest->setDown(false);
        _ballistaSouth->setDown(false);
        
    }
    
	if (input.vScrolling() < 0 && currentCastleFloor>0 && !_actions->isActive(ACT_KEY)) {
		//Moving down
		OverworldScene::doMove(_movedn, currentCastleFloor);
		OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
		OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor - 1);
		currentCastleFloor -= 1;
        click=false;
        disableButtons();
	}
	else if (input.vScrolling() > 0 && currentCastleFloor<3 && !_actions->isActive(ACT_KEY)) {
		//Moving up
		OverworldScene::doMove(_moveup, currentCastleFloor);
		OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
		OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor + 1);
		currentCastleFloor += 1;
        click=false;
        disableButtons();
	}
    if (!_actions->isActive(ACT_KEY)){
        click=true;
        enableButtons();

    }
    
    if (!_actions->isActive(ACT_KEY+3)){
        _cloud1->setPosition(-700,200);
        _move1 = MoveTo::alloc(Vec2(150,200),DURATION2);
        doMove1(_move1, _cloud1);
    }
    
    if (!_actions->isActive(ACT_KEY+4)){
        _cloud2->setPosition(-1000,50);
        _move2 = MoveTo::alloc(Vec2(200,50),DURATION2*1.3);
        doMove2(_move2, _cloud2);
    }
    
    if (!_actions->isActive(ACT_KEY+5)){
        _cloud3->setPosition(-1200,-100);
        _move3 = MoveTo::alloc(Vec2(400,-100),DURATION2*1.6);
        doMove3(_move3, _cloud3);
    }

	// Animate
	_actions->update(timestep);
}

void OverworldScene::setActive(bool active) {
    _active = active;
    switchscene = 0;
    if(active){
        enableButtons();
        Application::get()->setClearColor(Color4(255,255,255,255));
    }
    else{
        disableButtons();

    }
}




