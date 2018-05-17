//
//  AmmoScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AmmoScene_h
#define AmmoScene_h

#include <stdio.h>
#include "GameModel.h"
#include "InputController.h"
#include <cugl/cugl.h>

class AmmoScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _ammoTOcastle;
    std::shared_ptr<cugl::Button> _hammer;
    std::shared_ptr<cugl::Button> _hammer2;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    std::shared_ptr<cugl::PolygonNode> _cursor;
    
    
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::ActionManager> _actions;
    
    /** The movement actions */
    std::shared_ptr<cugl::MoveTo> _moveleft;
    std::shared_ptr<cugl::MoveTo> _moveright;
    std::shared_ptr<cugl::MoveTo> _moveup_J;
    std::shared_ptr<cugl::MoveTo> _moveup_O;
    std::shared_ptr<cugl::MoveTo> _moveup_B;
	std::shared_ptr<cugl::FadeOut> _dmgFadeOUT;
    
    /** The animation */
    std::shared_ptr<cugl::AnimationNode> _hammerAnim;
    
    /** The animation actions */
    std::shared_ptr<cugl::Animate> _hammerAnimation;

    //text
    std::shared_ptr<cugl::Label> _ammoText;
    
    bool move_direction;
    
	/** Damage Indicators */
	std::vector<std::shared_ptr<cugl::PolygonNode>> _dmgIndicators;
    
    //tutorial
    std::shared_ptr<cugl::PolygonNode> _tap;
    bool _tapTutorial;
    
    
public:
    // Constructors
    AmmoScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~AmmoScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active);
    int switchscene;
    bool test;
    std::shared_ptr<cugl::PolygonNode> _jackpot;
    
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove(const std::shared_ptr<cugl::MoveTo>& action);
    
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMoveUp(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> jackpot);
    
    /**
     * Performs a film strip action
     *
     * @param action The film strip action
     */
    void doStrip(const std::shared_ptr<cugl::Animate>& action);

	/**iterate through gameModel's dmg castle health and apply indicators*/
	void pollDmgIndicators();
    
    int _ammoClick;
};


#endif /* AmmoScene_h */
