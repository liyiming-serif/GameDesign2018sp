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
    std::shared_ptr<cugl::FadeIn> _castleFadeINSTANT;
    
    //background floorplan
    std::shared_ptr<cugl::Node>  _background;
    std::shared_ptr<cugl::Node>  _levels;
	std::shared_ptr<cugl::PolygonNode> _oil_floor; //these floors are dynamic based on level
	std::shared_ptr<cugl::PolygonNode> _ballista_floor;
    std::shared_ptr<cugl::PolygonNode> _basement_floor;
	std::shared_ptr<cugl::PolygonNode> _lookout_floor;
    
    //castle views
    std::shared_ptr<cugl::PolygonNode> _castle_background;
    std::shared_ptr<cugl::PolygonNode> _castle_flag;
    std::shared_ptr<cugl::PolygonNode> _castle_black;
    std::shared_ptr<cugl::PolygonNode> _castle_ballista;
    std::shared_ptr<cugl::PolygonNode> _castle_oil;
    std::shared_ptr<cugl::PolygonNode> _castle_lookout;
    std::shared_ptr<cugl::PolygonNode> _castle_basement;


    //buttons
    std::shared_ptr<cugl::Button> _menuButton;
    //Ballista Buttons
        std::shared_ptr<cugl::Button> _ballistaNorth;
        std::shared_ptr<cugl::Button> _ballistaNorthEast;
        std::shared_ptr<cugl::Button> _ballistaEast;
        std::shared_ptr<cugl::Button> _ballistaSouthEast;
        std::shared_ptr<cugl::Button> _ballistaSouth;
        std::shared_ptr<cugl::Button> _ballistaSouthWest;
        std::shared_ptr<cugl::Button> _ballistaWest;
        std::shared_ptr<cugl::Button> _ballistaNorthWest;
    //Oil Buttons
        std::shared_ptr<cugl::Button> _oilNorth;
        std::shared_ptr<cugl::Button> _oilNorthEast;
        std::shared_ptr<cugl::Button> _oilEast;
        std::shared_ptr<cugl::Button> _oilSouthEast;
        std::shared_ptr<cugl::Button> _oilSouth;
        std::shared_ptr<cugl::Button> _oilSouthWest;
        std::shared_ptr<cugl::Button> _oilWest;
        std::shared_ptr<cugl::Button> _oilNorthWest;
    
        std::shared_ptr<cugl::PolygonNode> _oilNorthLOCKED;
        std::shared_ptr<cugl::PolygonNode> _oilNorthEastLOCKED;
        std::shared_ptr<cugl::PolygonNode> _oilEastLOCKED;
        std::shared_ptr<cugl::PolygonNode> _oilSouthEastLOCKED;
        std::shared_ptr<cugl::PolygonNode> _oilSouthLOCKED;
        std::shared_ptr<cugl::PolygonNode> _oilSouthWestLOCKED;
        std::shared_ptr<cugl::PolygonNode> _oilWestLOCKED;
        std::shared_ptr<cugl::PolygonNode> _oilNorthWestLOCKED;
    //Lookout Button
        std::shared_ptr<cugl::Button> _lookout_button;
    //Basement Buttons
        std::shared_ptr<cugl::Button> _repair_button;
        std::shared_ptr<cugl::Button> _mage_button;
        std::shared_ptr<cugl::Button> _ammo_button;
    
        std::shared_ptr<cugl::PolygonNode> _repair_buttonLOCKED;
        std::shared_ptr<cugl::PolygonNode>_mage_buttonLOCKED;
        std::shared_ptr<cugl::PolygonNode> _ammo_buttonLOCKED;
    

    
    
    
    
    std::shared_ptr<cugl::PolygonNode> _cloud1;
    std::shared_ptr<cugl::PolygonNode> _cloud2;
    std::shared_ptr<cugl::PolygonNode> _cloud3;

    
    
    /** The movement actions */
    std::shared_ptr<cugl::MoveTo> _move1;
    std::shared_ptr<cugl::MoveTo> _move2;
    std::shared_ptr<cugl::MoveTo> _move3;

	/** Damage Indicators */
	std::vector<std::shared_ptr<cugl::PolygonNode>> _dmgIndicators;

    cugl::Size _size;
    


public:
    // Constructors
    OverworldScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~OverworldScene() {dispose();}
    
    // Gameplay
    void update(float timestep);


    void setActive(bool active);
    int switchscene;
    bool isMoving = false;
    float stairs;
    cugl::Vec2 castleOrigin;
    int currentCastleFloor;
    
    bool click;
    int player_TEST=2;

    //-1 no direction, 0 N, 1 NE, 2 SE, 3 S, 4 SW, 5 SE
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
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove1(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
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
    
	/**iterate through gameModel's dmg castle health and apply indicators*/
	void pollDmgIndicators();
    
    /**
     * Disables all buttons
     *
     *
     */
    void disableButtons();
    
    /**
     * Enables all buttons
     *
     *
     */
    void enableButtons();
    
    

};




#endif /* OverworldScene_h */
