//
//  RepairScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 3/9/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "RepairScene.h"

using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

#define BALLISTA    1
#define OVERWORLD   2
#define LOOKOUT     3

#define FLOOR_SCALE .55f
#define BUTTON_SCALE .8f

/** Define the time settings for animation */
#define DURATION .5f
#define DISTANCE 200
#define REPEATS  3
#define ACT_KEY  "current"

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
    
    // Allocate the manager and the actions
    _actions = ActionManager::alloc();
    
    _wallFadeIN = FadeIn::alloc(DURATION);
    _wallFadeOUT = FadeOut::alloc(DURATION);
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("repair_background");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(FLOOR_SCALE); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _background->setPosition(0,_size.height/2);
    addChild(_background);

    
    
    
    // Wall Layers
        std::shared_ptr<Texture> plainFloor_texture  = _assets->get<Texture>("repair_plain_wall");
        plain_floor = PolygonNode::allocWithTexture(plainFloor_texture);
        plain_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        plain_floor->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        plain_floor->setPosition(_size.width/80,_size.height/2);
        addChild(plain_floor);
    
        std::shared_ptr<Texture> northWall_texture  = _assets->get<Texture>("repair_north_wall");
        northWall_floor = PolygonNode::allocWithTexture(northWall_texture);
        northWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        northWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        northWall_floor->setPosition(_size.width/80,_size.height/2);
        northWall_floor->setColor(Color4(255,255,255,0));
        addChild(northWall_floor);
    
        std::shared_ptr<Texture> northeastWall_texture  = _assets->get<Texture>("repair_northeast_wall");
        northeastWall_floor = PolygonNode::allocWithTexture(northeastWall_texture);
        northeastWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        northeastWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        northeastWall_floor->setPosition(_size.width/80,_size.height/2);
        northeastWall_floor->setColor(Color4(255,255,255,0));
        addChild(northeastWall_floor);
    
        std::shared_ptr<Texture> southeastWall_texture  = _assets->get<Texture>("repair_southeast_wall");
        southeastWall_floor = PolygonNode::allocWithTexture(southeastWall_texture);
        southeastWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        southeastWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        southeastWall_floor->setPosition(_size.width/80,_size.height/2);
        southeastWall_floor->setColor(Color4(255,255,255,0));
        addChild(southeastWall_floor);
    
        std::shared_ptr<Texture> southWall_texture  = _assets->get<Texture>("repair_south_wall");
        southWall_floor = PolygonNode::allocWithTexture(southWall_texture);
        southWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        southWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        southWall_floor->setPosition(_size.width/80,_size.height/2);
        southWall_floor->setColor(Color4(255,255,255,0));
        addChild(southWall_floor);
    
        std::shared_ptr<Texture> southwestWall_texture  = _assets->get<Texture>("repair_southwest_wall");
        southwestWall_floor = PolygonNode::allocWithTexture(southwestWall_texture);
        southwestWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        southwestWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        southwestWall_floor->setPosition(_size.width/80,_size.height/2);
        southwestWall_floor->setColor(Color4(255,255,255,0));
        addChild(southwestWall_floor);
    
        std::shared_ptr<Texture> northwestWall_texture  = _assets->get<Texture>("repair_northwest_wall");
        northwestWall_floor = PolygonNode::allocWithTexture(northwestWall_texture);
        northwestWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        northwestWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        northwestWall_floor->setPosition(_size.width/80,_size.height/2);
        northwestWall_floor->setColor(Color4(255,255,255,0));
        addChild(northwestWall_floor);
    
    
    
    
    
    // Buttons
        std::shared_ptr<Texture> image_up   = _assets->get<Texture>("repair_wall_button");
        std::shared_ptr<Texture> image_down = _assets->get<Texture>("repair_wall_button");
        _northWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                       PolygonNode::allocWithTexture(image_down));
        _northeastWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                           PolygonNode::allocWithTexture(image_down));
        _southeastWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                      PolygonNode::allocWithTexture(image_down));
        _southWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                           PolygonNode::allocWithTexture(image_down));
        _southwestWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                       PolygonNode::allocWithTexture(image_down));
        _northwestWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up),
                                           PolygonNode::allocWithTexture(image_down));
    
    // Create a callback function for the Ballista buttons
        _northWallButton->setName("fade in N");
        _northWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                 CULog("N");
                RepairScene::doFadeIn(_wallFadeIN, "N");
                RepairScene::doFadeOut(_wallFadeOUT, _curr_wall);
                _curr_wall="N";
            }
        });
        _northeastWallButton->setName("fade in NE");
        _northeastWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                CULog("NE");
                RepairScene::doFadeIn(_wallFadeIN, "NE");
                RepairScene::doFadeOut(_wallFadeOUT, _curr_wall);
                _curr_wall="NE";
            }
        });
        _southeastWallButton->setName("fade in SE");
        _southeastWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                CULog("SE");
                RepairScene::doFadeIn(_wallFadeIN, "SE");
                RepairScene::doFadeOut(_wallFadeOUT, _curr_wall);
                _curr_wall="SE";
                
            }
        });
        _southWallButton->setName("fade in S");
        _southWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                CULog("S");
                RepairScene::doFadeIn(_wallFadeIN, "S");
                RepairScene::doFadeOut(_wallFadeOUT, _curr_wall);
                _curr_wall="S";
            }
        });
        _southwestWallButton->setName("fade in SW");
        _southwestWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                CULog("SW");
                RepairScene::doFadeIn(_wallFadeIN, "SW");
                RepairScene::doFadeOut(_wallFadeOUT, _curr_wall);
                _curr_wall="SW";
            }
        });
        _northwestWallButton->setName("fade in NW");
        _northwestWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down  && !_actions->isActive(ACT_KEY)) {
                CULog("NW");
                RepairScene::doFadeIn(_wallFadeIN, "NW");
                RepairScene::doFadeOut(_wallFadeOUT, _curr_wall);
                _curr_wall="NW";
            }
        });
    
    
    
        //Positions the Catapult Buttons
        //Catapult Floor Center
        float centerX = plain_floor->getContentSize().width/2;
        float centerY = plain_floor->getContentSize().height/2;
        _northWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northWallButton->setPosition(centerX,centerY+.37*plain_floor->getContentHeight());
    
        _northeastWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northeastWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northeastWallButton->setPosition(centerX+.27*plain_floor->getContentWidth(),centerY+.18*plain_floor->getContentHeight());
        _northeastWallButton->setAngle(-M_PI/3);
    
        _southeastWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _southeastWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _southeastWallButton->setPosition(centerX+.27*plain_floor->getContentWidth(),centerY-.18*plain_floor->getContentHeight());
        _southeastWallButton->setAngle(-2*M_PI/3);
    
        _southWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _southWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _southWallButton->setPosition(centerX,centerY-.37*plain_floor->getContentHeight());
        _southWallButton->setAngle(-3*M_PI/3);
    
        _southwestWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _southwestWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _southwestWallButton->setPosition(centerX-.28*plain_floor->getContentWidth(),centerY-.18*plain_floor->getContentHeight());
        _southwestWallButton->setAngle(-4*M_PI/3);
    
        _northwestWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northwestWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northwestWallButton->setPosition(centerX-.28*plain_floor->getContentWidth(),centerY+.18*plain_floor->getContentHeight());
        _northwestWallButton->setAngle(M_PI/3);
    
    
        _buttons = Node::alloc();
        _buttons->setScale(.67); // Magic number to rescale asset
        _buttons->setAnchor(Vec2::ANCHOR_CENTER);
        _buttons->setPosition(-_size.width/24,0);
        addChild(_buttons);
    
    
        //Adds the buttons to the Scene Graph
        _buttons->addChild(_northWallButton);
        _buttons->addChild(_northeastWallButton);
        _buttons->addChild(_southeastWallButton);
        _buttons->addChild(_southWallButton);
        _buttons->addChild(_southwestWallButton);
        _buttons->addChild(_northwestWallButton);
    
    
    _northWallButton->activate(1);
    _northeastWallButton->activate(2);
    _southeastWallButton->activate(3);
    _southWallButton->activate(4);
    _southwestWallButton->activate(5);
    _northwestWallButton->activate(6);
    
    _northWallButton->setColor(Color4(255,255,255,0));
    _northeastWallButton->setColor(Color4(255,255,255,0));
    _southeastWallButton->setColor(Color4(255,255,255,0));
    _southWallButton->setColor(Color4(255,255,255,0));
    _southwestWallButton->setColor(Color4(255,255,255,0));
    _northwestWallButton->setColor(Color4(255,255,255,0));
    
    

    
    
    
    // Create the OVERWORLD button.  A button has an up image and a down image
    std::shared_ptr<Texture> overworld_up   = _assets->get<Texture>("basement_floor");
    std::shared_ptr<Texture> overworld_down = _assets->get<Texture>("basement_floor");
    

    _overworld_button3 = Button::alloc(PolygonNode::allocWithTexture(overworld_up),
                                       PolygonNode::allocWithTexture(overworld_down));
    _overworld_button3->setScale(0.1f); // Magic number to rescale asset
    
    // Create a callback function for the OVERWORLD button
    _overworld_button3->setName("overworld3");
    _overworld_button3->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("in repair");
            switchscene = OVERWORLD;
        }
    });
    
    
    // Position the overworld button in the bottom right
    //Size bsize = overworld_up->getSize();
    _overworld_button3->setAnchor(Vec2::ANCHOR_CENTER);
    _overworld_button3->setPosition(_size.width-60,60);
    
    // Add the logo and button to the scene graph
    addChild(_overworld_button3);
    
    // We can only activate a button AFTER it is added to a scene
    _overworld_button3->activate(30);
    
    return true;
}


