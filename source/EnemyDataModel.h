//
// EnemyDataModel.h
//
// EnemyDataModel contains PURE DATA for a single enemy, which is stored as a
// value in GameModel.
//
// Created by Yiming on 4/11/2018.
//

#ifndef BUILD_ANDROID_ENEMYDATAMODEL_H
#define BUILD_ANDROID_ENEMYDATAMODEL_H

#include <cugl/cugl.h>

class EnemyDataModel {
protected:
	std::string _name; //random string to fetch this enemy from map
	int _health;
	cugl::Vec2 _pos;
	int _type;
	int _wall; //duplicate information to quickly construct enemy array from network

public:
	//CONSTRUCTORS
	EnemyDataModel(void) {}
	//TODO: read health and movement from an EnemyTypes.json
	static std::shared_ptr<EnemyDataModel> alloc(std::string name, int health, cugl::Vec2 pos, int type, int wall) {
		std::shared_ptr<EnemyDataModel> ref = std::make_shared<EnemyDataModel>();
		return (ref->init(name, health, pos, type, wall) ? ref : nullptr);
	}

	bool init(std::string name, int health, cugl::Vec2 pos, int type, int wall) {
		_name = name;
		_health = health;
		_pos.set(pos);
		_type = type;
		_wall = wall;
	}
	

	//DESTRUCTORS
	void dispose() {
		_name = "";
	}

	~EnemyDataModel() { dispose(); }


	//GETTERS AND SETTERS
	std::string getName() { return _name; }

	int getHealth() { return _health; }
	void setHealth(int health) { _health = health; }

	cugl::Vec2 getPos() { return _pos; }

	int getType() { return _type; }

	int getWall() { return _wall; }

	std::string toString() {
		return _name +":"+std::to_string(_health)+":"+std::to_string(_pos.x)+":"+std::to_string(_pos.y)+":"
			+std::to_string(_type)+":"+std::to_string(_wall);
	}
};

#endif //BUILD_ANDROID_ENEMYDATAMODEL_H
