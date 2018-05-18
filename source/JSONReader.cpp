//
// Created by Josh on 3/27/2018.
//

#include "JSONReader.h"

using namespace cugl;

/**
 * Creates a JSONReader
 */
JSONReader::JSONReader(void):Asset(){

}

bool JSONReader::preload(const std::string& file){
    _active = true;

    _reader = JsonReader::allocWithAsset(file);
    return preload(_reader->readJson());
}

bool JSONReader::preload(const std::shared_ptr<cugl::JsonValue>& json){
    if (json == nullptr) {
        CUAssertLog(false, "Failed to load level file");
        return false;
    }
    _json = json;
    return true;
}

void JSONReader::update(float deltaTime) {

}

std::vector<std::vector<float>> JSONReader::readJSON(int players, int level){
    _enemyArray.clear();
    if (_json == nullptr) {
        CUAssertLog(false, "Failed to load level file");
        return _enemyArray;
    }
    //parse number of players
    if(players==1){
        _modeJSON = _json->get("one");
    }
    else if(players==2){
        _modeJSON = _json->get("two");
    }
    else if(players==3){
        _modeJSON = _json->get("three");
    }
    else if(players==4){
        _modeJSON = _json->get("four");
    }
    else if(players==5){
        _modeJSON = _json->get("five");
    }
    else{
        //players==6
        _modeJSON = _json->get("six");
    }
    _levelJSON = _modeJSON->get("level" + std::to_string(level));

    //read the actual level

    gameModel.setAmmo(0, _levelJSON->getInt("ammo"));

    _roomsJSON = _levelJSON->get("rooms_unlocked");
    gameModel._unlockedRooms["lookout"] = _roomsJSON->getBool("lookout");
    gameModel._unlockedRooms["oil"] = _roomsJSON->getBool("oil");
    gameModel._unlockedRooms["mage"] = _roomsJSON->getBool("mage");
    gameModel._unlockedRooms["ballista"] = _roomsJSON->getBool("ballista");
    gameModel._unlockedRooms["repair"] = _roomsJSON->getBool("repair");
    gameModel._unlockedRooms["ammo"] = _roomsJSON->getBool("ammo");


    _enemiesJSON = _levelJSON->get("enemies");
    for(int i = 1; i<=_enemiesJSON->getInt("numSpawn"); i++){
        _singleEnemyJSON = _enemiesJSON->get("spawn" + std::to_string(i));
        if(i ==_enemiesJSON->getInt("numSpawn")){
            //parse final spawn time
            gameModel.setEndTime(_singleEnemyJSON->getFloat("spawn_time"));
            CULog("%f", _singleEnemyJSON->getFloat("spawn_time"));
        }
		if (_singleEnemyJSON != nullptr) {
			//get values and store this somewhere
			_enemy = {
				//XCoord
				_singleEnemyJSON->getFloat("xCoord"),
				//YCoord
				_singleEnemyJSON->getFloat("yCoord"),
				//Type
				getType(_singleEnemyJSON->getString("name")),
				//Health
				getHealth(_singleEnemyJSON->getString("name")),
				//SpawnTime
				_singleEnemyJSON->getFloat("spawn_time"),
				//Sector
				_singleEnemyJSON->getFloat("sector") };

			_enemyArray.push_back(_enemy);
		}
    }
    return _enemyArray;
}

float JSONReader::getHealth(std::string name){
    if(name=="skeleton"||name=="archer"){
        return 1.0;
    }
    else if(name=="warrior"){
        return 2.0;
    }
	else if (name == "reaper") {
		return 4.0;
	}
	else if(name=="berserker"){
        return 3.0;
    }
    else{
        return 0.0;
    }
}

float JSONReader::getType(std::string name){
    if(name=="skeleton"){
        return 1.0;
    }
    else if(name=="archer"){
        return 2.0;
    }
    else if(name=="warrior"){
        return 3.0;
    }
    else if(name=="reaper"){
        return 4.0;
    }
    else if(name=="berserker"){
        return 5.0;
    }
    else{
        return 0.0;
    }
}

void JSONReader::dispose(){
    _active = false;
}
