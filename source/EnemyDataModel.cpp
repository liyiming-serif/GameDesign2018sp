//
// Created by Yiming on 4/17/2018.
//

#include "EnemyDataModel.h"

/*
* ENEMY TYPES:
* 1 Regular
* 2 Flying Ranged
*/

using namespace cugl;

bool EnemyDataModel::init(const std::string& name,int hp,const Vec2& pos,int type,int wall) {
	_name = name;
	_health = hp;
	_pos.set(pos);
	_type = type;
	_wall = wall;
	_atkCounter = -1;
	switch (type) {
		case 1: //skeleton
			_atkRange = -1;
			break;
		case 2: //flying ranged
			_atkRange = rand()%150+300;
			break;
		case 3: //lil reaper
			_atkRange = 8;
			break;
		case 4: //big reaper
			_atkRange = 8;
			break;
		case 5: //berserker
			_atkRange = 8;
			break;
		default:
			return false;
	}
	return true;
}

int EnemyDataModel::getSpeed() {
	if (_type == 1 || _type == 2 || _type == 3) {
		return 3;
	}
	else if (_type == 4) {
		return 2;
	}
	else if (_type == 5) {
		if (_health == 3) {
			return 3;
		}
		else if (_health == 2) {
			return 5;
		}
		else {
			return 8;
		}
	}
	return 0;
}

int EnemyDataModel::getDamage() {
	if (_type == 1) {
		return 10;
	}
	else if (_type == 2) {
		return 3;
	}
	else if (_type == 3 || _type == 5) {
		return 5;
	}
	else if (_type == 4) {
		return 20;
	}
	return 0;
}

int EnemyDataModel::getAtkSpeed() {
	if (_type == 2) {
		return 75;
	}
	else if (_type == 3) {
		return 45;
	}
	else if (_type == 4) {
		return 120;
	}
	else if (_type == 5) {
		if (_health == 3) {
			return 50;
		}
		else if (_health == 2) {
			return 40;
		}
		else {
			return 25;
		}
	}
	return 0;
}
