//
// Created by Josh on 3/14/2018.
//

#include "EnemyModel.h"

#define ANIMATION_SP 0.2f

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
	_atkProgress = -1;
	isDying = false;
	doneDying = false;
	std::shared_ptr<Texture> texture;

	switch (type) {
		case 1: //regular enemies
			//set animation constants
			_rows = 4;
			_cols = 5;
			_walkFrameStart = 0;
			_dieFrameStart = 10;
			_attackFrameStart = 20;
			_walkAnimSp = 0.15f;
			_dieAnimSp = 0.3f;

			//create the scene node
			texture = assets->get<Texture>("skeleton");
			_node = AnimationNode::alloc(texture, _rows, _cols);
			_node->setFrame(_walkFrameStart);
			_node->setScale(0.28);
			_node->setAnchor(Vec2::ANCHOR_CENTER);
			break;

		case 2: //flying, ranged enemies
			//set animation constants
			_rows = 8;
			_cols = 5;
			_offset = 39;
			_walkFrameStart = 0;
			_dieFrameStart = 10;
			_attackFrameStart = 20;
			_walkAnimSp = 0.2f;
			_dieAnimSp = 0.4f;

			//create the scene node
			texture = assets->get<Texture>("flying");
			_node = AnimationNode::alloc(texture, _rows, _cols, _offset);
			_node->setFrame(_walkFrameStart);
			_node->setScale(0.7);
			_node->setAnchor(Vec2::ANCHOR_CENTER);
			break;

		case 3: //warriors/small reapers, continuous melee damage
			//set animation constants
			_rows = 11;
			_cols = 4;
			_walkFrameStart = 0;
			_dieFrameStart = 10;
			_attackFrameStart = 24;
			_walkAnimSp = 0.08f;
			_dieAnimSp = 0.25f;

			//create the scene node
			texture = assets->get<Texture>("warrior");
			_node = AnimationNode::alloc(texture, _rows, _cols);
			_node->setFrame(_walkFrameStart);
			_node->setScale(0.8);
			_node->setAnchor(Vec2::ANCHOR_CENTER);
			break;

		case 4: //big reapers, slow and heavy melee
			//set animation constants
			_rows = 9;
			_cols = 4;
			_walkFrameStart = 0;
			_dieFrameStart = 8;
			_attackFrameStart = 16;
			_walkAnimSp = 0.05f;
			_dieAnimSp = 0.15f;

			//create the scene node
			texture = assets->get<Texture>("reaper");
			_node = AnimationNode::alloc(texture, _rows, _cols);
			_node->setFrame(_walkFrameStart);
			_node->setScale(1.2);
			_node->setAnchor(Vec2::ANCHOR_CENTER);
			break;
		case 5: //berserkers, stronger when attacked
			//set animation constants
			_rows = 11;
			_cols = 4;
			_walkFrameStart = 0;
			_dieFrameStart = 10;
			_attackFrameStart = 24;
			_walkAnimSp = 0.08f;
			_dieAnimSp = 0.25f;

			//create the scene node
			texture = assets->get<Texture>("berserker");
			_node = AnimationNode::alloc(texture, _rows, _cols);
			_node->setFrame(_walkFrameStart);
			_node->setScale(0.8);
			_node->setAnchor(Vec2::ANCHOR_CENTER);
			break;

		default: //unrecognized enemy type
			return false;
	}
	_currFrame = 0.0f;
	_node->setPosition(pos);
	_node->setZOrder(-1);

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

		//Animate Dying
		if (isDying) {
			if (_currFrame < _dieFrameStart) {
				_currFrame = _dieFrameStart;
			}
			else {
				_currFrame += _dieAnimSp;
				if (_currFrame >= _node->getSize() || _currFrame >= _attackFrameStart) {
					doneDying = true;
					return;
				}
			}
			_node->setFrame(static_cast<int>(floor(_currFrame)));
			return;
		}

		//Animate Attacking
		if (_atkProgress >= 0.0f && _atkProgress <= 1.0f) {
			int atkLen = _node->getSize() - _attackFrameStart;
			_node->setFrame(static_cast<int>(floor((1.0f-_atkProgress)*atkLen))%atkLen + _attackFrameStart);
			return;
		}

		//Animate Walking
		_currFrame += _walkAnimSp;
		_currFrame = std::fmodf(_currFrame,(float)_dieFrameStart);
		_node->setFrame(static_cast<int>(floor(_currFrame)));
		
    }
}

/**For Berserkers: multiply walking speed as they become more enraged.
 * Return the new walking animation speed.
 */
float EnemyModel::scaleAnimSp(float factor) {
	_walkAnimSp *= factor;
	return _walkAnimSp;
}

void EnemyModel::dispose(){
    _node = nullptr;
}
