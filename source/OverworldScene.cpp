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
#define WIN         11
#define LOSE        12

#define DRAW_SCALE 32


#define BUTTON_SCALE .9f
#define BUTTON_SCALE2 1.06f
#define TOWER_SCALE .55f
#define FLOOR_SCALEx .52f
#define FLOOR_SCALEy .534f


using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

/** Define the time settings for animation */
#define DURATION .5f
#define DURATION2 20.0f
#define DMG_DURATION 1.0f
#define DISTANCE 200
#define REPEATS  3
#define ACT_KEY  "current"
#define DMG_ACT_KEY "marker"

// Decide when to use heavy damage indicator
#define HVY_DMG 6

#define JUNGLE  5
#define SNOW  8


bool OverworldScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    
    

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
    _castleFadeINSTANT = FadeIn::alloc(0.0f);
    _castleFadeOUT = FadeOut::alloc(DURATION);
	_dmgFadeOUT = FadeOut::alloc(DMG_DURATION);

    
    // Creates the Scene Graph
    _background = Node::alloc();
    _levels = Node::alloc();

	// Allocate the damage indicators
	std::shared_ptr<Texture> dmg_img = _assets->get<Texture>("dmg_indicator_n");
	std::shared_ptr<PolygonNode> dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size/dmg_ind->getSize());
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
    
    
    std::shared_ptr<Texture> tut_swipe  = _assets->get<Texture>("tutorial_swipe");
    _swipe = PolygonNode::allocWithTexture(tut_swipe);
    _swipe->setScale(1.0); // Magic number to rescale asset
    _swipe->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _swipe->setPosition(-_size.width/2,0);
    _background->addChild(_swipe);
    
