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
	static std::shared_ptr<EnemyDataModel> alloc(const std::string& name, int health, const cugl::Vec2& pos, int type, int wall) {
		std::shared_ptr<EnemyDataModel> ref = std::make_shared<EnemyDataModel>();
		return (ref->init(name, health, pos, type, wall) ? ref : nullptr);
	}

	bool init(const std::string& name, int health, const cugl::Vec2& pos, int type, int wall) {
		_name = name;
		_health = health;
		_pos.set(pos);
		_type = type;
		_wall = wall;
		return true;
	}
	

	//DESTRUCTORS
	void dispose() {
		_name = "";
	}

	~EnemyDataModel() { dispose(); }


	//GETTERS AND SETTERS
	const std::string& getName() const { return _name; }

	int getHealth() const { return _health; }
	void setHealth(int health) { _health = health; }

	const cugl::Vec2& getPos() const { return _pos; }
	void setPos(const cugl::Vec2& pos) { _pos.set(pos); }

	int getType() const { return _type; }

	int getWall() const { return _wall; }

    //health only matters for berserker
	int getSpeed(int type, int health){
	    if(type==1||type==2||type==3){
            return 3;
	    }
	    else if(type==4){
	        return 2;
	    }
	    else if(type==5){
	        if(health==3){
	            return 1;
	        }
	        else if(health==2){
	            return 3;
	        }
	        else{
	            return 6;
	        }
	    }
	    else{
	        return 0;
	    }
	}

	int getDamage(int type){
	    if(type==1){
	        return 10;
	    }
	    else if(type==2){
	        return 3;
	    }
	    else if(type==3||type==5){
	        return 5;
	    }
	    else if(type==4){
	        return 20;
	    }
	}

	const std::string& toString() const {
		return _name +":"+std::to_string(_health)+":"+std::to_string(_pos.x)+":"+std::to_string(_pos.y)+":"
			+std::to_string(_type)+":"+std::to_string(_wall);
	}
};

#endif //BUILD_ANDROID_ENEMYDATAMODEL_H
