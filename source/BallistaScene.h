//
// Created by Yiming on 2/25/2018.
//

#ifndef BUILD_ANDROID_BALLISTASCENE_H
#define BUILD_ANDROID_BALLISTASCENE_H
#include <cugl/cugl.h>
#include "ArrowModel.h"
#include "InputController.h"
#include "GameModel.h"
#include <set>

class BallistaScene : public cugl::Scene{
protected:
    cugl::Size _size;

    int _spawnTimer;

    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;

    // Physics manager
    std::shared_ptr<cugl::ObstacleWorld> _world;

    // Memory Managers
    std::set<std::shared_ptr<ArrowModel>> _arrows;
	std::set<std::shared_ptr<ArrowModel>> _arrowsToFree;

    // Models
    std::shared_ptr<cugl::PolygonNode> _ballista;
    std::shared_ptr<cugl::PolygonNode> _background;
    std::shared_ptr<cugl::Button> _overworld_button;

public:
    // Constructors
    BallistaScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    // Destructors
    void dispose();
    ~BallistaScene() {dispose();}

    // Gameplay
    void update(float deltaTime);

    //Pause or Resume
    void setActive(bool active);
    int switchscene;

};

#endif //BUILD_ANDROID_BALLISTASCENE_H