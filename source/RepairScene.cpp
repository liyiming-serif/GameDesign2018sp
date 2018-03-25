//
//  RepairScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 3/9/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "RepairScene.h"
#include <string>
#include "GameModel.h"

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

#define FLOOR_SCALE .54f
#define BUTTON_SCALE 1.255f

/** Define the time settings for animation */
#define DURATION .4f
#define DISTANCE 200
#define REPEATS  3
#define ACT_KEY  "current"

#define UP  _assets->get<Texture>("ammo_icon")

#define FONT    _assets->get<Font>("langdon")

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
        std::shared_ptr<Texture> image_up   = _assets->get<Texture>("repair_wall_button");
    


    std::shared_ptr<Texture> image_3   = _assets->get<Texture>("ammo_icon");
        _northWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
    
    //CULog("change image");
   // _northWallButton->setUp(PolygonNode::allocWithTexture(image_3));
    // _northWallButton = Button::init(const std::shared_ptr<Node> &up);
    //_northWallButton = Button::init((PolygonNode::allocWithTexture(image_up));
  //  _northWallButton = Button::alloc
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
                 CULog("N");
                _new_wall = "N";
                gameModel.changeWallHealth(0, 2);
                _northText->setText(std::to_string(gameModel.getWallHealth(0))+"%");
            }
        });
    
        _northeastWallButton->setName("fade in NE");
        _northeastWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                CULog("NE");
                _new_wall = "NE";
                gameModel.changeWallHealth(1, 2);
                _northeastText->setText(std::to_string(gameModel.getWallHealth(1))+"%");
            }
        });
    
    
        _southeastWallButton->setName("fade in SE");
        _southeastWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                CULog("SE");
                _new_wall = "SE";
                gameModel.changeWallHealth(2, 2);
                _southeastText->setText(std::to_string(gameModel.getWallHealth(2))+"%");

            }
        });
        _southWallButton->setName("fade in S");
        _southWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                CULog("S");
                _new_wall = "S";
                gameModel.changeWallHealth(3, 2);
                _southText->setText(std::to_string(gameModel.getWallHealth(3))+"%");
            }
        });
        _southwestWallButton->setName("fade in SW");
        _southwestWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                CULog("SW");
				_new_wall = "SW";
                gameModel.changeWallHealth(4, 2);
                _southwestText->setText(std::to_string(gameModel.getWallHealth(4))+"%");
            }
        });
        _northwestWallButton->setName("fade in NW");
        _northwestWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                CULog("NW");
				_new_wall = "NW";
                gameModel.changeWallHealth(5, 2);
                _northwestText->setText(std::to_string(gameModel.getWallHealth(5))+"%");
            }
        });
    
    
    
        //Positions the Catapult Buttons
        //Catapult Floor Center
        float centerX = _background->getContentSize().width/2;
        float centerY = _background->getContentSize().height/2;
        _northWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northWallButton->setPosition(centerX,centerY+.22*_background->getContentHeight());
        //_northeastWallButton->setPushable(<#const std::vector<Vec2> &vertices#>)
   // _northText->setFont(<#const std::shared_ptr<Font> &font#>)
//    std::vector<Vec2> vec = new std::vector<Vec2>(.5f,.5f);
//    _northeastWallButton->setPushable(std::vector<Vec2>);
    float array [8] = {0,158,90,0,220,0,309,153};
    
    //cugl::Poly2 myPushable = Poly2(array);
   // cugl::SimpleTriangulator obj = SimpleTriangulator({0,158,90,0,220,0,309,153});
   // obj.calculate();
 //   _northWallButton->setPushable(obj.getPolygon());
    
    
            _northText =Label::alloc((std::string) "                         ", FONT);
            _northWallButton->addChild(_northText);
            _northText->setAnchor(Vec2::ANCHOR_CENTER);
            _northText->setPosition(_northWallButton->getContentWidth()/2,_northWallButton->getContentHeight()/2);
            _northText->setForeground(cugl::Color4(233,225,212,255));
    
//        _winnode = Label::alloc(WIN_MESSAGE, _assets->get<Font>(MESSAGE_FONT));
//        _winnode->setAnchor(Vec2::ANCHOR_CENTER);
//        _winnode->setPosition(dimen.width/2.0f,dimen.height/2.0f);
//        _winnode->setForeground(WIN_COLOR);


    
        _northeastWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northeastWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northeastWallButton->setPosition(centerX+.11*_background->getContentWidth(),centerY+.11*_background->getContentHeight());
        _northeastWallButton->setAngle(-M_PI/3);
    
            _northeastText =Label::alloc( (std::string) "                         ", FONT);
            _northeastWallButton->addChild(_northeastText);
            _northeastText->setAnchor(Vec2::ANCHOR_CENTER);
            _northeastText->setPosition(_northeastWallButton->getContentWidth()/2,_northeastWallButton->getContentHeight()/2);
            _northeastText->setForeground(cugl::Color4(233,225,212,255));
    
        _southeastWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _southeastWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _southeastWallButton->setPosition(centerX+.11*_background->getContentWidth(),centerY-.111*_background->getContentHeight());
        _southeastWallButton->setAngle(-2*M_PI/3);
    
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
    
            _northwestText =Label::alloc((std::string) "                         ", FONT);
            _northwestWallButton->addChild(_northwestText);
            _northwestText->setAnchor(Vec2::ANCHOR_CENTER);
            _northwestText->setPosition(_northwestWallButton->getContentWidth()/2,_northwestWallButton->getContentHeight()/2);
            _northwestText->setForeground(cugl::Color4(233,225,212,255));
    
    
        _buttons = Node::alloc();
        _buttons->setScale(.5); // Magic number to rescale asset
        _buttons->setAnchor(Vec2::ANCHOR_CENTER);
        _buttons->setPosition(15,-4);
        addChild(_buttons);
    
    
        //Adds the buttons to the Scene Graph
        _buttons->addChild(_northWallButton);
        _buttons->addChild(_northeastWallButton);
        _buttons->addChild(_southeastWallButton);
        _buttons->addChild(_southWallButton);
        _buttons->addChild(_southwestWallButton);
        _buttons->addChild(_northwestWallButton);
        //_northWallButton = Button::alloc(PolygonNode::allocWithTexture(UP));
    
	_northWallButton->activate(44);
	_northeastWallButton->activate(211);
	_southeastWallButton->activate(311);
	_southWallButton->activate(411);
	_southwestWallButton->activate(420);
	_northwestWallButton->activate(69);
    
