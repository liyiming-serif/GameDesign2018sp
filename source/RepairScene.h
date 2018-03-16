//
//  RepairScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 3/9/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef RepairScene_h
#define RepairScene_h

#include <stdio.h>
#include <cugl/cugl.h>

using namespace cugl;

class RepairScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::ActionManager> _actions;
    
    std::shared_ptr<cugl::FadeIn> _wallFadeIN;
    std::shared_ptr<cugl::FadeOut> _wallFadeOUT;
    
    std::shared_ptr<PolygonNode> plain_floor;
    std::shared_ptr<PolygonNode> northWall_floor;
    std::shared_ptr<PolygonNode> northeastWall_floor;
    std::shared_ptr<PolygonNode> southeastWall_floor;
    std::shared_ptr<PolygonNode> southWall_floor;
    std::shared_ptr<PolygonNode> southwestWall_floor;
    std::shared_ptr<PolygonNode> northwestWall_floor;

    
    std::shared_ptr<cugl::Button> _overworld_button3;
    std::shared_ptr<cugl::Button> _northWallButton;
    std::shared_ptr<cugl::Button> _northeastWallButton;
    std::shared_ptr<cugl::Button> _southeastWallButton;
    std::shared_ptr<cugl::Button> _southWallButton;
    std::shared_ptr<cugl::Button> _southwestWallButton;
    std::shared_ptr<cugl::Button> _northwestWallButton;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    std::shared_ptr<cugl::Node>  _buttons;
    
    std::string _curr_wall;
    std::string _new_wall;

    std::shared_ptr<cugl::PolygonNode> _healthbar_good;
    std::shared_ptr<cugl::PolygonNode> _healthbar_warning;
    std::shared_ptr<cugl::PolygonNode> _healthbar_low;
    std::shared_ptr<cugl::PolygonNode> _health_frame;
    
    
    
public:
    // Constructors
    RepairScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~RepairScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active);
    int switchscene;
    
    /**
     * Performs a fade in action
     *
     * @param action The move action
     */
    void doFadeIn(const std::shared_ptr<cugl::FadeIn>& action, std::string direction);
    
    /**
     * Performs a fade out action
     *
     * @param action The move action
     */
    void doFadeOut(const std::shared_ptr<cugl::FadeOut>& action, std::string direction);
    
    std::shared_ptr<cugl::PolygonNode> getWall(std::string direction);
};


#endif /* RepairScene_h */
