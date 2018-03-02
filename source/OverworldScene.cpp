//
//  OverworldScene.cpp
//  HelloWorld
//
//  Created by Noah Sterling on 3/1/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "OverworldScene.h"



#define LISTENER_KEY  1

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
    
    //    // Get the font and make a label for the logo
    //    std::shared_ptr<Font> font = _assets->get<Font>("charlemagne");
    //    std::shared_ptr<Label> label = Label::alloc("CUGL",font);
    //    label->setAnchor(Vec2::ANCHOR_TOP_CENTER);
    //    label->setPosition(15,-15); // Magic numbers for some manual kerning
    //    _logo->addChild(label);
    
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(size.width/2,size.height/2);

    
    
    // Create a button.  A button has an up image and a down image
    std::shared_ptr<Texture> up   = _assets->get<Texture>("close-normal");
    std::shared_ptr<Texture> down = _assets->get<Texture>("close-selected");
    
    Size bsize = up->getSize();
    std::shared_ptr<Button> button = Button::alloc(PolygonNode::allocWithTexture(up),
                                                   PolygonNode::allocWithTexture(down));
    
    // Create the BALLISTA button.  A button has an up image and a down image
    std::shared_ptr<Texture> ballista_up   = _assets->get<Texture>("ballista_icon");
    std::shared_ptr<Texture> ballista_down = _assets->get<Texture>("ballista_icon");
    
    Size ballista_b_size = ballista_up->getSize();
    std::shared_ptr<Button> ballista_button = Button::alloc(PolygonNode::allocWithTexture(ballista_up),
                                                            PolygonNode::allocWithTexture(ballista_down));
    ballista_button->setScale(0.2f); // Magic number to rescale asset
    
    
    // Create the LOOKOUT button.  A button has an up image and a down image
    std::shared_ptr<Texture> lookout_up   = _assets->get<Texture>("lookout");
    std::shared_ptr<Texture> lookout_down = _assets->get<Texture>("lookout");
    
    //Size lookout_b_size = lookout_up->getSize();
    std::shared_ptr<Button> lookout_button = Button::alloc(PolygonNode::allocWithTexture(lookout_up),
                                                           PolygonNode::allocWithTexture(lookout_down));
    lookout_button->setScale(0.2f); // Magic number to rescale asset
    
//    // Create a callback function for the button
    button->setName("close");
    button->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("Goodbye!");
            Application::get()->quit();
        }
    });
    
    
//    // Create a callback function for the BALLISTA button
//    ballista_button->setName("close");
//    ballista_button->setListener([=] (const std::string& name, bool down) {
//        // Only quit when the button is released
//        if (!down) {
//            CULog("Ballista");
//            (HelloApp) Application::get()->_ballistaScene;
//        }
//    });
    
    
//    // Create a callback function for the LOOKOUT button
//    lookout_button->setName("close");
//    lookout_button->setListener([=] (const std::string& name, bool down) {
//        // Only quit when the button is released
//        if (!down) {
//            CULog("Goodbye!");
//            this->quit();
//        }
//    });
    
    // Position the button in the bottom right corner
    button->setAnchor(Vec2::ANCHOR_CENTER);
    button->setPosition(size.width-bsize.width/2,bsize.height/2);
    
    // Position the BALLISTA button on the background
    ballista_button->setAnchor(Vec2::ANCHOR_CENTER);
    ballista_button->setPosition(ballista_b_size.width/2+90,ballista_b_size.height/2+90);
    
    // Position the LOOKOUT button in the center
    lookout_button->setAnchor(Vec2::ANCHOR_CENTER);
    lookout_button->setPosition(size.width/2,size.height/2);
    
    // Add the logo and button to the scene graph

    addChild(_background);
    addChild(button);
    
    addChild(ballista_button);
    addChild(lookout_button);
    
    
    // We can only activate a button AFTER it is added to a scene
    button->activate(1);
    ballista_button->activate(2);
    lookout_button->activate(3);
    
    return true;
}

void OverworldScene::dispose() {
    if (_active) {
        removeAllChildren();
        _active = false;
    }
}

void OverworldScene::update(float timestep){
    
}