// CREATES THE FLOORS
    
    std::shared_ptr<Texture> room_lock  = _assets->get<Texture>("room_lock");
    
    // Basement Floor
        std::shared_ptr<Texture> basementFloor_texture  = _assets->get<Texture>("basement_floor");
        _basement_floor = PolygonNode::allocWithTexture(basementFloor_texture);
        _basement_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
        _basement_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        _basement_floor->setPosition(_size.width/2.14,-3*_size.height);
    
    
        // Create the Basement Buttons
        std::shared_ptr<Texture> repair_up   = _assets->get<Texture>("repair_icon");
        _repair_button = Button::alloc(PolygonNode::allocWithTexture(repair_up));
        std::shared_ptr<Texture> ammo_up   = _assets->get<Texture>("ammo_icon");
        _ammo_button = Button::alloc(PolygonNode::allocWithTexture(ammo_up));
        std::shared_ptr<Texture> mage_up   = _assets->get<Texture>("mage_icon");
        _mage_button = Button::alloc(PolygonNode::allocWithTexture(mage_up));
    
        _repair_buttonLOCKED = PolygonNode::allocWithTexture(room_lock);
        _ammo_buttonLOCKED = PolygonNode::allocWithTexture(room_lock);
        _mage_buttonLOCKED = PolygonNode::allocWithTexture(room_lock);
    
    
    
        // Create a callback function for the Basement buttons
            _repair_button->setName("repair");
            _repair_button->setListener([=] (const std::string& name, bool down) {
                // Only go to lookout when the button is released
                if (!down) {
                    if (click){
                        _roomClick+=1;
                        switchscene = REPAIR;
                    }
                    
                }
            });
            _ammo_button->setName("ammo");
            _ammo_button->setListener([=] (const std::string& name, bool down) {
                // Only go to lookout when the button is released
                if (!down) {
                    if (click){
                        _roomClick+=1;
                        switchscene = AMMO;
                    }
                }
            });
            _mage_button->setName("mage");
            _mage_button->setListener([=] (const std::string& name, bool down) {
                // Only go to lookout when the button is released
                if (!down) {
                    if (click){
                        _roomClick+=1;
                        switchscene = MAGE;
                    }
                }
            });
    
        //Positions the Basement Buttons
        //Basement Floor Center
        float centerX = _basement_floor->getContentSize().width/2;
        float centerY = _basement_floor->getContentSize().height/2;
        _repair_button->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _repair_button->setAnchor(Vec2::ANCHOR_CENTER);
        _repair_button->setPosition(centerX-.23*_basement_floor->getContentWidth(),centerY);
    
        _repair_buttonLOCKED->setScale(BUTTON_SCALE2); // Magic number to rescale asset
        _repair_buttonLOCKED->setAnchor(Vec2::ANCHOR_CENTER);
        _repair_buttonLOCKED->setPosition(_repair_button->getContentWidth()/2,_repair_button->getContentHeight()/2);
    
        _ammo_button->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _ammo_button->setAnchor(Vec2::ANCHOR_CENTER);
        _ammo_button->setPosition(centerX+.12*_basement_floor->getContentWidth(),centerY+.19*_basement_floor->getContentHeight());
    
        _ammo_buttonLOCKED->setScale(BUTTON_SCALE2); // Magic number to rescale asset
        _ammo_buttonLOCKED->setAnchor(Vec2::ANCHOR_CENTER);
        _ammo_buttonLOCKED->setPosition(_ammo_button->getContentWidth()/2,_ammo_button->getContentHeight()/2);
    
        _mage_button->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _mage_button->setAnchor(Vec2::ANCHOR_CENTER);
        _mage_button->setPosition(centerX+.12*_basement_floor->getContentWidth(),centerY-.19*_basement_floor->getContentHeight());

        _mage_buttonLOCKED->setScale(BUTTON_SCALE2); // Magic number to rescale asset
        _mage_buttonLOCKED->setAnchor(Vec2::ANCHOR_CENTER);
        _mage_buttonLOCKED->setPosition(_mage_button->getContentWidth()/2,_mage_button->getContentHeight()/2);
    
    
    
        //Adds the button to the Scene Graph
        _basement_floor->addChild(_repair_button);
        _repair_button->addChild(_repair_buttonLOCKED);
        _basement_floor->addChild(_ammo_button);
        _ammo_button->addChild(_ammo_buttonLOCKED);
        _basement_floor->addChild(_mage_button);
        _mage_button->addChild(_mage_buttonLOCKED);
    
    
        //Adds the Basement Floor to the castle
        _levels->addChild(_basement_floor);
    
    // Oil Floor
        std::shared_ptr<Texture> oilFloor_texture  = _assets->get<Texture>("oil_floor");
        std::shared_ptr<Texture> oilFloor_texture_s  = _assets->get<Texture>("oil_floor_s");
        std::shared_ptr<Texture> oilFloor_texture_d  = _assets->get<Texture>("oil_floor_d");
        if (gameModel.level<JUNGLE) {
            _oil_floor = PolygonNode::allocWithTexture(oilFloor_texture);
        }
        else if (gameModel.level<SNOW) {
            _oil_floor = PolygonNode::allocWithTexture(oilFloor_texture_s);
        }
        else {
            _oil_floor = PolygonNode::allocWithTexture(oilFloor_texture_d);
        }
        _oil_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
        _oil_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        _oil_floor->setPosition(_size.width/2.14,-2*_size.height);
    
    
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
    
            _oilNorthLOCKED = PolygonNode::allocWithTexture(room_lock);
            _oilNorthEastLOCKED = PolygonNode::allocWithTexture(room_lock);
            _oilNorthWestLOCKED = PolygonNode::allocWithTexture(room_lock);
            _oilSouthLOCKED = PolygonNode::allocWithTexture(room_lock);
            _oilSouthEastLOCKED = PolygonNode::allocWithTexture(room_lock);
            _oilSouthWestLOCKED = PolygonNode::allocWithTexture(room_lock);
    
        // Create a callback function for the Oil button
            _oilNorth->setName("oilNorth");
            _oilNorth->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    if (click){
                        _roomClick+=1;
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
                        _roomClick+=1;
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
                        _roomClick+=1;
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
                        _roomClick+=1;
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
                        _roomClick+=1;
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
                        _roomClick+=1;
                        switchscene = OIL;
                        direction = 5;
                    }
                }
            });
    
    
        //Positions the Oil Buttons
            //Oil Floor Center
            centerX = _oil_floor->getContentSize().width/2;
            centerY = _oil_floor->getContentSize().height/2;
    
            _oilNorth->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilNorth->setAnchor(Vec2::ANCHOR_CENTER);
            _oilNorth->setPosition(centerX,centerY+.27*_oil_floor->getContentHeight());
    
            _oilNorthLOCKED->setScale(BUTTON_SCALE2); // Magic number to rescale asset
            _oilNorthLOCKED->setAnchor(Vec2::ANCHOR_CENTER);
            _oilNorthLOCKED->setPosition(_oilNorth->getContentWidth()/2,_oilNorth->getContentHeight()/2);
    
            _oilNorthEast->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilNorthEast->setAnchor(Vec2::ANCHOR_CENTER);
            _oilNorthEast->setPosition(centerX+.24*_oil_floor->getContentWidth(),centerY+.13*_oil_floor->getContentHeight());
            _oilNorthEast->setAngle(-M_PI/3);
    
            _oilNorthEastLOCKED->setScale(BUTTON_SCALE2); // Magic number to rescale asset
            _oilNorthEastLOCKED->setAnchor(Vec2::ANCHOR_CENTER);
            _oilNorthEastLOCKED->setPosition(_oilNorthEast->getContentWidth()/2,_oilNorthEast->getContentHeight()/2);
    
            _oilSouthEast->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilSouthEast->setAnchor(Vec2::ANCHOR_CENTER);
            _oilSouthEast->setPosition(centerX+.24*_oil_floor->getContentWidth(),centerY-.13*_oil_floor->getContentHeight());
            _oilSouthEast->setAngle(-2*M_PI/3);
    
            _oilSouthEastLOCKED->setScale(BUTTON_SCALE2); // Magic number to rescale asset
            _oilSouthEastLOCKED->setAnchor(Vec2::ANCHOR_CENTER);
            _oilSouthEastLOCKED->setPosition(_oilSouthEast->getContentWidth()/2,_oilSouthEast->getContentHeight()/2);
    
            _oilSouth->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilSouth->setAnchor(Vec2::ANCHOR_CENTER);
            _oilSouth->setPosition(centerX,centerY-.27*_oil_floor->getContentHeight());
            _oilSouth->setAngle(-3*M_PI/3);
    
            _oilSouthLOCKED->setScale(BUTTON_SCALE2); // Magic number to rescale asset
            _oilSouthLOCKED->setAnchor(Vec2::ANCHOR_CENTER);
            _oilSouthLOCKED->setPosition(_oilSouth->getContentWidth()/2,_oilSouth->getContentHeight()/2);
    
            _oilSouthWest->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilSouthWest->setAnchor(Vec2::ANCHOR_CENTER);
            _oilSouthWest->setPosition(centerX-.24*_oil_floor->getContentWidth(),centerY-.13*_oil_floor->getContentHeight());
            _oilSouthWest->setAngle(-4*M_PI/3);
    
            _oilSouthWestLOCKED->setScale(BUTTON_SCALE2); // Magic number to rescale asset
            _oilSouthWestLOCKED->setAnchor(Vec2::ANCHOR_CENTER);
            _oilSouthWestLOCKED->setPosition(_oilSouthWest->getContentWidth()/2,_oilSouthWest->getContentHeight()/2);
    
            _oilNorthWest->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _oilNorthWest->setAnchor(Vec2::ANCHOR_CENTER);
            _oilNorthWest->setPosition(centerX-.24*_oil_floor->getContentWidth(),centerY+.13*_oil_floor->getContentHeight());
            _oilNorthWest->setAngle(M_PI/3);
    
            _oilNorthWestLOCKED->setScale(BUTTON_SCALE2); // Magic number to rescale asset
            _oilNorthWestLOCKED->setAnchor(Vec2::ANCHOR_CENTER);
            _oilNorthWestLOCKED->setPosition(_oilNorthWest->getContentWidth()/2,_oilNorthWest->getContentHeight()/2);
    
        //Adds the buttons to the Scene Graph
        _oil_floor->addChild(_oilNorth);
        _oilNorth->addChild(_oilNorthLOCKED);
        _oil_floor->addChild(_oilNorthEast);
        _oilNorthEast->addChild(_oilNorthEastLOCKED);
        _oil_floor->addChild(_oilSouthEast);
        _oilSouthEast->addChild(_oilSouthEastLOCKED);
        _oil_floor->addChild(_oilSouth);
        _oilSouth->addChild(_oilSouthLOCKED);
        _oil_floor->addChild(_oilSouthWest);
        _oilSouthWest->addChild(_oilSouthWestLOCKED);
        _oil_floor->addChild(_oilNorthWest);
        _oilNorthWest->addChild(_oilNorthWestLOCKED);
    
    //Adds the Oil Floor to the castle
    _levels->addChild(_oil_floor);
    
    
    
    
    //Ballista Floor
        std::shared_ptr<Texture> ballistaFloor_texture  = _assets->get<Texture>("ballista_floor");
        std::shared_ptr<Texture> ballistaFloor_texture_s  = _assets->get<Texture>("ballista_floor_s");
        std::shared_ptr<Texture> ballistaFloor_texture_d  = _assets->get<Texture>("ballista_floor_d");
        if (gameModel.level<JUNGLE) {
			_ballista_floor = PolygonNode::allocWithTexture(ballistaFloor_texture);
        }
        else if (gameModel.level<SNOW) {
            _ballista_floor = PolygonNode::allocWithTexture(ballistaFloor_texture_s);
        }
        else {
            _ballista_floor = PolygonNode::allocWithTexture(ballistaFloor_texture_d);
        }
        _ballista_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
        _ballista_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        _ballista_floor->setPosition(_size.width/2.14,-_size.height);
    
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
                        _roomClick+=1;
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
                        _roomClick+=1;
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
                        _roomClick+=1;
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
                        _roomClick+=1;
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
                        _roomClick+=1;
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
                        _roomClick+=1;
                        switchscene = BALLISTA;
                        direction = 5;
                    }
                }
            });



    
        //Positions the Ballista buttons
            //Ballista Floor Center
            centerX = _ballista_floor->getContentSize().width/2;
            centerY = _ballista_floor->getContentSize().height/2;
    
            _ballistaNorth->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaNorth->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaNorth->setPosition(centerX,centerY+.27*_ballista_floor->getContentHeight());
    
            _ballistaNorthEast->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaNorthEast->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaNorthEast->setPosition(centerX+.24*_ballista_floor->getContentWidth(),centerY+.13*_ballista_floor->getContentHeight());
            _ballistaNorthEast->setAngle(-M_PI/3);
    
            _ballistaSouthEast->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaSouthEast->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaSouthEast->setPosition(centerX+.24*_ballista_floor->getContentWidth(),centerY-.13*_ballista_floor->getContentHeight());
            _ballistaSouthEast->setAngle(-2*M_PI/3);
    
            _ballistaSouth->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaSouth->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaSouth->setPosition(centerX,centerY-.27*_ballista_floor->getContentHeight());
            _ballistaSouth->setAngle(-3*M_PI/3);
    
            _ballistaSouthWest->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaSouthWest->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaSouthWest->setPosition(centerX-.24*_ballista_floor->getContentWidth(),centerY-.13*_ballista_floor->getContentHeight());
            _ballistaSouthWest->setAngle(-4*M_PI/3);
    
            _ballistaNorthWest->setScale(BUTTON_SCALE); // Magic number to rescale asset
            _ballistaNorthWest->setAnchor(Vec2::ANCHOR_CENTER);
            _ballistaNorthWest->setPosition(centerX-.24*_ballista_floor->getContentWidth(),centerY+.13*_ballista_floor->getContentHeight());
            _ballistaNorthWest->setAngle(M_PI/3);
    
        //Adds the buttons to the Scene Graph
            _ballista_floor->addChild(_ballistaNorth);
            _ballista_floor->addChild(_ballistaNorthEast);
            _ballista_floor->addChild(_ballistaSouthEast);
            _ballista_floor->addChild(_ballistaSouth);
            _ballista_floor->addChild(_ballistaSouthWest);
            _ballista_floor->addChild(_ballistaNorthWest);
    
    //Adds the Ballista Floor to the castle
    _levels->addChild(_ballista_floor);
    
    
    
    
    
    // Lookout Floor
    std::shared_ptr<Texture> lookoutFloor_texture  = _assets->get<Texture>("lookout_floor");
    std::shared_ptr<Texture> lookoutFloor_texture_s  = _assets->get<Texture>("lookout_floor_s");
    std::shared_ptr<Texture> lookoutFloor_texture_d  = _assets->get<Texture>("lookout_floor_d");
    if (gameModel.level<JUNGLE) {
        _lookout_floor = PolygonNode::allocWithTexture(lookoutFloor_texture);
    }
    else if (gameModel.level<SNOW) {
        _lookout_floor = PolygonNode::allocWithTexture(lookoutFloor_texture_s);
    }
    else {
        _lookout_floor = PolygonNode::allocWithTexture(lookoutFloor_texture_d);
    }
    _lookout_floor->setScale(FLOOR_SCALEx,FLOOR_SCALEy); // Magic number to rescale asset
    _lookout_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
    _lookout_floor->setPosition(_size.width/2.14,0);
    
    
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
                _roomClick+=1;
                switchscene = LOOKOUT;
            }
        }
    });
    
    
    //Lookout Floor Center
    centerX = _lookout_floor->getContentSize().width/2;
    centerY = _lookout_floor->getContentSize().height/2;
    
    // Position the LOOKOUT button in the center
    _lookout_button->setAnchor(Vec2::ANCHOR_CENTER);
    _lookout_button->setPosition(centerX,centerY);
    
    //Adds the button to the Scene Graph
    _lookout_floor->addChild(_lookout_button);
    
    //Adds the Lookout Floor to the castle
    _levels->addChild(_lookout_floor);
    
    
    
    
    
    
    //Completes the Background layout
    _levels->setAnchor(Vec2::ANCHOR_CENTER);
    _levels->setPosition(_size.width/30,0);
    castleOrigin = Vec2 (_size.width/30,0);
    
    
    _background->addChild(_levels);
    
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    
    std::shared_ptr<Texture> tut_tap  = _assets->get<Texture>("tutorial_tap");
    _tap = PolygonNode::allocWithTexture(tut_tap);
    _tap->setScale(1.0); // Magic number to rescale asset
    _tap->setAnchor(Vec2::ANCHOR_TOP_CENTER);
    //_tap->setPosition(_size.width*.26f,_size.height*.07f);
    _background->addChild(_tap);
    
    
    std::shared_ptr<Texture> menu_tex   = _assets->get<Texture>("menu");
    _menuButton = Button::alloc(PolygonNode::allocWithTexture(menu_tex));
    
    // Position the button in the bottom right corner
    _menuButton->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _menuButton->setPosition(_size.width-(_menuButton->getWidth()/2+5),_size.height-5);
    _menuButton->setScale(.5f);
    
    
    // Create a callback function for the button
    _menuButton->setName("menu");
    _menuButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
                _pauseBG->setVisible(!_pauseBG->isVisible());
        }
    });
    
    
    std::shared_ptr<Texture> pauseBG  = _assets->get<Texture>("pause_BG");
    _pauseBG = PolygonNode::allocWithTexture(pauseBG);
    _pauseBG->setScale(.8); // Magic number to rescale asset
    _pauseBG->setAnchor(Vec2::ANCHOR_CENTER);
    _pauseBG->setPosition(_size.width/2,_size.height/2);
    _pauseBG->setVisible(false);
    
    
    std::shared_ptr<Texture> pauseQUIT   = _assets->get<Texture>("pause_quit");
    _pauseQUIT = Button::alloc(PolygonNode::allocWithTexture(pauseQUIT));
    _pauseQUIT->setAnchor(Vec2::ANCHOR_CENTER);
    _pauseQUIT->setPosition(_pauseBG->getContentWidth()/2, _pauseBG->getContentHeight()/2-90);
    _pauseQUIT->setScale(1.0f);
    _pauseBG->addChild(_pauseQUIT);
    
    
    // Create a callback function for the button
    _pauseQUIT->setName("quit");
    _pauseQUIT->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = MENU;
        }
    });
    
    std::shared_ptr<Texture> pauseREPLAY   = _assets->get<Texture>("pause_replay");
    _pauseREPLAY = Button::alloc(PolygonNode::allocWithTexture(pauseREPLAY));
    _pauseREPLAY->setAnchor(Vec2::ANCHOR_CENTER);
    _pauseREPLAY->setPosition(_pauseBG->getContentWidth()/2, _pauseBG->getContentHeight()/2-180);
    _pauseREPLAY->setScale(1.0f);
    _pauseBG->addChild(_pauseREPLAY);
    
    
    // Create a callback function for the button
    _pauseREPLAY->setName("replay");
    _pauseREPLAY->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            _pauseBG->setVisible(false);
            switchscene = OVERWORLD;
        }
    });
    
    std::shared_ptr<Texture> pauseBACK   = _assets->get<Texture>("pause_back");
    _pauseBACK = Button::alloc(PolygonNode::allocWithTexture(pauseBACK));
    _pauseBACK->setAnchor(Vec2::ANCHOR_CENTER);
    _pauseBACK->setPosition(_pauseBG->getContentWidth()/2, _pauseBG->getContentHeight()/2);
    _pauseBACK->setScale(1.0f);
    _pauseBG->addChild(_pauseBACK);
    
    
    // Create a callback function for the button
    _pauseBACK->setName("back");
    _pauseBACK->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
             _pauseBG->setVisible(false);
        }
    });
    
    
    // Add the background to the scene graph
    addChild(_background);
    addChild(_menuButton);
    addChild(_pauseBG);
	// Add damage indicators overlay
	for (int i = 0; i < _dmgIndicators.size(); i++) {
		addChild(_dmgIndicators.at(i));
	}
    
    
    // We can only activate a button AFTER it is added to a scene
    _menuButton->activate(input.generateKey("menuButton"));
    
    _pauseBACK->activate(input.generateKey("pauseBack"));
    _pauseREPLAY->activate(input.generateKey("pauseReplay"));
    _pauseQUIT->activate(input.generateKey("pauseQuit"));
    
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
		_dmgIndicators.clear();
		_active = false;
        _assets = nullptr;
		_actions = nullptr;
        _moveup = nullptr;
        _movedn = nullptr;
        _castleFadeIN = nullptr;
        _castleFadeOUT = nullptr;
		_castleFadeINSTANT = nullptr;
		_dmgFadeOUT = nullptr;
		_background = nullptr;
		_levels = nullptr;
		_oil_floor = nullptr;
		_ballista_floor = nullptr;
		_lookout_floor = nullptr;
		_cloud1 = nullptr;
		_cloud2 = nullptr;
		_cloud3 = nullptr;
		_move1 = nullptr;
		_move2 = nullptr;
		_move3 = nullptr;

		_castle_background = nullptr;
		_castle_flag = nullptr;
		_castle_black = nullptr;
		_castle_ballista = nullptr;
		_castle_oil = nullptr;
		_castle_lookout = nullptr;
		_castle_basement = nullptr;

		_menuButton = nullptr;
        _pauseBACK = nullptr;
        _pauseREPLAY = nullptr;
        _pauseQUIT = nullptr;

		_ballistaNorth = nullptr;
		_ballistaNorthEast = nullptr;
		_ballistaEast = nullptr;
		_ballistaSouthEast = nullptr;
		_ballistaSouth = nullptr;
		_ballistaSouthWest = nullptr;
		_ballistaWest = nullptr;
		_ballistaNorthWest = nullptr;

		_oilNorth = nullptr;
		_oilNorthEast = nullptr;
		_oilEast = nullptr;
		_oilSouthEast = nullptr;
		_oilSouth = nullptr;
		_oilSouthWest = nullptr;
		_oilWest = nullptr;
		_oilNorthWest = nullptr;

		_oilNorthLOCKED = nullptr;
		_oilNorthEastLOCKED = nullptr;
		_oilEastLOCKED = nullptr;
		_oilSouthEastLOCKED = nullptr;
		_oilSouthLOCKED = nullptr;
		_oilSouthWestLOCKED = nullptr;
		_oilWestLOCKED = nullptr;
		_oilNorthWestLOCKED = nullptr;

		_lookout_button = nullptr;

		_repair_button = nullptr;
		_mage_button = nullptr;
		_ammo_button = nullptr;

		_repair_buttonLOCKED = nullptr;
		_mage_buttonLOCKED = nullptr;
		_ammo_buttonLOCKED = nullptr;
    }
}

