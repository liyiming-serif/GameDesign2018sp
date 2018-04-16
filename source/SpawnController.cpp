//
// Created by Josh on 3/26/2018.
//

#include "SpawnController.h"

using namespace cugl;

#define ENEMY_NAME_LEN 4

/**Helper function for generating random enemy keys
 * Source: Carl (stackoverflow)
 */
std::string genRandName(int len)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		return charset[rand() % (sizeof(charset) - 1)];
	};
	std::string str(len, 0);
	std::generate_n(str.begin(), len, randchar);
	return str;
}

/**
 * Creates a spawn controller
 */
bool SpawnController::init(const std::shared_ptr<AssetManager>& assets){
    //when this equals -1, stop spawning
    _active = true;
    _currSpawnIndex = 0;
    _totalTime = 0;

    //temporarily fill in the enemyArray, will read from JSON later
    //XCoord, YCoord, Type, Health, SpawnTime, Sector
    std::vector<float> enemy = {100.0, 500.0, 1.0, 1.0, 300.0, 0.0};
    _enemyArray.push_back(enemy);
    enemy = {500.0, 500.0, 1.0, 1.0, 315.0, 0.0};
    _enemyArray.push_back(enemy);
    enemy = {950.0, 500.0, 1.0, 1.0, 330.0, 0.0};
    _enemyArray.push_back(enemy);
    enemy = {300.0, 500.0, 1.0, 1.0, 600.0, 3.0};
    _enemyArray.push_back(enemy);
    enemy = {700.0, 500.0, 1.0, 1.0, 615.0, 3.0};
    _enemyArray.push_back(enemy);
    enemy = {400.0, 500.0, 1.0, 1.0, 1200.0, 0.0};
    _enemyArray.push_back(enemy);
    enemy = {650.0, 500.0, 1.0, 1.0, 1300.0, 1.0};
    _enemyArray.push_back(enemy);
    enemy = {200.0, 500.0, 1.0, 1.0, 1400.0, 2.0};
    _enemyArray.push_back(enemy);
    enemy = {700.0, 500.0, 1.0, 1.0, 1500.0, 3.0};
    _enemyArray.push_back(enemy);
    enemy = {350.0, 500.0, 1.0, 1.0, 1600.0, 4.0};
    _enemyArray.push_back(enemy);
    enemy = {800.0, 500.0, 1.0, 1.0, 1700.0, 5.0};
    _enemyArray.push_back(enemy);
    enemy = {200.0, 500.0, 1.0, 1.0, 2200.0, 2.0};
    _enemyArray.push_back(enemy);
    enemy = {550.0, 500.0, 1.0, 1.0, 2215.0, 2.0};
    _enemyArray.push_back(enemy);
    enemy = {800.0, 500.0, 1.0, 1.0, 2230.0, 2.0};
    _enemyArray.push_back(enemy);
    enemy = {400.0, 500.0, 1.0, 1.0, 2500.0, 5.0};
    _enemyArray.push_back(enemy);
    enemy = {800.0, 500.0, 1.0, 1.0, 2500.0, 5.0};
    _enemyArray.push_back(enemy);
    enemy = {900.0, 500.0, 1.0, 2.0, 3000.0, 1.0};
    _enemyArray.push_back(enemy);

    return true;
}

void SpawnController::update(float deltaTime) {
    _totalTime++;
    if(_currSpawnIndex >= 0){
        if(_enemyArray[_currSpawnIndex][4]<_totalTime){
			//we need to spawn one

			//keep generating ekey until we find a unique one
			std::string ekey;
			do {
				ekey = genRandName(ENEMY_NAME_LEN);
			} while (gameModel._enemyArrayMaster[(int)(_enemyArray[_currSpawnIndex][5])].find(ekey)
				!= gameModel._enemyArrayMaster[(int)(_enemyArray[_currSpawnIndex][5])].end());

			// Allocate a new EnemyDataModel in memory
			std::shared_ptr<EnemyDataModel> e = EnemyDataModel::alloc(ekey,_enemyArray[_currSpawnIndex][3],
				Vec2(_enemyArray[_currSpawnIndex][0], _enemyArray[_currSpawnIndex][1]),
				_enemyArray[_currSpawnIndex][2], _enemyArray[_currSpawnIndex][5]);

			if (e != nullptr) {
				gameModel._enemyArrayMaster[(int)(_enemyArray[_currSpawnIndex][5])][ekey] = e;
			}

            if(_currSpawnIndex != _enemyArray.size()-1){
            //still have more enemies to spawn
                _currSpawnIndex++;
            }
            else{
            //done spawning
                _currSpawnIndex = -1;
            }
        }
    }
}

void SpawnController::dispose(){
    _active = false;
    _enemyArray.clear();
}