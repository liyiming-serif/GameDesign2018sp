//
//  LevelselectScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 4/10/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelselectScene_h
#define LevelselectScene_h

#include "InputController.h"
#include <stdio.h>
#include <cugl/cugl.h>

class LevelselectScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _playButton;
    std::shared_ptr<cugl::Button> _backButton;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    
    std::shared_ptr<cugl::PolygonNode> _cloud1;
    std::shared_ptr<cugl::PolygonNode> _cloud2;
    std::shared_ptr<cugl::PolygonNode> _cloud3;
    std::shared_ptr<cugl::PolygonNode> _cloud4;
    std::shared_ptr<cugl::PolygonNode> _cloud5;
    std::shared_ptr<cugl::PolygonNode> _cloud6;
    
    
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::ActionManager> _actions;
    
    /** The movement actions */
    std::shared_ptr<cugl::MoveTo> _move1;
    std::shared_ptr<cugl::MoveTo> _move2;
    std::shared_ptr<cugl::MoveTo> _move3;
    std::shared_ptr<cugl::MoveTo> _move4;
    std::shared_ptr<cugl::MoveTo> _move5;
    std::shared_ptr<cugl::MoveTo> _move6;
    
    bool move1 = true;
    bool move2 = true;
    bool move3 = true;
    bool move4 = true;
    bool move5 = true;
    bool move6 = true;

    
    
    
public:
    // Constructors
    LevelselectScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~LevelselectScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active, int players);
    int switchscene;
    int mode;
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove2(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove3(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove4(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove5(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove6(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);

};

#endif /* LevelselectScene_h */
