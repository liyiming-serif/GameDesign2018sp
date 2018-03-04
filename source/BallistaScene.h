//
// Created by Yiming on 2/25/2018.
//

#ifndef BUILD_ANDROID_BALLISTASCENE_H
#define BUILD_ANDROID_BALLISTASCENE_H
#include <cugl/cugl.h>

class BallistaScene : public cugl::Scene{
protected:
    cugl::Size _size;

    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;

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
    void update(float timestep);

    //Pause or Resume
    void setActive(bool active);
    int switchscene;

    // Input Callbacks
    void touchDragCB(const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus);
    void touchReleaseCB(const cugl::TouchEvent& event, bool focus);

};

#endif //BUILD_ANDROID_BALLISTASCENE_H