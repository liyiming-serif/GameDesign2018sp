//
// Created by Yiming on 2/25/2018.
//

#include "BallistaScene.h"
#define LISTENER_KEY                1

using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024


bool BallistaScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    Size size = Application::get()->getDisplaySize();
    size *= GAME_WIDTH/size.width;

    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(size)) {
        return false;
    }

    // Set background color
    Application::get()->setClearColor(Color4(225,229,170,255));

    _assets = assets;

    // Get the image and attach it to a polygon obj. (no model yet)
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("ballista");
    _ballista = PolygonNode::allocWithTexture(texture);
    _ballista->setScale(0.9f); // Magic number to rescale asset
    _ballista->setAnchor(Vec2::ANCHOR_CENTER);
    _ballista->setPosition(size.width/2,size.height/2);

    addChild(_ballista);

    // Input controller
#ifdef CU_TOUCH_SCREEN
    Touchscreen* touch = Input::get<Touchscreen>();

    touch->addMotionListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, const Vec2& prev, bool focus) {
        this->touchDragCB(event,prev,focus);
    });
#endif

    return true;
}

void BallistaScene::dispose() {
    if (_active) {
        removeAllChildren();
        _active = false;
    }
}

void BallistaScene::update(float timestep){

}

void BallistaScene::touchDragCB(const TouchEvent& event, const Vec2& previous, bool focus) {
    Vec2* pointdir;
    Vec2::subtract(_ballista->getPosition(),event.position,pointdir);
    _ballista->setAngle(pointdir->getAngle());
}