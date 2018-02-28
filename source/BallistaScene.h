//
// Created by Yiming on 2/25/2018.
//

#ifndef BUILD_ANDROID_BALLISTASCENE_H
#define BUILD_ANDROID_BALLISTASCENE_H
#include <cugl/cugl.h>

class BallistaScene : public cugl::Scene{
protected:
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;

    // Controllers
    void touchDragCB(const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus);

    // Models
    std::shared_ptr<cugl::PolygonNode> _ballista;

public:
    // Constructors
    BallistaScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    // Destructors
    void dispose();
    ~BallistaScene() {dispose();}

    // Gameplay
    void update(float timestep);

};

#endif //BUILD_ANDROID_BALLISTASCENE_H