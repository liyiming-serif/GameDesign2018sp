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
    std::shared_ptr<Texture> repair_down = _assets->get<Texture>("repair_icon_selected");
    _repair_button = Button::alloc(PolygonNode::allocWithTexture(repair_up),
                                   PolygonNode::allocWithTexture(repair_down));
    
    
    // Create a callback function for the Basement buttons
    _repair_button->setName("repair");
    _repair_button->setListener([=] (const std::string& name, bool down) {
        // Only go to lookout when the button is released
        if (!down) {
            switchscene = REPAIR;
        }
    });
    
    //Positions the Basement Buttons
    //Basement Floor Center
    float centerX = basement_floor->getContentSize().width/2;
    float centerY = basement_floor->getContentSize().height/2;
    _repair_button->setScale(BUTTON_SCALE); // Magic number to rescale asset
    _repair_button->setAnchor(Vec2::ANCHOR_CENTER);
    _repair_button->setPosition(centerX,centerY);
    
    
    
    //Adds the button to the Scene Graph
    basement_floor->addChild(_repair_button);
    
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
                std::shared_ptr<Texture> image_down = _assets->get<Texture>("catapult_icon_selected");
                _catapultNorth = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                          PolygonNode::allocWithTexture(image_down));
                _catapultNorthEast = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                              PolygonNode::allocWithTexture(image_down));
               _catapultEast = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                         PolygonNode::allocWithTexture(image_down));
                _catapultSouthEast = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                              PolygonNode::allocWithTexture(image_down));
                _catapultSouth = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                          PolygonNode::allocWithTexture(image_down));
                _catapultSouthWest = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                              PolygonNode::allocWithTexture(image_down));
                _catapultWest = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                         PolygonNode::allocWithTexture(image_down));
                _catapultNorthWest = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                              PolygonNode::allocWithTexture(image_down));
    
    
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
            image_down = _assets->get<Texture>("oil_icon_selected");
            _oilNorth = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                           PolygonNode::allocWithTexture(image_down));
            _oilNorthEast = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                               PolygonNode::allocWithTexture(image_down));
            _oilEast = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                          PolygonNode::allocWithTexture(image_down));
            _oilSouthEast = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                               PolygonNode::allocWithTexture(image_down));
            _oilSouth = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                           PolygonNode::allocWithTexture(image_down));
            _oilSouthWest = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                               PolygonNode::allocWithTexture(image_down));
            _oilWest = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                          PolygonNode::allocWithTexture(image_down));
            _oilNorthWest = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                               PolygonNode::allocWithTexture(image_down));
    
        // Create a callback function for the Oil button
    
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
            image_down = _assets->get<Texture>("ballista_icon_selected");
            _ballistaNorth = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                           PolygonNode::allocWithTexture(image_down));
            _ballistaNorthEast = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                               PolygonNode::allocWithTexture(image_down));
            _ballistaEast = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                          PolygonNode::allocWithTexture(image_down));
            _ballistaSouthEast = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                               PolygonNode::allocWithTexture(image_down));
            _ballistaSouth = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                           PolygonNode::allocWithTexture(image_down));
            _ballistaSouthWest = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                               PolygonNode::allocWithTexture(image_down));
            _ballistaWest = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                          PolygonNode::allocWithTexture(image_down));
            _ballistaNorthWest = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                               PolygonNode::allocWithTexture(image_down));
    
        // Create a callback function for the Ballista buttons
            _ballistaNorth->setName("ballista");
            _ballistaNorth->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
                }
            });
            _ballistaNorthEast->setName("ballista");
            _ballistaNorthEast->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
                }
            });
            _ballistaSouthEast->setName("ballista");
            _ballistaSouthEast->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
                }
            });
            _ballistaSouth->setName("ballista");
            _ballistaSouth->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
                }
            });
            _ballistaSouthWest->setName("ballista");
            _ballistaSouthWest->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
                }
            });
            _ballistaNorthWest->setName("ballista");
            _ballistaNorthWest->setListener([=] (const std::string& name, bool down) {
                // Only switch scenes when the button is released
                if (!down) {
                    switchscene = BALLISTA;
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
    std::shared_ptr<Texture> lookout_down = _assets->get<Texture>("lookout_icon_selected");
    _lookout_button = Button::alloc(PolygonNode::allocWithTexture(lookout_up),
                                    PolygonNode::allocWithTexture(lookout_down));
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
    _ballistaNorth->activate(200); _ballistaNorthEast->activate(3); _ballistaSouthEast->activate(4);
    _ballistaSouth->activate(5); _ballistaSouthWest->activate(6); _ballistaNorthWest->activate(7);
    _lookout_button->activate(8);
    _quitButton->activate(9);
//    _up_button->activate(10);
//    _down_button->activate(11);
    _repair_button->activate(13);
    
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


//void OverworldScene::touchDragCB(const TouchEvent& event, const Vec2& previous, bool focus) {
//    if(_active) {
//        if (event.position.y - previous.y > 30 ) {
//            CULog("Move Up");
//            OverworldScene::doMove(_moveup, currentCastleFloor);
//            OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
//            OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor+1);
//            currentCastleFloor += 1;
//        }
//        else if (event.position.y - previous.y < -30 && currentCastleFloor>0 && !_actions->isActive(ACT_KEY)){
//            CULog("Move Down");
//            OverworldScene::doMove(_movedn, currentCastleFloor);
//            OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
//            OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor-1);
//            currentCastleFloor -= 1;
//        }
//    }
//}


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
        _quitButton->activate(2);
        _ballistaNorth->activate(30);
        _lookout_button->activate(8);
        Application::get()->setClearColor(Color4(132,180,113,255));
    }
    else{
        _quitButton->deactivate();
        _ballistaNorth->deactivate();
        _lookout_button->deactivate();
    }
}




