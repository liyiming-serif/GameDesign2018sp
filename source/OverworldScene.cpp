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

using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

/** Define the time settings for animation */
#define DURATION 1.5f
#define DISTANCE 200
#define REPEATS  3
#define ACT_KEY  "current"


bool OverworldScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    
    Size size = Application::get()->getDisplaySize();
    size *= GAME_WIDTH/size.width;
    
    
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(size)) {
        return false;
    }
    
    _assets = assets;

    switchscene = 0;

    // Set background color
    Application::get()->setClearColor(Color4(132,180,113,255));
    
    // Allocate the manager and the actions
    _actions = ActionManager::alloc();
    
    _moveup = MoveBy::alloc(Vec2(0,-size.height),DURATION);
    _movedn = MoveBy::alloc(Vec2(0,size.height),DURATION);
    _castleFadeIN = FadeIn::alloc(DURATION);
    _castleFadeOUT = FadeOut::alloc(DURATION);
    _castleFadeINSTANT = FadeOut::alloc(.001f);
    
    
    
    
    

    
    

    
    // Creates the Scene Graph
    _background = Node::alloc();
    _levels = Node::alloc();
    
        // Get the ballista castle image and add it to the node.
        std::shared_ptr<Texture> ca_texture_ballista  = _assets->get<Texture>("castle_ballista");
        _castle_ballista = PolygonNode::allocWithTexture(ca_texture_ballista);
        _castle_ballista->setScale(0.5f); // Magic number to rescale asset
        _castle_ballista->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        _castle_ballista->setPosition(-size.width/2,0);

    
        // Get the catapult castle image and add it to the node.
        std::shared_ptr<Texture> ca_texture_catapult  = _assets->get<Texture>("castle_catapult");
        _castle_catapult = PolygonNode::allocWithTexture(ca_texture_catapult);
        _castle_catapult->setScale(0.5f); // Magic number to rescale asset
        _castle_catapult->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        _castle_catapult->setPosition(-size.width/2,0);


        // Get the oil castle image and add it to the node.
        std::shared_ptr<Texture> ca_texture_oil  = _assets->get<Texture>("castle_oil");
        _castle_oil = PolygonNode::allocWithTexture(ca_texture_oil);
        _castle_oil->setScale(0.5f); // Magic number to rescale asset
        _castle_oil->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        _castle_oil->setPosition(-size.width/2,0);

    
        // Get the lookout castle image and add it to the node.
        std::shared_ptr<Texture> ca_texture_lookout  = _assets->get<Texture>("castle_lookout");
        _castle_lookout = PolygonNode::allocWithTexture(ca_texture_lookout);
        _castle_lookout->setScale(0.5f); // Magic number to rescale asset
        _castle_lookout->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        _castle_lookout->setPosition(-size.width/2,0);

    
        // Get the lookout castle image and add it to the node.
        std::shared_ptr<Texture> ca_texture_basement  = _assets->get<Texture>("castle_basement");
        _castle_basement = PolygonNode::allocWithTexture(ca_texture_basement);
        _castle_basement->setScale(0.5f); // Magic number to rescale asset
        _castle_basement->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        _castle_basement->setPosition(-size.width/2,0);

        _background->addChild(_castle_basement);
        _background->addChild(_castle_catapult);
        _background->addChild(_castle_oil);
        _background->addChild(_castle_lookout);
        _background->addChild(_castle_ballista);
        _castle_ballista->setColor(Color4(255,255,255,0));
        _castle_basement->setColor(Color4(255,255,255,0));
        _castle_oil->setColor(Color4(255,255,255,0));
        _castle_catapult->setColor(Color4(255,255,255,0));




    
//        _castle_basement->setVisible(false);
//         _castle_catapult->setVisible(false);
//         _castle_oil->setVisible(false);
//         _castle_lookout->setVisible(false);
    
        currentCastleFloor = 4;
