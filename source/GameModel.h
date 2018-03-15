//
// GameModel.h
//
// Created by Josh on 3/15/2018.
// stores enemy positions in vectors
//

#ifndef BUILD_ANDROID_GAMEMODEL_H
#define BUILD_ANDROID_GAMEMODEL_H

#include <cugl/cugl.h>
#include <vector>

class GameModel{
protected:
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
public:

    // Constructors
    MenuScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    //enemy array, specifies air/ground and direction (N, NE, SE, S, SW, NW), will add the rest later
    const std::vector<std::shared_ptr<EnemyModel>> enemyArrayGroundN;

    // Destructors
    void dispose();
    ~MenuScene() {dispose();}

    // Gameplay
    void update(float timestep);

};

#endif //BUILD_ANDROID_GAMEMODEL_H
