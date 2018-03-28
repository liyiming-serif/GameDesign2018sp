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
#define DRAW_SCALE 32


#define BUTTON_SCALE .9f
#define TOWER_SCALE .55f
#define FLOOR_SCALEx .52f
#define FLOOR_SCALEy .533f


using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

/** Define the time settings for animation */
#define DURATION .7f
//#define DURATION .3f
#define DISTANCE 200
#define REPEATS  3
#define ACT_KEY  "current"


bool OverworldScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    
    Size _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }
    
    _assets = assets;

    std::vector<std::vector<float>> temp;
    std::vector<int> temp2;
    for(int i = 0; i<6; i++){
        gameModel._enemyArrayMaster.push_back(temp);
        gameModel._enemiesToFreeMaster.push_back(temp2);
    }
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
    // Ballista Floor
    std::shared_ptr<Texture> ca_texture_ballista  = _assets->get<Texture>("castle_ballista");
    _castle_ballista = PolygonNode::allocWithTexture(ca_texture_ballista);
    _castle_ballista->setScale(TOWER_SCALE); // Magic number to rescale asset
    _castle_ballista->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _castle_ballista->setPosition(-_size.width/2,0);
    // Catapult Floor
    std::shared_ptr<Texture> ca_texture_catapult  = _assets->get<Texture>("castle_catapult");
    _castle_catapult = PolygonNode::allocWithTexture(ca_texture_catapult);
    _castle_catapult->setScale(TOWER_SCALE); // Magic number to rescale asset
    _castle_catapult->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _castle_catapult->setPosition(-_size.width/2,0);
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
    _background->addChild(_castle_basement);
    _background->addChild(_castle_catapult);
    _background->addChild(_castle_oil);
    _background->addChild(_castle_lookout);
    _background->addChild(_castle_ballista);
    _castle_ballista->setColor(Color4(255,255,255,0));
    _castle_basement->setColor(Color4(255,255,255,0));
    _castle_oil->setColor(Color4(255,255,255,0));
    _castle_catapult->setColor(Color4(255,255,255,0));
    
    
    currentCastleFloor = 4;
    
