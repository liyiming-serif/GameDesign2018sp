//
// Created by Yiming on 3/26/2018.
//

#include "BallistaModel.h"

#define MIN_ANGLE 0
#define MAX_ANGLE 1
#define TURN_SPEED 2

#define MAX_POWER 12
#define PULLBACK_SPEED 2

#define NUM_ROWS 1
#define NUM_COLS 12

#define IDLE_FRAME_START 0
#define AIMING_FRAME_START 1
#define FIRING_FRAME_START 7

using namespace cugl;

bool BallistaModel::init(cugl::Vec2 position, const std::shared_ptr<AssetManager>& assets) {
	//create the scene node
	_node = nullptr;
	std::shared_ptr<Texture> texture = assets->get<Texture>("ballista");
	_node = AnimationNode::alloc(texture, NUM_ROWS, NUM_COLS);
	_node->setFrame(IDLE_FRAME_START);
	_node->setAnchor(Vec2::ANCHOR_CENTER);
	_node->setPosition(position);
	return true;
}

void BallistaModel::update(float deltaTime) {
	if (_node != nullptr) {
		unsigned int frame = _node->getFrame;
		//Manipulate animation frame
		if (_power > 0.0f) {
			//aiming
		}
		else if (!isReadyToFire) {
			//recoiling from fire
			frame++;
			//finish recoiling
			isReadyToFire = true;
		}
		else {
			//idle
			frame = 0;
		}
		_node->setFrame(frame);
		_node->setAngle(getAngle());
	}
}

/**All angles are in radians*/
void BallistaModel::setAngle(float angle) {
	
}

void BallistaModel::setPower(float power) {

}

void BallistaModel::dispose() {
	CULog("Ballista destroyed!");
	_node = nullptr;
}
