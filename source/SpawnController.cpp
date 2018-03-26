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
    std::vector<float> enemy = {100.0, 100.0, 1.0, 1.0, 360.0};
    std::vector<float> enemy2 = {100.0, 400.0, 1.0, 1.0, 420.0};
    _enemyArray.push_back(enemy);
    _enemyArray.push_back(enemy2);

    return true;
}

void SpawnController::update(float deltaTime) {
    CULog("total Time: %d", _totalTime);
    _totalTime++;
    if(_currSpawnIndex != -1){
        if(_enemyArray[_currSpawnIndex][4]<_totalTime){
        CULog("SPAWNING SPAWNING");
        //we need to spawn one
            std::vector<float> enemy = {_enemyArray[_currSpawnIndex][0], _enemyArray[_currSpawnIndex][1],
                                        _enemyArray[_currSpawnIndex][2], _enemyArray[_currSpawnIndex][3]};
            gameModel._enemyArrayGroundN.push_back(enemy);
            CULog("GameModel array size: %d", gameModel._enemyArrayGroundN.size());

            if(_currSpawnIndex != _enemyArray.size()-1){
            //still have more enemies to spawn
                _currSpawnIndex++;
            }
            else{
            //done spawning
                _currSpawnIndex = -1;
            }
        CULog("currSpawnIndex: %d", _currSpawnIndex);
        }
    }
}

void SpawnController::dispose(){
    _active = false;
    _enemyArray.clear();
}