// CREATES THE FLOORS
    
    // Basement Floor
        std::shared_ptr<Texture> basementFloor_texture  = _assets->get<Texture>("basement_floor");
        std::shared_ptr<PolygonNode> basement_floor = PolygonNode::allocWithTexture(basementFloor_texture);
        basement_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
        basement_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        basement_floor->setPosition(_size.width/2.14,-4*_size.height);
    
    
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
                switchscene = REPAIR;
            }
        });
        _ammo_button->setName("ammo");
        _ammo_button->setListener([=] (const std::string& name, bool down) {
            // Only go to lookout when the button is released
            if (!down) {
                switchscene = AMMO;
            }
        });
        _mage_button->setName("mage");
        _mage_button->setListener([=] (const std::string& name, bool down) {
            // Only go to lookout when the button is released
            if (!down) {
                switchscene = MAGE;
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
    
    
    // Catapult Floor
        std::shared_ptr<Texture> catapultFloor_texture  = _assets->get<Texture>("catapult_floor");
        std::shared_ptr<PolygonNode> catapult_floor = PolygonNode::allocWithTexture(catapultFloor_texture);
        catapult_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
        catapult_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        catapult_floor->setPosition(_size.width/2.14,-3*_size.height);
    
    
            // Create the Catapult Buttons
                std::shared_ptr<Texture> image_up   = _assets->get<Texture>("catapult_icon");
                _catapultNorth = Button::alloc(PolygonNode::allocWithTexture(image_up));
                _catapultNorthEast = Button::alloc(PolygonNode::allocWithTexture(image_up));
                _catapultEast = Button::alloc(PolygonNode::allocWithTexture(image_up));
                _catapultSouthEast = Button::alloc(PolygonNode::allocWithTexture(image_up));
                _catapultSouth = Button::alloc(PolygonNode::allocWithTexture(image_up));
                _catapultSouthWest = Button::alloc(PolygonNode::allocWithTexture(image_up));
                _catapultWest = Button::alloc(PolygonNode::allocWithTexture(image_up));
                _catapultNorthWest = Button::alloc(PolygonNode::allocWithTexture(image_up));
    
    
            // Create a callback function for the Catapult button
    
            //Positions the Catapult Buttons
            //Catapult Floor Center
            centerX = catapult_floor->getContentSize().width/2;
            centerY = catapult_floor->getContentSize().height/2;
            _catapultNorth->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _catapultNorth->setAnchor(Vec2::ANCHOR_CENTER);
            _catapultNorth->setPosition(centerX,centerY+.27*catapult_floor->getContentHeight());
            
    
            _catapultNorthEast->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _catapultNorthEast->setAnchor(Vec2::ANCHOR_CENTER);
            _catapultNorthEast->setPosition(centerX+.24*catapult_floor->getContentWidth(),centerY+.13*catapult_floor->getContentHeight());
            _catapultNorthEast->setAngle(-M_PI/3);
    
            _catapultSouthEast->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _catapultSouthEast->setAnchor(Vec2::ANCHOR_CENTER);
            _catapultSouthEast->setPosition(centerX+.24*catapult_floor->getContentWidth(),centerY-.13*catapult_floor->getContentHeight());
            _catapultSouthEast->setAngle(-2*M_PI/3);
    
            _catapultSouth->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _catapultSouth->setAnchor(Vec2::ANCHOR_CENTER);
            _catapultSouth->setPosition(centerX,centerY-.27*catapult_floor->getContentHeight());
            _catapultSouth->setAngle(-3*M_PI/3);
    
            _catapultSouthWest->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _catapultSouthWest->setAnchor(Vec2::ANCHOR_CENTER);
            _catapultSouthWest->setPosition(centerX-.24*catapult_floor->getContentWidth(),centerY-.13*catapult_floor->getContentHeight());
            _catapultSouthWest->setAngle(-4*M_PI/3);
    
            _catapultNorthWest->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _catapultNorthWest->setAnchor(Vec2::ANCHOR_CENTER);
            _catapultNorthWest->setPosition(centerX-.24*catapult_floor->getContentWidth(),centerY+.13*catapult_floor->getContentHeight());
            _catapultNorthWest->setAngle(M_PI/3);
    
            //Adds the buttons to the Scene Graph
            catapult_floor->addChild(_catapultNorth);
            catapult_floor->addChild(_catapultNorthEast);
            catapult_floor->addChild(_catapultSouthEast);
            catapult_floor->addChild(_catapultSouth);
            catapult_floor->addChild(_catapultSouthWest);
            catapult_floor->addChild(_catapultNorthWest);
    
        //Adds the Catapult Floor to the castle
        _levels->addChild(catapult_floor);
    
    
    
    
    
    // Oil Floor
        std::shared_ptr<Texture> oilFloor_texture  = _assets->get<Texture>("oil_floor");
        std::shared_ptr<PolygonNode> oil_floor = PolygonNode::allocWithTexture(oilFloor_texture);
        oil_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
        oil_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        oil_floor->setPosition(_size.width/2.14,-2*_size.height);
    
    
        // Create the Oil Buttons
            image_up   = _assets->get<Texture>("oil_icon");
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
                    switchscene = OIL;
                    direction = 0;
                    CULog("OIL");
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
        std::shared_ptr<PolygonNode> ballista_floor = PolygonNode::allocWithTexture(ballistaFloor_texture);
        ballista_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
        ballista_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        ballista_floor->setPosition(_size.width/2.14,-_size.height);
    
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
                    switchscene = BALLISTA;
                    direction = 0;
                }
            });
            _ballistaNorthEast->setName("ballistaNortheast");
            _ballistaNorthEast->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
                     direction = 5;
                }
            });
            _ballistaSouthEast->setName("ballistaSoutheast");
            _ballistaSouthEast->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
                    direction = 4;
                }
            });
            _ballistaSouth->setName("ballistaSouth");
            _ballistaSouth->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
                    direction = 3;
                }
            });
            _ballistaSouthWest->setName("ballistaSouthwest");
            _ballistaSouthWest->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
                    direction = 2;
                }
            });
            _ballistaNorthWest->setName("ballistaNorthwest");
            _ballistaNorthWest->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
                    direction = 1;
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
    std::shared_ptr<PolygonNode> lookout_floor = PolygonNode::allocWithTexture(lookoutFloor_texture);
    //lookout_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
    lookout_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
    lookout_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
    lookout_floor->setPosition(_size.width/2.147,1);
    
    
    // Create the Lookout button
    std::shared_ptr<Texture> lookout_up   = _assets->get<Texture>("lookout_icon");
    _lookout_button = Button::alloc(PolygonNode::allocWithTexture(lookout_up));
    _lookout_button->setScale(BUTTON_SCALE); // Magic number to rescale asset
    
    // Create a callback function for the lookout button
    _lookout_button->setName("lookout");
    _lookout_button->setListener([=] (const std::string& name, bool down) {
        // Only go to lookout when the button is released
        if (!down) {
            switchscene = LOOKOUT;
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
    
    
//    //FLOOR NAVIGATION
//    std::shared_ptr<Texture> floor_up   = _assets->get<Texture>("up");
//    std::shared_ptr<Texture> floor_down = _assets->get<Texture>("down");
//    _up_button = Button::alloc(PolygonNode::allocWithTexture(floor_up),
//                               PolygonNode::allocWithTexture(floor_up));
//    _down_button = Button::alloc(PolygonNode::allocWithTexture(floor_down),
//                                 PolygonNode::allocWithTexture(floor_down));
//    
//    // Create a callback function for the MOVE UP button
//    _up_button->setName("lookout");
//    _up_button->setListener([=] (const std::string& name, bool down) {
//        // Only go to lookout when the button is released
//        if (!down && currentCastleFloor<4 && !_actions->isActive(ACT_KEY)) {
//            CULog("Move Up");
//            CULog("curent floor");
//            CULog("%d/n",currentCastleFloor);
//            OverworldScene::doMove(_moveup, currentCastleFloor);
//            CULog("fade out");
//            CULog("%d/n",currentCastleFloor);
//            OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
//            CULog("fade in");
//            CULog("%d/n",currentCastleFloor+1);
//            OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor+1);
//            currentCastleFloor += 1;
//            CULog("current floor");
//            CULog("%d/n",currentCastleFloor);
//        }
//    });
//    
//    
//    // Create a callback function for the MOVE DOWN button
//    _down_button->setName("lookout");
//    _down_button->setListener([=] (const std::string& name, bool down) {
//        // Only go to lookout when the button is released
//        if (!down && currentCastleFloor>0  && !_actions->isActive(ACT_KEY)) {
//            CULog("Move Down");
//            CULog("curent floor");
//            CULog("%d/n",currentCastleFloor);
//            OverworldScene::doMove(_movedn, currentCastleFloor);
//            CULog("fade out");
//            CULog("%d/n",currentCastleFloor);
//            OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
//            CULog("fade in");
//            CULog("%d/n",currentCastleFloor-1);
//            OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor-1);
//            currentCastleFloor -= 1;
//            CULog("current floor");
//            CULog("%d/n",currentCastleFloor);
//        }
//    });
//    
//    _up_button->setScale(0.1f); // Magic number to rescale asset
//    _up_button->setAnchor(Vec2::ANCHOR_CENTER);
//    _up_button->setPosition(-60,50);
//    
//    _down_button->setScale(0.1f); // Magic number to rescale asset
//    _down_button->setAnchor(Vec2::ANCHOR_CENTER);
//    _down_button->setPosition(-60,-50);
//    
//    _background->addChild(_up_button);
//    _background->addChild(_down_button);
    
    
    // Create the quit button.  A button has an up image and a down image
    std::shared_ptr<Texture> up   = _assets->get<Texture>("close-normal");
    std::shared_ptr<Texture> down = _assets->get<Texture>("close-selected");
    
    Size bsize = up->getSize();
    _quitButton = Button::alloc(PolygonNode::allocWithTexture(up),
                                PolygonNode::allocWithTexture(down));
    
    // Position the button in the bottom right corner
    _quitButton->setAnchor(Vec2::ANCHOR_CENTER);
    _quitButton->setPosition(_size.width-bsize.width/2,bsize.height/2);
    
    
    // Create a callback function for the button
    _quitButton->setName("close");
    _quitButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("Goodbye!");
            Application::get()->quit();
        }
    });
    
    
    
    
    
    
    // Add the background to the scene graph
    addChild(_background);
    addChild(_quitButton);

    
    
    
    
    
    // We can only activate a button AFTER it is added to a scene
    _quitButton->activate(input.generateKey("quitButton"));
    
    _lookout_button->activate(input.generateKey("lookout_button"));
    
    _oilNorth->activate(input.generateKey("oilNorth"));
    
    _ballistaNorth->activate(input.generateKey("ballistaNorth"));
	_ballistaNorthEast->activate(input.generateKey("ballistaNorthEast"));
	_ballistaSouthEast->activate(input.generateKey("ballistaSouthEast"));
    _ballistaSouth->activate(input.generateKey("ballistaSouth"));
	_ballistaSouthWest->activate(input.generateKey("ballistaSouthWest"));
	_ballistaNorthWest->activate(input.generateKey("ballistaNorthWest"));

    _repair_button->activate(input.generateKey("repair_button"));
    _ammo_button->activate(input.generateKey("ammo_button"));
    _mage_button->activate(input.generateKey("mage_button"));
    
    //    _up_button->activate(10);
    //    _down_button->activate(11);
    
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
    CULog("Do move");
    auto fcn = EasingFunction::alloc(EasingFunction::Type::CUBIC_OUT);
    _actions->activate(ACT_KEY, action, _levels, fcn);
}


