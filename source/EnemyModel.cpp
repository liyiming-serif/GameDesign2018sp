//
// Created by Josh on 3/14/2018.
//

#include "EnemyModel.h"

/*
* ENEMY TYPES:
* 1 Regular
* 2 Flying Ranged
*/

using namespace cugl;

bool EnemyModel::init(std::string name,Vec2 pos,int type,int drawScale,
	const std::shared_ptr<AssetManager>&assets){

	_drawScale = drawScale;
	_name = name;
	_node = nullptr;
	std::shared_ptr<Texture> texture;

	switch (type) {
		case 1: //regular enemies
			//set animation constants
			_rows = 4;
			_cols = 5;
			_walkFrameStart = 0;
			_dieFrameStart = 10;

			//create the scene node
			texture = assets->get<Texture>("skeleton");
			_node = AnimationNode::alloc(texture, _rows, _cols);
			_node->setFrame(_walkFrameStart);
			_node->setScale(0.25);
			_node->setAnchor(Vec2::ANCHOR_CENTER);
			break;

		case 2: //flying, ranged enemies
			//set animation constants
			_rows = 8;
			_cols = 5;
			_offset = 39;
			_walkFrameStart = 0;
			_dieFrameStart = 10;
			_attackFrameStart = 10;

			//create the scene node
			texture = assets->get<Texture>("flying");
			_node = AnimationNode::alloc(texture, _rows, _cols, _offset);
			_node->setFrame(_walkFrameStart);
			_node->setScale(0.2);
			_node->setAnchor(Vec2::ANCHOR_CENTER);
			break;

		case 3: //warriors/small reapers, continuous melee damage
			//set animation constants
			_rows = 6;
			_cols = 7;
			_offset = 41;
			_walkFrameStart = 0;
			_dieFrameStart = 16;

			//create the scene node
			texture = assets->get<Texture>("warrior");
			_node = AnimationNode::alloc(texture, _rows, _cols, _offset);
			_node->setFrame(_walkFrameStart);
			_node->setScale(0.4);
			_node->setAnchor(Vec2::ANCHOR_CENTER);
			break;

		default: //unrecognized enemy type
			return false;
	}

	_node->setPosition(pos);

	//initialize the box2d obstacle
	BoxObstacle::init(pos / _drawScale, Size(_node->getWidth() / _drawScale, _node->getHeight() / _drawScale));
    return true;
}

void EnemyModel::update(float deltaTime) {
	//Physics
    setLinearVelocity(0,0);
    Obstacle::update(deltaTime);

	//Drawing
    if (_node != nullptr) {
        _node->setPosition(getPosition()*_drawScale);
        _node->setAngle(getAngle());

		//Animation
		unsigned int frame = _node->getFrame();
		frame++;
		_node->setFrame((frame)%_dieFrameStart);
    }
}

void EnemyModel::dispose(){
    _node = nullptr;
}