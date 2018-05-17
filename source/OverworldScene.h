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
	std::shared_ptr<cugl::FadeOut> _dmgFadeOUT;
    
    //background floorplan
    std::shared_ptr<cugl::Node>  _background;
    std::shared_ptr<cugl::Node>  _levels;
	std::shared_ptr<cugl::PolygonNode> _oil_floor; //these floors are dynamic based on level
	std::shared_ptr<cugl::PolygonNode> _ballista_floor;
    std::shared_ptr<cugl::PolygonNode> _basement_floor;
	std::shared_ptr<cugl::PolygonNode> _lookout_floor;
    
    //castle views
    std::shared_ptr<cugl::PolygonNode> _castle_background;
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
    
        std::shared_ptr<cugl::PolygonNode> _oilNorthNEW;
        std::shared_ptr<cugl::PolygonNode> _oilNorthEastNEW;
        std::shared_ptr<cugl::PolygonNode> _oilEastNEW;
        std::shared_ptr<cugl::PolygonNode> _oilSouthEastNEW;
        std::shared_ptr<cugl::PolygonNode> _oilSouthNEW;
        std::shared_ptr<cugl::PolygonNode> _oilSouthWestNEW;
        std::shared_ptr<cugl::PolygonNode> _oilWestNEW;
        std::shared_ptr<cugl::PolygonNode> _oilNorthWestNEW;
    //Lookout Button
        std::shared_ptr<cugl::Button> _lookout_button;
    //Basement Buttons
        std::shared_ptr<cugl::Button> _repair_button;
        std::shared_ptr<cugl::Button> _mage_button;
        std::shared_ptr<cugl::Button> _ammo_button;
    
        std::shared_ptr<cugl::PolygonNode> _repair_buttonLOCKED;
        std::shared_ptr<cugl::PolygonNode>_mage_buttonLOCKED;
        std::shared_ptr<cugl::PolygonNode> _ammo_buttonLOCKED;
    
        std::shared_ptr<cugl::PolygonNode> _repair_buttonNEW;
        std::shared_ptr<cugl::PolygonNode>_mage_buttonNEW;
        std::shared_ptr<cugl::PolygonNode> _ammo_buttonNEW;
    

    
    
    
    
    std::shared_ptr<cugl::PolygonNode> _cloud1;
    std::shared_ptr<cugl::PolygonNode> _cloud2;
    std::shared_ptr<cugl::PolygonNode> _cloud3;

    
    
    /** The movement actions */
    std::shared_ptr<cugl::MoveTo> _move1;
    std::shared_ptr<cugl::MoveTo> _move2;
    std::shared_ptr<cugl::MoveTo> _move3;
    
    
    /** The animation */
    std::shared_ptr<cugl::AnimationNode> _castle_flag;
    
    /** The animation actions */
    std::shared_ptr<cugl::Animate> _flagAnimation;

	/** Damage Indicators */
	std::vector<std::shared_ptr<cugl::PolygonNode>> _dmgIndicators;
    
    /** Tutorial */
    std::shared_ptr<cugl::PolygonNode> _swipe;
    int _swipeUP;
    int _swipeDN;
    std::shared_ptr<cugl::PolygonNode> _tap;
    int _roomClick;
    int _oilClick;
    int _ammoClick;
    int _repairClick;
    int _mageClick;
    bool _swipeTutorial;
    bool _newOil;
    bool _newAmmo;
    bool _newRepair;

    
    //Pause
    std::shared_ptr<cugl::PolygonNode> _pauseBG;
    std::shared_ptr<cugl::Button> _pauseQUIT;
    std::shared_ptr<cugl::Button> _pauseREPLAY;
    std::shared_ptr<cugl::Button> _pauseBACK;
    std::shared_ptr<cugl::Button> _pauseSOUND;
    std::shared_ptr<cugl::PolygonNode> _pauseMUTE;


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
    bool wizard;
    int player_TEST=2;
    bool _newMage;
//    bool _mac;

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
    
    
    /**
     * Performs a film strip action
     *
     * @param action The film strip action
     */
    void doStrip(const std::shared_ptr<cugl::Animate>& action);
    
    void resetTutorial();
    
    bool isPaused;
    
    
    
    

};




#endif /* OverworldScene_h */
