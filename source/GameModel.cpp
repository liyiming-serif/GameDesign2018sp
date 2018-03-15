//
// Created by Josh on 3/15/2018.
//

#include "GameModel.h"
#define DRAW_SCALE 32

using namespace cugl;

bool GameModel::init(const std::shared_ptr<AssetManager>& assets){

    if (assets == nullptr) {
            return false;
        } else if (!Scene::init(_size)) {
            return false;
        }

    _assets = assets;


    //testing enemy code
        std::shared_ptr<EnemyModel> enemy1 = EnemyModel::alloc(Vec2(200, 200), 0, 1, DRAW_SCALE, _assets);
            if(a != nullptr) {
                _enemyArrayGroundN.push_back(enemy1);
            }
        std::shared_ptr<EnemyModel> enemy2 = EnemyModel::alloc(Vec2(200, 400), 0, 1, DRAW_SCALE, _assets);
            if(a != nullptr) {
                _enemyArrayGroundN.push_back(enemy2);
            }
    return true;
}

void GameModel::dispose() {
    removeAllChildren();
    _assets = nullptr;
    _enemyArrayGroundN = nullptr;
}

void GameModel::update(float timestep){

}