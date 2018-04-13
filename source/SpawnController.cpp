//
// Created by Josh on 3/26/2018.
//

#include "SpawnController.h"

using namespace cugl;

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
    if(_currSpawnIndex >= 0){
        if(_enemyArray[_currSpawnIndex][4]<_totalTime){
        //we need to spawn one
            std::vector<float> enemy = {_enemyArray[_currSpawnIndex][0], _enemyArray[_currSpawnIndex][1],
                                        _enemyArray[_currSpawnIndex][2], _enemyArray[_currSpawnIndex][3]};
            std::vector<float> enemySpawn = {_enemyArray[_currSpawnIndex][0], _enemyArray[_currSpawnIndex][1],
                      _enemyArray[_currSpawnIndex][2], _enemyArray[_currSpawnIndex][3], _enemyArray[_currSpawnIndex][5]};
            gameModel._enemyArrayMaster[(int)(_enemyArray[_currSpawnIndex][5])].push_back(enemy);
            CULog("SPAWNING SPAWNING");
            CULog("Length: %d", gameModel._enemyArrayMaster[(int)(_enemyArray[_currSpawnIndex][5])].size());

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