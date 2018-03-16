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
#include "InputController.h"
#include <map>
#include "GameModel.h"


class OverworldScene : public cugl::Scene{
protected:
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::ActionManager> _actions;
    
    /** The movement actions */
    std::shared_ptr<cugl::MoveBy> _moveup;
    std::shared_ptr<cugl::MoveBy> _movedn;
    std::shared_ptr<cugl::FadeIn> _castleFadeIN;
    std::shared_ptr<cugl::FadeOut> _castleFadeOUT;
    std::shared_ptr<cugl::FadeOut> _castleFadeINSTANT;
    
    //background floorplan
    std::shared_ptr<cugl::Node>  _background;
    std::shared_ptr<cugl::Node>  _levels;
    
    //castle views
    std::shared_ptr<cugl::PolygonNode> _castle_ballista;
    std::shared_ptr<cugl::PolygonNode> _castle_catapult;
    std::shared_ptr<cugl::PolygonNode> _castle_oil;
    std::shared_ptr<cugl::PolygonNode> _castle_lookout;
    std::shared_ptr<cugl::PolygonNode> _castle_basement;


    //buttons
    std::shared_ptr<cugl::Button> _quitButton;
    std::shared_ptr<cugl::Button> _resetButton;
    //Ballista Buttons
        std::shared_ptr<cugl::Button> _ballistaNorth;
        std::shared_ptr<cugl::Button> _ballistaNorthEast;
        std::shared_ptr<cugl::Button> _ballistaEast;
        std::shared_ptr<cugl::Button> _ballistaSouthEast;
        std::shared_ptr<cugl::Button> _ballistaSouth;
        std::shared_ptr<cugl::Button> _ballistaSouthWest;
        std::shared_ptr<cugl::Button> _ballistaWest;
        std::shared_ptr<cugl::Button> _ballistaNorthWest;
    //Catapult Buttons
        std::shared_ptr<cugl::Button> _catapultNorth;
        std::shared_ptr<cugl::Button> _catapultNorthEast;
        std::shared_ptr<cugl::Button> _catapultEast;
        std::shared_ptr<cugl::Button> _catapultSouthEast;
        std::shared_ptr<cugl::Button> _catapultSouth;
        std::shared_ptr<cugl::Button> _catapultSouthWest;
        std::shared_ptr<cugl::Button> _catapultWest;
        std::shared_ptr<cugl::Button> _catapultNorthWest;
    //Oil Buttons
        std::shared_ptr<cugl::Button> _oilNorth;
        std::shared_ptr<cugl::Button> _oilNorthEast;
        std::shared_ptr<cugl::Button> _oilEast;
        std::shared_ptr<cugl::Button> _oilSouthEast;
        std::shared_ptr<cugl::Button> _oilSouth;
        std::shared_ptr<cugl::Button> _oilSouthWest;
        std::shared_ptr<cugl::Button> _oilWest;
        std::shared_ptr<cugl::Button> _oilNorthWest;
    //Lookout Button
    std::shared_ptr<cugl::Button> _lookout_button;
    //Basement Buttons
    std::shared_ptr<cugl::Button> _repair_button;
    
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
    cugl::Vec2 castleOrigin;
    int currentCastleFloor;

    //0 no direction, 1 N, 2 NE, 3 SE, 4 S, 5 SW, 6 SE
    int direction;
    
    
    /**
     * Resets the castle to the first floor
     *
     *
     */
    void resetCastle();
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove(const std::shared_ptr<cugl::MoveBy>& action, int floor);
    
    /**
     * Performs a fade in action
     *
     * @param action The move action
     */
    void doFadeIn(const std::shared_ptr<cugl::FadeIn>& action,  int floor);
    
    /**
     * Performs a fade out action
     *
     * @param action The move action
     */
    void doFadeOut(const std::shared_ptr<cugl::FadeOut>& action,  int floor);
    
    /**
     * Returns the correct tower view
     *
     * @param floor The floor number
     */
    std::shared_ptr<cugl::PolygonNode> getTowerView(int floor);
    
    
    //void touchDragCB(const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus);
    
};




#endif /* OverworldScene_h */