//        OverworldScene::doFadeOut(_castleFadeINSTANT, 0);
//        OverworldScene::doFadeOut(_castleFadeINSTANT, 2);
      //  OverworldScene::doFadeOut(_castleFadeINSTANT, 3);
      //  OverworldScene::doFadeOut(_castleFadeINSTANT, 4);
    
        // CREATES THE FLOORS
    
        // Basement Floor
        std::shared_ptr<Texture> basementFloor_texture  = _assets->get<Texture>("basement_floor");
        std::shared_ptr<PolygonNode> basement_floor = PolygonNode::allocWithTexture(basementFloor_texture);
        basement_floor->setScale(0.4f); // Magic number to rescale asset
        basement_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        basement_floor->setPosition(size.width/2,-4*size.height);
    
    
            // Create the Basement Buttons
    
            // Create a callback function for the Basement buttons
    
            //Positions the Basement Buttons
            //Basement Floor Center
            float centerX = basement_floor->getContentSize().width/2;
            float centerY = basement_floor->getContentSize().height/2;
    
    
    
            //Adds the button to the Scene Graph
    
    
            //Adds the Basement Floor to the castle
            _levels->addChild(basement_floor);
    
    
        // Catapult Floor
        std::shared_ptr<Texture> catapultFloor_texture  = _assets->get<Texture>("catapult_floor");
        std::shared_ptr<PolygonNode> catapult_floor = PolygonNode::allocWithTexture(catapultFloor_texture);
        catapult_floor->setScale(0.4f); // Magic number to rescale asset
        catapult_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        catapult_floor->setPosition(size.width/2,-3*size.height);
    
    
            // Create the Catapult Buttons
    
            // Create a callback function for the Catapult button
    
            //Positions the Catapult Buttons
            //Catapult Floor Center
            centerX = catapult_floor->getContentSize().width/2;
            centerY = catapult_floor->getContentSize().height/2;
    
    
    
            //Adds the button to the Scene Graph
    
    
            //Adds the Catapult Floor to the castle
            _levels->addChild(catapult_floor);
    
        // Oil Floor
        std::shared_ptr<Texture> oilFloor_texture  = _assets->get<Texture>("oil_floor");
        std::shared_ptr<PolygonNode> oil_floor = PolygonNode::allocWithTexture(oilFloor_texture);
        oil_floor->setScale(0.4f); // Magic number to rescale asset
        oil_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        oil_floor->setPosition(size.width/2,-2*size.height);
    
    
            // Create the Oil Buttons
    
            // Create a callback function for the Oil button
    
            //Positions the Oil Buttons
            //Oil Floor Center
            centerX = oil_floor->getContentSize().width/2;
            centerY = oil_floor->getContentSize().height/2;
    
    
    
            //Adds the button to the Scene Graph
    
    
            //Adds the Oil Floor to the castle
            _levels->addChild(oil_floor);
    
    
    
        //Ballista Floor
        std::shared_ptr<Texture> ballistaFloor_texture  = _assets->get<Texture>("ballista_floor");
        std::shared_ptr<PolygonNode> ballista_floor = PolygonNode::allocWithTexture(ballistaFloor_texture);
        ballista_floor->setScale(0.4f); // Magic number to rescale asset
        ballista_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        ballista_floor->setPosition(size.width/2,-size.height);

            //Creates Ballista buttons
                std::shared_ptr<Texture> ballista_up   = _assets->get<Texture>("ballista_icon");
                std::shared_ptr<Texture> ballista_down = _assets->get<Texture>("ballista_icon_selected");
                Size ballista_b_size = ballista_up->getSize();
                _ballistaNorth = Button::alloc(PolygonNode::allocWithTexture(ballista_up),
                                               PolygonNode::allocWithTexture(ballista_down));
                _ballistaNorthEast = Button::alloc(PolygonNode::allocWithTexture(ballista_up),
                                               PolygonNode::allocWithTexture(ballista_down));
                _ballistaEast = Button::alloc(PolygonNode::allocWithTexture(ballista_up),
                                               PolygonNode::allocWithTexture(ballista_down));
                _ballistaSouthEast = Button::alloc(PolygonNode::allocWithTexture(ballista_up),
                                               PolygonNode::allocWithTexture(ballista_down));
                _ballistaSouth = Button::alloc(PolygonNode::allocWithTexture(ballista_up),
                                               PolygonNode::allocWithTexture(ballista_down));
                _ballistaSouthWest = Button::alloc(PolygonNode::allocWithTexture(ballista_up),
                                               PolygonNode::allocWithTexture(ballista_down));
                _ballistaWest = Button::alloc(PolygonNode::allocWithTexture(ballista_up),
                                               PolygonNode::allocWithTexture(ballista_down));
                _ballistaNorthWest = Button::alloc(PolygonNode::allocWithTexture(ballista_up),
                                               PolygonNode::allocWithTexture(ballista_down));
    
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
    
                _ballistaNorth->setScale(0.4f); // Magic number to rescale asset
                _ballistaNorth->setAnchor(Vec2::ANCHOR_CENTER);
                _ballistaNorth->setPosition(centerX,centerY+.3*ballista_floor->getContentHeight());

                _ballistaNorthEast->setScale(0.4f); // Magic number to rescale asset
                _ballistaNorthEast->setAnchor(Vec2::ANCHOR_CENTER);
                _ballistaNorthEast->setPosition(centerX+.2*ballista_floor->getContentWidth(),centerY+.15*ballista_floor->getContentHeight());
                _ballistaNorthEast->setAngle(-M_PI/3);

                _ballistaSouthEast->setScale(0.4f); // Magic number to rescale asset
                _ballistaSouthEast->setAnchor(Vec2::ANCHOR_CENTER);
                _ballistaSouthEast->setPosition(centerX+.2*ballista_floor->getContentWidth(),centerY-.15*ballista_floor->getContentHeight());
                _ballistaSouthEast->setAngle(-2*M_PI/3);

                _ballistaSouth->setScale(0.4f); // Magic number to rescale asset
                _ballistaSouth->setAnchor(Vec2::ANCHOR_CENTER);
                _ballistaSouth->setPosition(centerX,centerY-.3*ballista_floor->getContentHeight());
                _ballistaSouth->setAngle(-3*M_PI/3);

                _ballistaSouthWest->setScale(0.4f); // Magic number to rescale asset
                _ballistaSouthWest->setAnchor(Vec2::ANCHOR_CENTER);
                _ballistaSouthWest->setPosition(centerX-.2*ballista_floor->getContentWidth(),centerY-.15*ballista_floor->getContentHeight());
                _ballistaSouthWest->setAngle(-4*M_PI/3);

                _ballistaNorthWest->setScale(0.4f); // Magic number to rescale asset
                _ballistaNorthWest->setAnchor(Vec2::ANCHOR_CENTER);
                _ballistaNorthWest->setPosition(centerX-.2*ballista_floor->getContentWidth(),centerY+.15*ballista_floor->getContentHeight());
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
        lookout_floor->setScale(0.4f); // Magic number to rescale asset
        lookout_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        lookout_floor->setPosition(size.width/2,0);
    
    
            // Create the Lookout button
            std::shared_ptr<Texture> lookout_up   = _assets->get<Texture>("lookout_icon");
            std::shared_ptr<Texture> lookout_down = _assets->get<Texture>("lookout_icon_selected");
            _lookout_button = Button::alloc(PolygonNode::allocWithTexture(lookout_up),
                                            PolygonNode::allocWithTexture(lookout_down));
            _lookout_button->setScale(1.6f); // Magic number to rescale asset
    
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
    _levels->setPosition(size.width/30,0);
    castleOrigin = Vec2 (size.width/30,0);

    
    _background->addChild(_levels);
    
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(size.width/2,size.height/2);
    
    
    //FLOOR NAVIGATION
        std::shared_ptr<Texture> floor_up   = _assets->get<Texture>("up");
        std::shared_ptr<Texture> floor_down = _assets->get<Texture>("down");
        _up_button = Button::alloc(PolygonNode::allocWithTexture(floor_up),
                                       PolygonNode::allocWithTexture(floor_up));
        _down_button = Button::alloc(PolygonNode::allocWithTexture(floor_down),
                                           PolygonNode::allocWithTexture(floor_down));
    
        // Create a callback function for the MOVE UP button
        _up_button->setName("lookout");
        _up_button->setListener([=] (const std::string& name, bool down) {
            // Only go to lookout when the button is released
            if (!down && currentCastleFloor<4 && !_actions->isActive(ACT_KEY)) {
                CULog("Move Up");
                CULog("curent floor");
                CULog("%d/n",currentCastleFloor);
                OverworldScene::doMove(_moveup, currentCastleFloor);
                CULog("fade out");
                CULog("%d/n",currentCastleFloor);
                OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
                CULog("fade in");
                CULog("%d/n",currentCastleFloor+1);
                OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor+1);
                currentCastleFloor += 1;
                CULog("current floor");
                CULog("%d/n",currentCastleFloor);
            }
        });
    
        // Create a callback function for the MOVE DOWN button
        _down_button->setName("lookout");
        _down_button->setListener([=] (const std::string& name, bool down) {
            // Only go to lookout when the button is released
            if (!down && currentCastleFloor>0  && !_actions->isActive(ACT_KEY)) {
                CULog("Move Down");
                CULog("curent floor");
                CULog("%d/n",currentCastleFloor);
                OverworldScene::doMove(_movedn, currentCastleFloor);
                CULog("fade out");
                CULog("%d/n",currentCastleFloor);
                OverworldScene::doFadeOut(_castleFadeOUT, currentCastleFloor);
                CULog("fade in");
                CULog("%d/n",currentCastleFloor-1);
                OverworldScene::doFadeIn(_castleFadeIN, currentCastleFloor-1);
                currentCastleFloor -= 1;
                CULog("current floor");
                CULog("%d/n",currentCastleFloor);
            }
        });
    
        _up_button->setScale(0.1f); // Magic number to rescale asset
        _up_button->setAnchor(Vec2::ANCHOR_CENTER);
        _up_button->setPosition(-80,50);
    
        _down_button->setScale(0.1f); // Magic number to rescale asset
        _down_button->setAnchor(Vec2::ANCHOR_CENTER);
        _down_button->setPosition(-80,-50);
    
        _background->addChild(_up_button);
        _background->addChild(_down_button);
    

    // Create the quit button.  A button has an up image and a down image
        std::shared_ptr<Texture> up   = _assets->get<Texture>("close-normal");
        std::shared_ptr<Texture> down = _assets->get<Texture>("close-selected");
    
        Size bsize = up->getSize();
        _quitButton = Button::alloc(PolygonNode::allocWithTexture(up),
                                PolygonNode::allocWithTexture(down));
    
            // Position the button in the bottom right corner
            _quitButton->setAnchor(Vec2::ANCHOR_CENTER);
            _quitButton->setPosition(size.width-bsize.width/2,bsize.height/2);
    
    
            // Create a callback function for the button
            _quitButton->setName("close");
            _quitButton->setListener([=] (const std::string& name, bool down) {
                // Only quit when the button is released
                if (!down) {
                    CULog("Goodbye!");
                    Application::get()->quit();
                }
            });
    
    
    // Create the reset button.  A button has an up image and a down image
    _resetButton = Button::alloc(PolygonNode::allocWithTexture(up),
                                 PolygonNode::allocWithTexture(down));
    
    // Position the button in the bottom right corner
    _resetButton->setAnchor(Vec2::ANCHOR_CENTER);
    _resetButton->setPosition(0,0);
    
    
    // Create a callback function for the button
    _resetButton->setName("close");
    _resetButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("reset!");
            OverworldScene::resetCastle();
        }
    });
    
    
    
    // Add the background to the scene graph
    addChild(_background);
    addChild(_quitButton);
    addChild(_resetButton);

    

    
    
    // We can only activate a button AFTER it is added to a scene
    _ballistaNorth->activate(2); _ballistaNorthEast->activate(3); _ballistaSouthEast->activate(4);
    _ballistaSouth->activate(5); _ballistaSouthWest->activate(6); _ballistaNorthWest->activate(7);
    _lookout_button->activate(8);
    _quitButton->activate(9);
    _up_button->activate(10);
    _down_button->activate(11);
    _resetButton->activate(12);
    
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
//        std::shared_ptr<cugl::PolygonNode> _curr_castle_floor;
//        _curr_castle_floor = OverworldScene::getTowerView(floor);
//        _actions->activate(ACT_KEY+1, _castleFadeOUT, _curr_castle_floor, fcn);
//        std::shared_ptr<cugl::PolygonNode> _new_castle_floor;
//        if (action==_movedn){
//            _new_castle_floor = OverworldScene::getTowerView(floor-1);
//        }
//        else {
//            _new_castle_floor = OverworldScene::getTowerView(floor+1);
//        }
////        if (!_new_castle_floor->isVisible()){
////            _new_castle_floor->setVisible(true);
////        }
//        _actions->activate(ACT_KEY+2, _castleFadeIN, _new_castle_floor, fcn);
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
    // Animate
    _actions->update(timestep);
}

void OverworldScene::setActive(bool active) {
    _active = active;
    switchscene = 0;
    if(active){
        _quitButton->activate(2);
        _ballistaNorth->activate(3);
        _lookout_button->activate(8);
        Application::get()->setClearColor(Color4(132,180,113,255));
    }
    else{
        _quitButton->deactivate();
        _ballistaNorth->deactivate();
        _lookout_button->deactivate();
    }
}




