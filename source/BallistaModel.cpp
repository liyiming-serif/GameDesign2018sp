//
// Created by Yiming on 3/26/2018.
//

#include "BallistaModel.h"

#define MIN_ANGLE M_PI*11.0f/12.0f //165 degrees
#define MAX_ANGLE M_PI/12.0f //15 degrees
#define TURN_SPEED M_PI/20.0f

#define MAX_POWER 24.0f
#define PULLBACK_SPEED 3.0f

#define NUM_ROWS 7
#define NUM_COLS 5

#define IDLE_FRAME_START 0
#define AIMING_FRAME_START 10
#define FIRING_FRAME_START 20
#define RECOIL_SPEED 0.5f

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
	_node->setScale(0.4);
	_node->setFrame(IDLE_FRAME_START);
	_node->setAnchor(Vec2::ANCHOR_CENTER);
	_node->setPosition(position);
	return true;
}

void BallistaModel::update(float deltaTime, bool hasAmmo) {
	if (_node != nullptr) {
		unsigned int frame = _node->getFrame();
		//Manipulate animation frame
		if (_power > 0.0f) {
			if (hasAmmo) {
				//aiming WITH arrow loaded
				int pullbackFrames = FIRING_FRAME_START - AIMING_FRAME_START - 1;
				frame = floor(_power / MAX_POWER*pullbackFrames) + AIMING_FRAME_START;
			}
			else {
				//aiming WITHOUT arrow loaded
				int pullbackFrames = AIMING_FRAME_START - IDLE_FRAME_START - 1;
				frame = floor(_power / MAX_POWER*pullbackFrames);
			}
		}
		else if (!isReadyToFire) {
			if (frame < FIRING_FRAME_START) {
				//just fired
				frame = FIRING_FRAME_START;
				_interFrame = frame;
			}
			else {
				//recoiling from fire
				_interFrame += RECOIL_SPEED;
				frame = floor(_interFrame);
			}
			//finish recoiling
			if (frame >= _node->getSize()) {
				isReadyToFire = true;
				frame = IDLE_FRAME_START;
			}
		}
		else {
			//idle
			frame = IDLE_FRAME_START;
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