std::shared_ptr<cugl::PolygonNode> OverworldScene::getTowerView(int floor) {
    if (floor==0){
        return _castle_basement;
    }
    else if (floor==1){
        return _castle_catapult;
    }
    else if (floor==2){
        return _castle_oil;
    }
    else if (floor==3){
        return _castle_ballista;
    }
    else {
        return _castle_lookout;
    }
}

void OverworldScene::doFadeIn(const std::shared_ptr<FadeIn>& action, int floor) {
    CULog("Do fade in");
    std::shared_ptr<cugl::PolygonNode> _curr_castle_floor;
    _curr_castle_floor = OverworldScene::getTowerView(floor);
    auto fcn = EasingFunction::alloc(EasingFunction::Type::CUBIC_OUT);
    _actions->activate(ACT_KEY+1, action, _curr_castle_floor, fcn);
}

void OverworldScene::doFadeOut(const std::shared_ptr<FadeOut>& action, int floor) {
    CULog("Do fade out");
    std::shared_ptr<cugl::PolygonNode> _curr_castle_floor;
    _curr_castle_floor = OverworldScene::getTowerView(floor);
    auto fcn = EasingFunction::alloc(EasingFunction::Type::CUBIC_OUT);
    _actions->activate(ACT_KEY+2, action, _curr_castle_floor, fcn);
}


