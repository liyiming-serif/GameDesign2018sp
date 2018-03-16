//
// Created by Josh on 3/15/2018.
//

#include "GameModel.h"
#define DRAW_SCALE 12
#define GAME_WIDTH 1024

using namespace cugl;

bool GameModel::init(const std::shared_ptr<AssetManager>& assets){
    // Set display size
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;

    _assets = assets;

    _spawnTimer = 360;

    // Create the physics world
    _world = ObstacleWorld::alloc(Rect(Vec2::ZERO, _size/DRAW_SCALE),Vec2::ZERO);


    //testing enemy code
        std::shared_ptr<EnemyModel> enemy1 = EnemyModel::alloc(Vec2(200, 200), 0, 1, DRAW_SCALE, _assets);
            if(enemy1 != nullptr) {
                _enemyArrayGroundN.insert(enemy1);
                _world->addObstacle(enemy1);
            }
        std::shared_ptr<EnemyModel> enemy2 = EnemyModel::alloc(Vec2(200, 400), 0, 1, DRAW_SCALE, _assets);
            if(enemy2 != nullptr) {
                _enemyArrayGroundN.insert(enemy2);
                _world->addObstacle(enemy2);
            }
    return true;
}

void GameModel::dispose() {
    if (_world != nullptr) {
        _world->clear();
        _world = nullptr;
    }
    _assets = nullptr;
    _enemyArrayGroundN.clear();
}

void GameModel::update(float timestep){
    if(_spawnTimer == 0){
        std::shared_ptr<EnemyModel> enemy1 = EnemyModel::alloc(Vec2(200, 200), 0, 1, DRAW_SCALE, _assets);
                if(enemy1 != nullptr) {
                    _enemyArrayGroundN.insert(enemy1);
                    _world->addObstacle(enemy1);
                }
        _spawnTimer = 360;
        CULog("spawn");
    }
    else{
        _spawnTimer--;
    }
    _world->update(timestep);
}