//
// Created by Yiming on 3/2/2018.
//

#include "LookoutScene.h"
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

bool LookoutScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("lookout_view");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(0,0);
    addChild(_background);

    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);

    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    _lookoutTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    _lookoutTOcastle->setScale(.8f); // Magic number to rescale asset

    // Create a callback function for the OVERWORLD button
    _lookoutTOcastle->setName("lookoutTOcastle");
    _lookoutTOcastle->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            CULog("at lookout");
            switchscene = OVERWORLD;
        }
    });


    // Position the overworld button in the bottom left
    _lookoutTOcastle->setAnchor(Vec2::ANCHOR_CENTER);
    _lookoutTOcastle->setPosition(100,80);

    // Add the logo and button to the scene graph
    addChild(_lookoutTOcastle);

    // We can only activate a button AFTER it is added to a scene
    _lookoutTOcastle->activate(input.generateKey("lookoutTOcastle));

    return true;
}

void LookoutScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _lookoutTOcastle = nullptr;
        _background = nullptr;
        _active = false;
    }
}

void LookoutScene::update(float timestep){

}

//void touchReleaseCB(const cugl::TouchEvent& event, bool focus){
//
//};

//Pause or Resume
void LookoutScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _overworld_button2->activate(input.findKey("lookoutToOverworld"));
        _lookoutTOcastle->activate(input.findKey("lookoutTOCastle"));
		for (auto it = gameModel._enemyArrayGroundN.begin(); it != gameModel._enemyArrayGroundN.end(); it++) {
			std::shared_ptr<EnemyModel> e = *it;
			addChild(e->getIcon());
		}
    }
    else{
        _lookoutTOcastle->deactivate();
		for (auto it = gameModel._enemyArrayGroundN.begin(); it != gameModel._enemyArrayGroundN.end(); it++) {
			std::shared_ptr<EnemyModel> e = *it;
			removeChild(e->getIcon());
		}
    }
}