//    _northWallButton->setColor(Color4(255,255,255,0));
//    _northeastWallButton->setColor(Color4(255,255,255,0));
//    _southeastWallButton->setColor(Color4(255,255,255,0));
//    _southWallButton->setColor(Color4(255,255,255,0));
//    _southwestWallButton->setColor(Color4(255,255,255,0));
//    _northwestWallButton->setColor(Color4(255,255,255,0));
    
    

    
    
    
    // Create the back button.
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    

    _repairTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    _repairTOcastle->setScale(0.8f); // Magic number to rescale asset
    
    // Create a callback function for the back button
    _repairTOcastle->setName("repairTOcastle");
    _repairTOcastle->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("in repair");
            switchscene = OVERWORLD;
        }
    });
    
    
    // Position the overworld button in the bottom right
    //Size bsize = overworld_up->getSize();
    _repairTOcastle->setAnchor(Vec2::ANCHOR_CENTER);
    _repairTOcastle->setPosition(_size.width-60,60);
    
    // Add the logo and button to the scene graph
    addChild(_repairTOcastle);
    
    // We can only activate a button AFTER it is added to a scene
    _repairTOcastle->activate(30);
    
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

    

        _northText->setText(std::to_string(gameModel.getWallHealth(0))+"%");
        _northeastText->setText(std::to_string(gameModel.getWallHealth(1))+"%");
        _southeastText->setText(std::to_string(gameModel.getWallHealth(2))+"%");
        _southText->setText(std::to_string(gameModel.getWallHealth(3))+"%");
        _southwestText->setText(std::to_string(gameModel.getWallHealth(4))+"%");
        _northwestText->setText(std::to_string(gameModel.getWallHealth(5))+"%");
    
    std::shared_ptr<Texture> image   = _assets->get<Texture>("castle");


        _curr_wall_health = gameModel.getWallHealth(0);
        if (_curr_wall_health < 10){
            _northText->setPosition(190,80);
//            CULog("change image");
//             _northWallButton->setUp(PolygonNode::allocWithTexture(image));
    
        }
        else if (_curr_wall_health < 100){
            _northText->setPosition(170,80);
        }
        else {
            _northText->setPosition(150,80);
        }
    
    
    
        _curr_wall_health = gameModel.getWallHealth(1);
        if (_curr_wall_health < 10){
            _northeastText->setPosition(190,80);
        }
        else if (_curr_wall_health < 100){
            _northeastText->setPosition(170,80);
        }
        else {
            _northeastText->setPosition(150,80);
        }

    
        _curr_wall_health = gameModel.getWallHealth(2);
        if (_curr_wall_health < 10){
            _southeastText->setPosition(120,90);
        }
        else if (_curr_wall_health < 100){
            _southeastText->setPosition(140,90);
        }
        else {
            _southeastText->setPosition(160,90);
        }


        _curr_wall_health = gameModel.getWallHealth(3);
        if (_curr_wall_health < 10){
            _southText->setPosition(120,90);
        }
        else if (_curr_wall_health < 100){
            _southText->setPosition(140,90);
        }
        else {
            _southText->setPosition(160,90);
        }


        _curr_wall_health = gameModel.getWallHealth(4);
        if (_curr_wall_health < 10){
            _southwestText->setPosition(120,90);
        }
        else if (_curr_wall_health < 100){
            _southwestText->setPosition(140,90);
        }
        else {
            _southwestText->setPosition(160,90);
        }

        _curr_wall_health = gameModel.getWallHealth(5);
        if (_curr_wall_health < 10){
            _northwestText->setPosition(190,80);
        }
        else if (_curr_wall_health < 100){
            _northwestText->setPosition(170,80);
        }
        else {
            _northwestText->setPosition(150,80);
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
        _repairTOcastle->activate(50);
        _northWallButton->activate(44);
        _northeastWallButton->activate(211);
        _southeastWallButton->activate(311);
        _southWallButton->activate(411);
        _southwestWallButton->activate(420);
        _northwestWallButton->activate(69);
        CULog("Repair scene");
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
