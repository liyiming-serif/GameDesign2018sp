//
//  MageScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef MageScene_h
#define MageScene_h

#include <stdio.h>
#include "GameModel.h"
#include "InputController.h"
#include <cugl/cugl.h>

using namespace cugl;

class MageScene : public cugl::Scene{
protected:
    cugl::Size _size;
	int _spellTimer;

    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
	// Actions
	std::shared_ptr<cugl::FadeOut> _dmgFadeOUT;

    std::shared_ptr<cugl::Button> _mageTOcastle;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    
    std::shared_ptr<cugl::Node>  _hex;
	std::shared_ptr<cugl::Rect> _hexCanvas; //can't rotate, so rotate pointer instead
	std::shared_ptr<cugl::PathNode> _spellPath;
	std::vector<cugl::Vec2> _spellPathVertices;
	float _minSpellPathX; //used to reposition _spellPath
	float _minSpellPathY;
    
    std::shared_ptr<cugl::Button> _northWallButton;
    std::shared_ptr<cugl::Button> _northeastWallButton;
    std::shared_ptr<cugl::Button> _southeastWallButton;
    std::shared_ptr<cugl::Button> _southWallButton;
    std::shared_ptr<cugl::Button> _southwestWallButton;
    std::shared_ptr<cugl::Button> _northwestWallButton;
    
    std::shared_ptr<PolygonNode> plain_floor;
    std::shared_ptr<PolygonNode> northWall_floor;
    std::shared_ptr<PolygonNode> northeastWall_floor;
    std::shared_ptr<PolygonNode> southeastWall_floor;
    std::shared_ptr<PolygonNode> southWall_floor;
    std::shared_ptr<PolygonNode> southwestWall_floor;
    std::shared_ptr<PolygonNode> northwestWall_floor;

	std::shared_ptr<cugl::Button> _bombButton;
	std::shared_ptr<cugl::Button> _barrierButton;
	std::shared_ptr<cugl::Button> _freezeButton;

	/** Damage Indicators */
	std::vector<std::shared_ptr<cugl::PolygonNode>> _dmgIndicators;

    // Helpers
	void resetSpellPath();

	bool inHexCanvas(const cugl::Vec2& point);

public:

    int lastSpellFloor;
    // Constructors
    MageScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~MageScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active);
    int switchscene;

	//spell info that gets passed to ballista or oil
	std::string queuedSpell;
	int selectedDir;
    
    void setSide (std::string side);
	void setSpell(std::string spellName);

	/**iterate through gameModel's dmg castle health and apply indicators*/
	void pollDmgIndicators();
};

#endif /* MageScene_h */
