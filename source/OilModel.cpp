// OilModel.cpp
// Animation controller for the oil.
//
// Created by Yiming on 4/18/2018.
//

#include "OilModel.h"

#define NUM_ROWS 6
#define NUM_COLS 4

#define POURING_FRAME_START 0
#define RESETTING_FRAME_START 6
#define REFILLING_FRAME_START 12

using namespace cugl;

bool OilModel::init(Vec2 position, const std::shared_ptr<AssetManager>& assets) {
	//create the scene node
	_node = nullptr;
	std::shared_ptr<Texture> texture = assets->get<Texture>("oil_vat");
	_node = AnimationNode::alloc(texture, NUM_ROWS, NUM_COLS);
	_node->setScale(0.55f);
	_node->setFrame(POURING_FRAME_START);
	_node->setAnchor(Vec2::ANCHOR_CENTER);
	_node->setPosition(position);
	return true;
}

void OilModel::update(float cooldown, float tilt) {
	//advance frame
	int end, start;
	float ratio;

	if (isReloading) {
		start = RESETTING_FRAME_START;
		end = REFILLING_FRAME_START;
		ratio = 1-tilt;
		if (tilt == 0.0f) {
			isReloading = false;
		}
	}
	else if(cooldown > 0.0f){
		start = REFILLING_FRAME_START;
		end = _node->getSize();
		ratio = 1-cooldown;
	}
	else {
		start = POURING_FRAME_START;
		end = RESETTING_FRAME_START;
		ratio = tilt;
	}

	int frame = floor(ratio*(end-start)) + start;
	_node->setFrame(frame%_node->getSize());
}

void OilModel::dispose() {
	_node = nullptr;
}