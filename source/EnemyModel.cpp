//
// Created by Josh on 3/14/2018.
//

#include "EnemyModel.h"

#define BASE_SPEED 5

/*
* ENEMY TYPES:
* 1 Regular
*/

using namespace cugl;

bool EnemyModel::init(Vec2 pos, float dir, int type, int drawScale, const std::shared_ptr<AssetManager>& assets){

	_drawScale = drawScale;

	//regular enemies, type 1
    if(type == 1){
    //create the scene node
    _node = nullptr;
    std::shared_ptr<Texture> texture  = assets->get<Texture>("skeleton");
    _node = PolygonNode::allocWithTexture(texture);
    _node->setAnchor(Vec2::ANCHOR_CENTER);

    //initialize the box2d obstacle
    BoxObstacle::init(pos/_drawScale, Size(_node->getWidth()/_drawScale, _node->getHeight()/_drawScale));
    setAngle(dir);
    setLinearVelocity(cos(dir)*BASE_SPEED,sin(dir)*BASE_SPEED);
    }
    return true;
}

void EnemyModel::update(float deltaTime) {
    Obstacle::update(deltaTime);
    if (_node != nullptr) {
        _node->setPosition(getPosition()*_drawScale);
        _node->setAngle(getAngle());
    }
}

void EnemyModel::dispose(){
    CULog("enemy destroyed");
    _node = nullptr;
}
