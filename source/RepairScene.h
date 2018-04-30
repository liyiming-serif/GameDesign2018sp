//
//  RepairScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 3/9/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef RepairScene_h
#define RepairScene_h

#include "InputController.h"
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
    
    std::shared_ptr<PolygonNode> N_100;
    std::shared_ptr<PolygonNode> NE_100;
    std::shared_ptr<PolygonNode> NW_100;
    std::shared_ptr<PolygonNode> S_100;
    std::shared_ptr<PolygonNode> SE_100;
    std::shared_ptr<PolygonNode> SW_100;
    
    std::shared_ptr<PolygonNode> N_75;
    std::shared_ptr<PolygonNode> NE_75;
    std::shared_ptr<PolygonNode> NW_75;
    std::shared_ptr<PolygonNode> S_75;
    std::shared_ptr<PolygonNode> SE_75;
    std::shared_ptr<PolygonNode> SW_75;
    
    std::shared_ptr<PolygonNode> N_50;
    std::shared_ptr<PolygonNode> NE_50;
    std::shared_ptr<PolygonNode> NW_50;
    std::shared_ptr<PolygonNode> S_50;
    std::shared_ptr<PolygonNode> SE_50;
    std::shared_ptr<PolygonNode> SW_50;
    
    std::shared_ptr<PolygonNode> N_25;
    std::shared_ptr<PolygonNode> NE_25;
    std::shared_ptr<PolygonNode> NW_25;
    std::shared_ptr<PolygonNode> S_25;
    std::shared_ptr<PolygonNode> SE_25;
    std::shared_ptr<PolygonNode> SW_25;


    
    std::shared_ptr<cugl::Button> _repairTOcastle;
    std::shared_ptr<cugl::Button> _northWallButton;
    std::shared_ptr<cugl::Button> _northeastWallButton;
    std::shared_ptr<cugl::Button> _southeastWallButton;
    std::shared_ptr<cugl::Button> _southWallButton;
    std::shared_ptr<cugl::Button> _southwestWallButton;
    std::shared_ptr<cugl::Button> _northwestWallButton;
    
    /** Reference to the label */
    std::shared_ptr<cugl::Label> _northText;
    std::shared_ptr<cugl::Label> _northeastText;
    std::shared_ptr<cugl::Label> _southeastText;
    std::shared_ptr<cugl::Label> _southText;
    std::shared_ptr<cugl::Label> _southwestText;
    std::shared_ptr<cugl::Label> _northwestText;
    
    std::shared_ptr<Texture> image_2;
    
    
    std::shared_ptr<cugl::PolygonNode> _background;
    std::shared_ptr<cugl::Node>  _buttons;
    
    std::string _curr_wall;
    std::string _new_wall;

    std::shared_ptr<cugl::PolygonNode> _healthbar_good;
    std::shared_ptr<cugl::PolygonNode> _healthbar_warning;
    std::shared_ptr<cugl::PolygonNode> _healthbar_low;
    std::shared_ptr<cugl::PolygonNode> _health_frame;
    
    int player_TEST=2;
    
    
    
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
    
    std::shared_ptr<cugl::Button> getWall(std::string direction);
};


#endif /* RepairScene_h */