void OverworldScene::update(float timestep){
    //moves enemies
    for(int i = 0; i<gameModel._enemyArrayMaster.size(); i++){
        for(int j = 0; j<gameModel._enemyArrayMaster[i].size(); j++){
            if(gameModel._enemyArrayMaster[i][j][1] < 85){
                //remove
                gameModel._enemiesToFreeMaster[i].push_back(j);
                gameModel.changeWallHealth(i, -9);
            }
            else{
                gameModel._enemyArrayMaster[i][j][1] -= 0.5;
            }
        }
    }
	//delete enemies here to not disrupt iterator
	for (int i = 0; i<gameModel._enemiesToFreeMaster.size(); i++) {
		for (int j = 0; j < gameModel._enemiesToFreeMaster[i].size(); j++) {
			gameModel._enemyArrayMaster[i].erase(gameModel._enemyArrayMaster[i].begin() + gameModel._enemiesToFreeMaster[i][j]);
		}
		gameModel._enemiesToFreeMaster[i].clear();
	}

	if (input.vScrolling() < 0 && currentCastleFloor>0 && !_actions->isActive(ACT_KEY)) {
		//Moving down
		OverworldScene::doMove(_movedn, currentCastleFloor);
		OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
		OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor - 1);
		currentCastleFloor -= 1;
	}
	else if (input.vScrolling() > 0 && currentCastleFloor<4 && !_actions->isActive(ACT_KEY)) {
		//Moving up
		OverworldScene::doMove(_moveup, currentCastleFloor);
		OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
		OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor + 1);
		currentCastleFloor += 1;
	}

	// Animate
	_actions->update(timestep);
}

void OverworldScene::setActive(bool active) {
    _active = active;
    switchscene = 0;
    if(active){
        _quitButton->activate(input.findKey("quitButton"));
        
        _lookout_button->activate(input.findKey("lookout_button"));
        
        _oilNorth->activate(input.findKey("oilNorth"));
        
        _ballistaNorth->activate(input.findKey("ballistaNorth"));
        _ballistaNorthEast->activate(input.findKey("ballistaNorthEast"));
        _ballistaNorthWest->activate(input.findKey("ballistaNorthWest"));
        _ballistaSouth->activate(input.findKey("ballistaSouth"));
        _ballistaSouthEast->activate(input.findKey("ballistaSouthEast"));
        _ballistaSouthWest->activate(input.findKey("ballistaSouthWest"));
    
        _repair_button->activate(input.findKey("repair_button"));
        _mage_button->activate(input.findKey("mage_button"));
        _ammo_button->activate(input.findKey("ammo_button"));
        
        Application::get()->setClearColor(Color4(132,180,113,255));
    }
    else{
        _quitButton->deactivate();
        
        _lookout_button->deactivate();
        
        _oilNorth->deactivate();
        
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
}




