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
#include <array>
#include <vector>
#include "EnemyModel.h"

class GameModel{
protected:
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    int _castleHealth[6];
    int _spawnTimer;
    cugl::Size _size;
    int _arrowAmmo[2];

public:

    // Constructors
    GameModel(){};
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    //enemy array, specifies air/ground and direction (N, NE, SE, S, SW, NW), will add the rest later
    //2D vector, each element has {xCoord, yCoord, type, remaining health}
    std::vector<std::vector<std::vector<float>>> _enemyArrayMaster;
    std::vector<std::vector<int>> _enemiesToFreeMaster;

    // Destructors
    void dispose();
    ~GameModel() {dispose();}

    // Gameplay
    void update(float deltaTime);

    // Physics manager
    //std::shared_ptr<cugl::ObstacleWorld> _world;

    int getWallHealth(int wall);

    void changeWallHealth(int wall, int damage);

    int getArrowAmmo(int type) {
        return _arrowAmmo[type];
    }

    void setArrowAmmo(int type, int amount) {
        if (_arrowAmmo[type] + amount > 99) {
            _arrowAmmo[type] = 99;
        }
        else if (_arrowAmmo[type] + amount < 0) {
            _arrowAmmo[type] = 0;
        }
        else {
            _arrowAmmo[type] += amount;
        }
    }

};

extern GameModel gameModel;

#endif //BUILD_ANDROID_GAMEMODEL_H
