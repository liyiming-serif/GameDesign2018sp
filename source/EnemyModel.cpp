//
// Created by Josh on 3/14/2018.
//

#include "EnemyModel.h"

#define BASE_SPEED 0

#define NUM_ROWS 4
#define NUM_COLS 5

#define WALK_FRAME_START 0
#define DYING_FRAME_START 10

/*
* ENEMY TYPES:
* 1 Regular
*/

using namespace cugl;

bool EnemyModel::init(std::string name,Vec2 pos,float dir,float type,int drawScale,
	const std::shared_ptr<AssetManager>&assets){

	_dir = dir;
	_drawScale = drawScale;
	_name = name;

	//regular enemies, type 1
    if(type == 1){
		//create the scene node
		_node = nullptr;
		std::shared_ptr<Texture> texture  = assets->get<Texture>("skeleton");
		_node = AnimationNode::alloc(texture, NUM_ROWS, NUM_COLS);
		_node->setFrame(WALK_FRAME_START);
		_node->setScale(0.3);
		_node->setAnchor(Vec2::ANCHOR_CENTER);
		_node->setPosition(pos);

		//initialize the box2d obstacle
		BoxObstacle::init(pos/_drawScale, Size(_node->getWidth()/_drawScale, _node->getHeight()/_drawScale));
		setAngle(0);
		setLinearVelocity(cos(dir)*BASE_SPEED,sin(dir)*BASE_SPEED);
    }
    return true;
}

void EnemyModel::update(float deltaTime) {
	//Physics
    setLinearVelocity(cos(_dir)*BASE_SPEED,sin(_dir)*BASE_SPEED);
    Obstacle::update(deltaTime);

	//Drawing
    if (_node != nullptr) {
        _node->setPosition(getPosition()*_drawScale);
        _node->setAngle(getAngle());

		//Animation
		unsigned int frame = _node->getFrame();
		_node->setFrame((frame++)%DYING_FRAME_START);
    }
}

void EnemyModel::dispose(){
    _node = nullptr;
}

int EnemyModel::getDamage(int type){
    if(type == 1){
        return 9;
    }
    else return 0;
}