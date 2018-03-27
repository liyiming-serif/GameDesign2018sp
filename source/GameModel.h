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
#include <Array>
#include "EnemyModel.h"

class GameModel {
protected:
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    int _castleHealth[6];
    int _playerAvatars[6];
    int _prevCastleHealth[6];
    int _spawnTimer;
    cugl::Size _size;
    std::shared_ptr<cugl::TextWriter> _networkJSONWriter;
    std::shared_ptr<cugl::TextReader> _networkJSONReader;
    std::string _networkStateOutPath;
    std::string _networkStateInPath;

public:

    // Constructors
    GameModel() {};

    bool init(const std::shared_ptr<cugl::AssetManager> &assets);

    //enemy array, specifies air/ground and direction (N, NE, SE, S, SW, NW), will add the rest later
    std::set<std::shared_ptr<EnemyModel>> _enemyArrayGroundN;
    std::set<std::shared_ptr<EnemyModel>> _enemiesToFree;

    // Destructors
    void dispose();

    ~GameModel() { dispose(); }

    // Gameplay
    void update(float deltaTime);

    // Physics manager
    //std::shared_ptr<cugl::ObstacleWorld> _world;

    int getWallHealth(int wall);

    void changeWallHealth(int wall, int damage);

    int getPlayerAvatar(int player);

    void setPlayerAvatar(int player, int avatar);

private:
    std::string getStateChange();

    void updateState();

};

extern GameModel gameModel;

#endif //BUILD_ANDROID_GAMEMODEL_H