void OverworldScene::resetCastle(){
    _levels->setPosition(castleOrigin);
    _castle_basement->setVisible(false);
    _castle_ballista->setVisible(false);
    _castle_oil->setVisible(false);
    _castle_lookout->setVisible(true);
    CULog("test");
    //OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
    currentCastleFloor = 3;
    OverworldScene::doFadeIn(_castleFadeINSTANT, currentCastleFloor);
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
        return _castle_oil;
    }
    else if (floor==2){
        return _castle_ballista;
    }
    else {
        return _castle_lookout;
    }
}

void OverworldScene::doFadeIn(const std::shared_ptr<FadeIn>& action, int floor) {
    std::shared_ptr<cugl::PolygonNode> _curr_castle_floor;
    _curr_castle_floor = OverworldScene::getTowerView(floor);
    if (!_curr_castle_floor->isVisible()) {
        _curr_castle_floor->setVisible(true);
    }
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

void OverworldScene::pollDmgIndicators() {
	for (int i = 0; i < 6; i++) {
		if (gameModel.getDmgHealth(i) > 0) {
			//turn on damage indicator for that side
			bool succ;
			if (gameModel.getDmgHealth(i) > HVY_DMG) {
				_dmgIndicators.at(i+6)->setColor(Color4::WHITE);
				succ = _actions->activate(DMG_ACT_KEY + i+6, _dmgFadeOUT, _dmgIndicators.at(i+6));
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

//void OverworldScene::shakeScreen() {
//
//}


void OverworldScene::resetTutorial() {
    _swipeDN = 0;
    _swipeUP=0;
    _roomClick = 0;
    _swipeTutorial=false;

}



void OverworldScene::disableButtons() {
    CULog("disable");


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
    
    CULog("enable");
    _menuButton->activate(input.findKey("menuButton"));
    
    _lookout_button->activate(input.findKey("lookout_button"));
    
    
//            _oilNorth->activate(input.findKey("oilNorth"));
//        _repair_button->activate(input.findKey("repair_button"));
    
    if (gameModel._unlockedRooms["oil"] == true) {
        _oilNorth->activate(input.findKey("oilNorth"));
        _oilNorthLOCKED->setVisible(false);
        _oilSouthEast->activate(input.findKey("oilSouthEast"));
        _oilSouthEastLOCKED->setVisible(false);
        _oilSouthWest->activate(input.findKey("oilSouthWest"));
        _oilSouthWestLOCKED->setVisible(false);
        
    }
    else {
        _oilNorth->deactivate();
        _oilNorthLOCKED->setVisible(true);
        _oilSouthEast->deactivate();
        _oilSouthEastLOCKED->setVisible(true);
        _oilSouthWest->deactivate();
        _oilSouthWestLOCKED->setVisible(true);
    }
    
    
    _ballistaNorth->activate(input.findKey("ballistaNorth"));

    _ballistaSouthEast->activate(input.findKey("ballistaSouthEast"));
    _ballistaSouthWest->activate(input.findKey("ballistaSouthWest"));
    

    if (gameModel._unlockedRooms["repair"] == true) {
        _repair_button->activate(input.findKey("repair_button"));
        _repair_buttonLOCKED->setVisible(false);
    }
    else {
        _repair_button->deactivate();
        _repair_buttonLOCKED->setVisible(true);
    }

    
    if (gameModel._unlockedRooms["mage"] == true) {
        _mage_button->activate(input.findKey("mage_button"));
        _mage_buttonLOCKED->setVisible(false);
    }
    else {
        _mage_button->deactivate();
        _mage_buttonLOCKED->setVisible(true);
    }

    if (gameModel._unlockedRooms["ammo"] == true) {
        _ammo_button->activate(input.findKey("ammo_button"));
        _ammo_buttonLOCKED->setVisible(false);
    }
    else {
        _ammo_button->deactivate();
        _ammo_buttonLOCKED->setVisible(true);
    }
  
    if (gameModel.getNoPlayers() > 1) {
        if (gameModel._unlockedRooms["oil"] == true) {
            _oilNorthEast->activate(input.findKey("oilNorthEast"));
            _oilNorthEastLOCKED->setVisible(false);
            _oilNorthWest->activate(input.findKey("oilNorthWest"));
            _oilNorthWestLOCKED->setVisible(false);
            _oilSouth->activate(input.findKey("oilSouth"));
            _oilSouthLOCKED->setVisible(false);
        }
        else {
            _oilNorthEast->deactivate();
            _oilNorthEastLOCKED->setVisible(true);
            _oilNorthWest->deactivate();
            _oilNorthWestLOCKED->setVisible(true);
            _oilSouth->deactivate();
            _oilSouthLOCKED->setVisible(true);
        }

        
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
    
    if (_pauseBG->isVisible()) {
        CULog("visible");
        disableButtons();
        _pauseBACK->activate(input.findKey("pauseBack"));
        _pauseREPLAY->activate(input.findKey("pauseReplay"));
        _pauseQUIT->activate(input.findKey("pauseQuit"));
    }
    else {
        CULog("invisible");
        enableButtons();
        _pauseBACK->deactivate();
       _pauseREPLAY->deactivate();
       _pauseQUIT->deactivate();
    }

    //TODO: Make the relevant buttons unclickable/covered in Avatars for networked game
    if (gameModel.getWallHealth(0) == 0 || gameModel.getWallHealth(1) == 0 || gameModel.getWallHealth(2) == 0 ||
        gameModel.getWallHealth(3) == 0 || gameModel.getWallHealth(4) == 0 || gameModel.getWallHealth(5) == 0) {
        switchscene = LOSE;
    }
    if (gameModel._currentTime > gameModel._endTime){
        if (gameModel._enemyArrayMaster[0].size()== 0 && gameModel._enemyArrayMaster[1].size()== 0 && gameModel._enemyArrayMaster[2].size()== 0 && gameModel._enemyArrayMaster[3].size()== 0 && gameModel._enemyArrayMaster[4].size()== 0 && gameModel._enemyArrayMaster[5].size()== 0) {
            switchscene = WIN;
        }
    }
    

    if (_swipeUP>1 && _swipeDN > 1) {
        _swipe->setVisible(false);
    }
    
    if (_roomClick>2) {
        _tap->setVisible(false);
    }

	//poll damage indicators
	pollDmgIndicators();

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
        _menuButton->deactivate();
        disableButtons();
        _swipeDN+=1;
	}
	else if (input.vScrolling() > 0 && currentCastleFloor<3 && !_actions->isActive(ACT_KEY)) {
		//Moving up
		OverworldScene::doMove(_moveup, currentCastleFloor);
		OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
		OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor + 1);
		currentCastleFloor += 1;
        click=false;
        _menuButton->deactivate();
        disableButtons();
        _swipeUP+=1;
	}
    if (currentCastleFloor==3) {
        _tap->setPosition(_size.width*.25f,_size.height*.03f);
    }
    else if (currentCastleFloor==0) {
        _tap->setPosition(_size.width*.315f,_size.height*.260f);
    }
    else {
        _tap->setPosition(_size.width*.25f,_size.height*.333f);
    }
    if (!_actions->isActive(ACT_KEY) && !_pauseBG->isVisible()){
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
        
        if (_pauseBG->isVisible()) {
            _pauseBG->setVisible(false);
        }

        if (gameModel.level==1){
            _swipeTutorial=true;
        }
        if (_swipeTutorial && _swipeUP<1 && _swipeDN < 1) {
            _swipe->setVisible(true);
        }
        if (_swipeTutorial && _roomClick < 2) {
            _tap->setVisible(true);
        }
        if (!_swipeTutorial) {
            _swipe->setVisible(false);
            _tap->setVisible(false);
        }


		//Change scenery based on level
		std::shared_ptr<Texture> oilFloor_texture = _assets->get<Texture>("oil_floor");
		std::shared_ptr<Texture> oilFloor_texture_s = _assets->get<Texture>("oil_floor_s");
		std::shared_ptr<Texture> oilFloor_texture_d = _assets->get<Texture>("oil_floor_d");
		std::shared_ptr<Texture> ballistaFloor_texture = _assets->get<Texture>("ballista_floor");
		std::shared_ptr<Texture> ballistaFloor_texture_s = _assets->get<Texture>("ballista_floor_s");
		std::shared_ptr<Texture> ballistaFloor_texture_d = _assets->get<Texture>("ballista_floor_d");
		std::shared_ptr<Texture> lookoutFloor_texture = _assets->get<Texture>("lookout_floor");
		std::shared_ptr<Texture> lookoutFloor_texture_s = _assets->get<Texture>("lookout_floor_s");
		std::shared_ptr<Texture> lookoutFloor_texture_d = _assets->get<Texture>("lookout_floor_d");
		if (gameModel.level<JUNGLE) {
			_oil_floor->setTexture(oilFloor_texture);
			_ballista_floor->setTexture(ballistaFloor_texture);
			_lookout_floor->setTexture(lookoutFloor_texture);
		}
		else if (gameModel.level<SNOW) {
			_oil_floor->setTexture(oilFloor_texture_s);
			_ballista_floor->setTexture(ballistaFloor_texture_s);
			_lookout_floor->setTexture(lookoutFloor_texture_s);
		}
		else {
			_oil_floor->setTexture(oilFloor_texture_d);
			_ballista_floor->setTexture(ballistaFloor_texture_d);
			_lookout_floor->setTexture(lookoutFloor_texture_d);
		}
       // resetCastle();

        
    }
    else{
        _menuButton->deactivate();
        disableButtons();

		//wipe residual action animations
		for (auto const& it : _dmgIndicators) {
			it->setColor(Color4::CLEAR);
			_actions->clearAllActions(it);
		}
    }
}




