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
    //XCoord, YCoord, Type, Health, SpawnTime, Sector
    std::vector<float> enemy = {100.0, 500.0, 1.0, 1.0, 300.0, 0.0};
    _enemyArray.push_back(enemy);
    enemy = {500.0, 500.0, 1.0, 1.0, 300.0, 0.0};
    _enemyArray.push_back(enemy);
    enemy = {950.0, 500.0, 1.0, 1.0, 300.0, 0.0};
    _enemyArray.push_back(enemy);
    enemy = {300.0, 500.0, 1.0, 1.0, 600.0, 3.0};
    _enemyArray.push_back(enemy);
    enemy = {700.0, 500.0, 1.0, 1.0, 600.0, 3.0};
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
    enemy = {550.0, 500.0, 1.0, 1.0, 2200.0, 2.0};
    _enemyArray.push_back(enemy);
    enemy = {800.0, 500.0, 1.0, 1.0, 2200.0, 2.0};
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