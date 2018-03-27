//
// Created by Josh on 3/26/2018.
//

#include "SpawnController.h"

using namespace cugl;

/**
 * Creates a spawn controller
 */
bool SpawnController::init(const std::shared_ptr<AssetManager>& assets){
    //when this equals -1, stop spawning
    _active = true;
    _currSpawnIndex = 0;
    _totalTime = 0;

    //temporarily fill in the enemyArray, will read from JSON later
    std::vector<float> enemy = {100.0, 500.0, 1.0, 2.0, 360.0, 1.0};
    std::vector<float> enemy2 = {500.0, 500.0, 1.0, 2.0, 420.0, 1.0};
    std::vector<float> enemy3 = {950.0, 500.0, 1.0, 2.0, 420.0, 1.0};
    _enemyArray.push_back(enemy);
    _enemyArray.push_back(enemy2);
    _enemyArray.push_back(enemy3);

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
            gameModel._newSpawn = enemySpawn;
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