//
// GameModel.h
//
// Created by Josh on 3/15/2018.
// stores enemy positions in vectors, moves enemies
//

#ifndef BUILD_ANDROID_GAMEMODEL_H
#define BUILD_ANDROID_GAMEMODEL_H

#include <cugl/cugl.h>
#include <set>
#include "EnemyModel.h"

class GameModel{
protected:
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;

    int _spawnTimer;
    cugl::Size _size;

public:

    // Constructors
    GameModel(){};
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    //enemy array, specifies air/ground and direction (N, NE, SE, S, SW, NW), will add the rest later
    std::set<std::shared_ptr<EnemyModel>> _enemyArrayGroundN;

    // Destructors
    void dispose();
    ~GameModel() {dispose();}

    // Gameplay
    void update(float timestep);

    // Physics manager
    std::shared_ptr<cugl::ObstacleWorld> _world;

};

#endif //BUILD_ANDROID_GAMEMODEL_H
