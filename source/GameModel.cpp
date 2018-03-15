//
// Created by Josh on 3/15/2018.
//

#include "GameModel.h"
#define DRAW_SCALE 32

using namespace cugl;

bool GameModel::init(const std::shared_ptr<AssetManager>& assets){

    _assets = assets;

    _spawnTimer = 360;


    //testing enemy code
        std::shared_ptr<EnemyModel> enemy1 = EnemyModel::alloc(Vec2(200, 200), 0, 1, DRAW_SCALE, _assets);
            if(enemy1 != nullptr) {
                _enemyArrayGroundN.insert(enemy1);
            }
        std::shared_ptr<EnemyModel> enemy2 = EnemyModel::alloc(Vec2(200, 400), 0, 1, DRAW_SCALE, _assets);
            if(enemy2 != nullptr) {
                _enemyArrayGroundN.insert(enemy2);
            }
    return true;
}

void GameModel::dispose() {
    _assets = nullptr;
    _enemyArrayGroundN.clear();
}

void GameModel::update(float timestep){
    if(_spawnTimer == 0){
        std::shared_ptr<EnemyModel> enemy1 = EnemyModel::alloc(Vec2(200, 200), 0, 1, DRAW_SCALE, _assets);
                if(enemy1 != nullptr) {
                    _enemyArrayGroundN.insert(enemy1);
                }
        _spawnTimer = 360;
    }
    else{
        _spawnTimer--;
    }
}