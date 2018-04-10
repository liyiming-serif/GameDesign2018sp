//
//  OilScene.c
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "OilScene.h"
using namespace cugl;

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

#define BUTTON_SCALE 1.0f

bool OilScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }
    
    // Set background color
    Application::get()->setClearColor(Color4(132,180,113,255));
    
    switchscene = 0;
    
    _assets = assets;
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("oil_background");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    addChild(_background);
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    
    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    _oilTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    _oilTOcastle->setScale(.8f); // Magic number to rescale asset
    
    // Create a callback function for the OVERWORLD button
    _oilTOcastle->setName("oilTOcastle");
    _oilTOcastle->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
        }
    });
    
    
    // Position the overworld button in the bottom left
    _oilTOcastle->setAnchor(Vec2::ANCHOR_CENTER);
    _oilTOcastle->setPosition(100,80);
    
    // Add the logo and button to the scene graph
    addChild(_oilTOcastle);
    
    // We can only activate a button AFTER it is added to a scene
    _oilTOcastle->activate(input.generateKey("oilTOcastle"));
    
    
    std::shared_ptr<Texture> north_compass  = _assets->get<Texture>("N_compass");
    N_compass = PolygonNode::allocWithTexture(north_compass);
    N_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    N_compass->setAnchor(Vec2::ANCHOR_CENTER);
    N_compass->setPosition(900,80);
    addChild(N_compass);
    N_compass->setVisible(false);
    
    std::shared_ptr<Texture> northeast_compass  = _assets->get<Texture>("NE_compass");
    NE_compass = PolygonNode::allocWithTexture(northeast_compass);
    NE_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    NE_compass->setAnchor(Vec2::ANCHOR_CENTER);
    NE_compass->setPosition(900,80);
    addChild(NE_compass);
    NE_compass->setVisible(false);
    
    std::shared_ptr<Texture> northwest_compass  = _assets->get<Texture>("NW_compass");
    NW_compass = PolygonNode::allocWithTexture(northwest_compass);
    NW_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    NW_compass->setAnchor(Vec2::ANCHOR_CENTER);
    NW_compass->setPosition(900,80);
    addChild(NW_compass);
    NW_compass->setVisible(false);
    
    std::shared_ptr<Texture> south_compass  = _assets->get<Texture>("S_compass");
    S_compass = PolygonNode::allocWithTexture(south_compass);
    S_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    S_compass->setAnchor(Vec2::ANCHOR_CENTER);
    S_compass->setPosition(900,80);
    addChild(S_compass);
    S_compass->setVisible(false);
    
    std::shared_ptr<Texture> southeast_compass  = _assets->get<Texture>("SE_compass");
    SE_compass = PolygonNode::allocWithTexture(southeast_compass);
    SE_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    SE_compass->setAnchor(Vec2::ANCHOR_CENTER);
    SE_compass->setPosition(900,80);
    addChild(SE_compass);
    SE_compass->setVisible(false);
    
    std::shared_ptr<Texture> southwest_compass  = _assets->get<Texture>("SW_compass");
    SW_compass = PolygonNode::allocWithTexture(southwest_compass);
    SW_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    SW_compass->setAnchor(Vec2::ANCHOR_CENTER);
    SW_compass->setPosition(900,80);
    addChild(SW_compass);
    SW_compass->setVisible(false);
    
    return true;
}

void OilScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _oilTOcastle = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void OilScene::setCompass(int direction){
    N_compass->setVisible(false);
    NE_compass->setVisible(false);
    NW_compass->setVisible(false);
    S_compass->setVisible(false);
    SE_compass->setVisible(false);
    SW_compass->setVisible(false);
    if (direction == 0) {
        N_compass->setVisible(true);
    }
    else if (direction == 1){
        NW_compass->setVisible(true);
    }
    else if (direction == 2){
        SW_compass->setVisible(true);
    }
    else if (direction == 3){
        S_compass->setVisible(true);
    }
    else if (direction == 4){
        SE_compass->setVisible(true);
    }
    else if (direction == 5){
        NE_compass->setVisible(true);
    }
}

void OilScene::update(float timestep){
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
			if (j<gameModel._enemyArrayMaster[i].size()) {
				gameModel._enemyArrayMaster[i].erase(gameModel._enemyArrayMaster[i].begin() + gameModel._enemiesToFreeMaster[i][j]);
			}
		}
		gameModel._enemiesToFreeMaster[i].clear();
	}
}


//Pause or Resume
void OilScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _oilTOcastle->activate(input.findKey("oilTOcastle"));
    }
    else{
        _oilTOcastle->deactivate();
    }
}
