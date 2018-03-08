//
//  OverworldScene.h
//  HelloWorld
//
//  Created by Noah Sterling on 3/1/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef OverworldScene_h
#define OverworldScene_h

#include <stdio.h>
#include <cugl/cugl.h>


class OverworldScene : public cugl::Scene{
protected:
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::ActionManager> _actions;
    
    /** The movement actions */
    std::shared_ptr<cugl::MoveBy> _moveup;
    std::shared_ptr<cugl::MoveBy> _movedn;
    
    //background floorplan
    std::shared_ptr<cugl::Node>  _background;
    std::shared_ptr<cugl::Node>  _levels;

    //buttons
    std::shared_ptr<cugl::Button> _quitButton;
    //Ballista Buttons
        std::shared_ptr<cugl::Button> _ballistaNorth;
        std::shared_ptr<cugl::Button> _ballistaNorthEast;
        std::shared_ptr<cugl::Button> _ballistaEast;
        std::shared_ptr<cugl::Button> _ballistaSouthEast;
        std::shared_ptr<cugl::Button> _ballistaSouth;
        std::shared_ptr<cugl::Button> _ballistaSouthWest;
        std::shared_ptr<cugl::Button> _ballistaWest;
        std::shared_ptr<cugl::Button> _ballistaNorthWest;
    //Lookout Button
    std::shared_ptr<cugl::Button> _lookout_button;
    
    std::shared_ptr<cugl::Button> _up_button;
    std::shared_ptr<cugl::Button> _down_button;
    


public:
    // Constructors
    OverworldScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~OverworldScene() {dispose();}
    
    // Gameplay
    void update(float timestep);

    // What sort of a codebase are you running here? The demos are so inconsistent.
    void setActive(bool active);
    int switchscene;
    bool isMoving = false;
    float stairs;
    
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove(const std::shared_ptr<cugl::MoveBy>& action);
    
    
};


#endif /* OverworldScene_h */