void RepairScene::doFadeIn(const std::shared_ptr<FadeIn>& action, std::string direction) {
    CULog("Do wall fade in");
    std::shared_ptr<cugl::PolygonNode> _curr_castle_wall;
    _curr_castle_wall = RepairScene::getWall(direction);
    auto fcn = EasingFunction::alloc(EasingFunction::Type::CUBIC_OUT);
    _actions->activate(ACT_KEY, action, _curr_castle_wall, fcn);
}

void RepairScene::doFadeOut(const std::shared_ptr<FadeOut>& action, std::string direction) {
    CULog("Do wall fade out");
    std::shared_ptr<cugl::PolygonNode> _curr_castle_wall;
    _curr_castle_wall = RepairScene::getWall(direction);
    auto fcn = EasingFunction::alloc(EasingFunction::Type::CUBIC_OUT);
    _actions->activate(ACT_KEY+1, action, _curr_castle_wall, fcn);
}

std::shared_ptr<cugl::PolygonNode> RepairScene::getWall(std::string direction) {
    if (direction=="N"){
        return northWall_floor;
    }
    else if (direction=="NE"){
        return northeastWall_floor;
    }
    else if (direction=="SE"){
        return southeastWall_floor;
    }
    else if (direction=="S"){
        return southWall_floor;
    }
    else if (direction=="SW"){
        return southwestWall_floor;
    }
    else if (direction=="NW"){
        return northwestWall_floor;
    }
    else {
        return plain_floor;
    }
}

void RepairScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _overworld_button3 = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void RepairScene::update(float timestep){
    // Animate
    _actions->update(timestep);
}



//Pause or Resume
void RepairScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        //Application::get()->setClearColor(Color4(132,180,113,255));
        Application::get()->setClearColor(Color4(0,0,0,255));
        _overworld_button3->activate(50);
        _northeastWallButton->activate(112);
        _northeastWallButton->activate(211);
        _southeastWallButton->activate(311);
        _southWallButton->activate(411);
        _southwestWallButton->activate(511);
        _northwestWallButton->activate(611);
        CULog("Repair scene");
    }
    else{
        _overworld_button3->deactivate();
        _northeastWallButton->deactivate();
        _northeastWallButton->deactivate();
        _southeastWallButton->deactivate();
        _southWallButton->deactivate();
        _southwestWallButton->deactivate();
        _northwestWallButton->deactivate();
    }
}
