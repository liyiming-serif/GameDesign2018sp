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
    
    Application::get()->setClearColor(Color4(170,170,170,255));
    
    // The logo is actually an image+label.  We need a parent node
    _background = Node::alloc();
    
    // Get the image and add it to the node.
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("background");
    std::shared_ptr<PolygonNode> background = PolygonNode::allocWithTexture(texture);
    background->setScale(0.4f); // Magic number to rescale asset
    background->setAnchor(Vec2::ANCHOR_CENTER);
    background->setPosition(0,0);

    _background->addChild(background);
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(size.width/2,size.height/2);


    // Create a button.  A button has an up image and a down image
    std::shared_ptr<Texture> up   = _assets->get<Texture>("close-normal");
    std::shared_ptr<Texture> down = _assets->get<Texture>("close-selected");
    
    Size bsize = up->getSize();
    _button = Button::alloc(PolygonNode::allocWithTexture(up),
                            PolygonNode::allocWithTexture(down));
    
    // Create the BALLISTA button.  A button has an up image and a down image
    std::shared_ptr<Texture> ballista_up   = _assets->get<Texture>("ballista_icon");
    std::shared_ptr<Texture> ballista_down = _assets->get<Texture>("ballista_icon");
    
    Size ballista_b_size = ballista_up->getSize();
    _ballista_button = Button::alloc(PolygonNode::allocWithTexture(ballista_up),
                                     PolygonNode::allocWithTexture(ballista_down));
    _ballista_button->setScale(0.2f); // Magic number to rescale asset
    
    
    // Create the LOOKOUT button.  A button has an up image and a down image
    std::shared_ptr<Texture> lookout_up   = _assets->get<Texture>("lookout");
    std::shared_ptr<Texture> lookout_down = _assets->get<Texture>("lookout");
    
    //Size lookout_b_size = lookout_up->getSize();
    _lookout_button = Button::alloc(PolygonNode::allocWithTexture(lookout_up),
                                    PolygonNode::allocWithTexture(lookout_down));
    _lookout_button->setScale(0.2f); // Magic number to rescale asset
    
//    // Create a callback function for the button
    _button->setName("close");
    _button->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("Goodbye!");
            Application::get()->quit();
        }
    });
    
    
    // Create a callback function for the BALLISTA button
    _ballista_button->setName("ballista");
    _ballista_button->setListener([=] (const std::string& name, bool down) {
        // Only switch scenes when the button is released
        if (!down) {
            switchscene = BALLISTA;
        }
    });
    
    
    // Create a callback function for the LOOKOUT button
    _lookout_button->setName("lookout");
    _lookout_button->setListener([=] (const std::string& name, bool down) {
        // Only go to lookout when the button is released
        if (!down) {
            switchscene = LOOKOUT;
        }
    });
    
    // Position the button in the bottom right corner
    _button->setAnchor(Vec2::ANCHOR_CENTER);
    _button->setPosition(size.width-bsize.width/2,bsize.height/2);
    
    // Position the BALLISTA button on the background
    _ballista_button->setAnchor(Vec2::ANCHOR_CENTER);
    _ballista_button->setPosition(ballista_b_size.width/2+90,ballista_b_size.height/2+90);
    
    // Position the LOOKOUT button in the center
    _lookout_button->setAnchor(Vec2::ANCHOR_CENTER);
    _lookout_button->setPosition(size.width/2,size.height/2);
    
    // Add the logo and button to the scene graph

    addChild(_background);
    addChild(_button);
    
    addChild(_ballista_button);
    addChild(_lookout_button);
    
    
    // We can only activate a button AFTER it is added to a scene
    _button->activate(2);
    _ballista_button->activate(3);
    _lookout_button->activate(4);
    
    return true;
}

void OverworldScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void OverworldScene::update(float timestep){
    
}

void OverworldScene::setActive(bool active) {
    _active = active;
    switchscene = 0;
    if(active){
        _button->activate(2);
        _ballista_button->activate(3);
        _lookout_button->activate(4);
        Application::get()->setClearColor(Color4(170,170,170,255));
    }
    else{
        _button->deactivate();
        _ballista_button->deactivate();
        _lookout_button->deactivate();
    }
}




