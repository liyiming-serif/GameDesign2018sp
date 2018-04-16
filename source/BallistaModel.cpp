//
// Created by Yiming on 3/26/2018.
//

#include "BallistaModel.h"

#define MIN_ANGLE M_PI*11.0f/12.0f //165 degrees
#define MAX_ANGLE M_PI/12.0f //15 degrees
#define TURN_SPEED M_PI/20.0f

#define MAX_POWER 18.0f
#define PULLBACK_SPEED 1.0f

#define NUM_ROWS 3
#define NUM_COLS 5

#define IDLE_FRAME_START 0
#define AIMING_FRAME_START 1
#define FIRING_FRAME_START 10

using namespace cugl;

/**Can't believe there isn't a standard C++ function for these helpers.*/
int sign(float number) {
	if (number > 0) {
		return 1;
	}
	else if (number < 0) {
		return -1;
	}
	return 0;
}

float angleDifference(float a1, float a2) {
	//normalized between -PI and PI
	float da = a2 - a1;
	if (da > M_PI) {
		da -= 2 * M_PI;
	}
	else if (da < -M_PI) {
		da += 2 * M_PI;
	}
	return da;
}

bool BallistaModel::init(Vec2 position, const std::shared_ptr<AssetManager>& assets) {
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
		unsigned int frame = _node->getFrame();
		//Manipulate animation frame
		if (_power > 0.0f) {
			//aiming
			int pullbackFrames = FIRING_FRAME_START - AIMING_FRAME_START;
			frame = floor(_power / MAX_POWER*pullbackFrames) + AIMING_FRAME_START;
		}
		else if (!isReadyToFire) {
			//recoiling from fire
			frame++;
			//finish recoiling
			if (frame >= _node->getSize()) {
				isReadyToFire = true;
				frame = 0;
			}
		}
		else {
			//idle
			frame = 0;
		}
		_node->setFrame(frame);
		_node->setAngle(_angle-M_PI/2.0f);
	}
}

/**All angles are in radians*/
void BallistaModel::setAngle(float angle) {
	//clamp angle so it can't fire backwards
	float minAngleDiff = angleDifference(MIN_ANGLE, angle);
	float maxAngleDiff = angleDifference(MAX_ANGLE, angle);
	if (minAngleDiff < 0 && maxAngleDiff > 0) {
		
	}
	else {
		angle = (abs(minAngleDiff)<abs(maxAngleDiff)) ? MIN_ANGLE : MAX_ANGLE;
	}

	//gradually rotate ballista
	float angleDiff = angleDifference(_angle, angle);
	float deltaAngle = std::fmin(TURN_SPEED, abs(angleDiff));
	_angle += sign(angleDiff)*deltaAngle;
}

void BallistaModel::setPower(float power, bool instantaneous) {
	//clamp power
	if (power > MAX_POWER) {
		power = MAX_POWER;
	}
	if (instantaneous) { //releasing the bow causes instant drop in power
		_power = power;
	}
	else { //drawing back the bow is gradual
		float deltaPower = std::fmin(PULLBACK_SPEED, abs(power - _power));
		_power += sign(power - _power)*deltaPower;
	}
}

void BallistaModel::dispose() {
	_node = nullptr;
}


