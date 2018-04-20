//
// Created by Yiming on 3/2/2018.
//

#ifndef BUILD_ANDROID_LOOKOUTSCENE_H
#define BUILD_ANDROID_LOOKOUTSCENE_H

#include "GameModel.h"
#include "EnemyModel.h"
#include "InputController.h"
#include <cugl/cugl.h>
#include <vector>

class LookoutScene : public cugl::Scene{
protected:
    cugl::Size _size;

    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;

    std::shared_ptr<cugl::Button> _lookoutTOcastle;

    std::shared_ptr<cugl::PolygonNode> _background;

	std::shared_ptr<cugl::Texture> _enemyIcon;

    std::vector<std::shared_ptr<cugl::Node>> _enemyMarkers;

public:
    // Constructors
    LookoutScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    // Destructors
    void dispose();
    ~LookoutScene() {dispose();}

    // Gameplay
    void update(float timestep);

    //Pause or Resume
    void setActive(bool active);
    int switchscene;
};
#endif //BUILD_ANDROID_LOOKOUTSCENE_H
