//
// Created by Yiming on 3/9/2018.
//

#include "ArrowModel.h"

using namespace cugl;

bool ArrowModel::init(Vec2 pos, float speed, float dir, int drawScale, const std::shared_ptr<AssetManager>& assets){
	collided = false;
	//_drawScale is pixels per meter (meter is for box2d)
	_drawScale = drawScale;
    //create the scene node
    _node = nullptr;
    std::shared_ptr<Texture> texture  = assets->get<Texture>("arrow");
    _node = PolygonNode::allocWithTexture(texture);
	_node->setScale(0.35);
    _node->setAnchor(Vec2::ANCHOR_CENTER);

    //initialize the box2d obstacle
    CapsuleObstacle::init(pos/_drawScale, Size(_node->getWidth()/_drawScale, _node->getHeight()/_drawScale));
    setAngle(dir);
    setLinearVelocity(cos(dir)*speed,sin(dir)*speed);
    return true;
}

void ArrowModel::update(float deltaTime) {
    Obstacle::update(deltaTime);
    if (_node != nullptr) {
        _node->setPosition(getPosition()*_drawScale);
        _node->setAngle(getAngle());
    }
}

void ArrowModel::dispose(){
    _node = nullptr;
}
