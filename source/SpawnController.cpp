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
bool SpawnController::init(const std::shared_ptr<AssetManager>& assets, std::vector<std::vector<float>> initialEnemies){
    //when this equals -1, stop spawning
    _active = true;
    _currSpawnIndex = 0;
    _totalTime = 0;

    _enemyArray = initialEnemies;

    return true;
}

void SpawnController::update(float deltaTime) {
    _totalTime++;
	if (_totalTime%10 == 0) {
		//CULog("Total time in Spawn Controller %d", _totalTime);
	}
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
			//CULog("Current Spawn Index: %d", _currSpawnIndex);
        }
    }
}

void SpawnController::dispose(){
    _active = false;
    _enemyArray.clear();
}