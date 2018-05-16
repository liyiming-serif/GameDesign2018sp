//
//  OilScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef OilScene_h
#define OilScene_h

#include <stdio.h>
#include "InputController.h"
#include "GameModel.h"
#include "OilModel.h"
#include "EnemyModel.h"
#include <cugl/cugl.h>

class OilScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
	// Physics manager
	std::shared_ptr<cugl::ObstacleWorld> _world;

	// Memory managers
	//enemies
	std::unordered_map<std::string, std::shared_ptr<EnemyModel>> _enemyArray;
	std::set<std::shared_ptr<EnemyModel>> _enemiesToFree;

	// Actions
	std::shared_ptr<cugl::FadeOut> _dmgFadeOUT;

	//models
    std::shared_ptr<cugl::Button> _oilTOcastle;
	std::shared_ptr<OilModel> _oil;
    std::shared_ptr<cugl::PolygonNode> _background;
    std::shared_ptr<cugl::PolygonNode> _wall_GREEN;
    std::shared_ptr<cugl::PolygonNode> _wall_YELLOW;
    std::shared_ptr<cugl::PolygonNode> _wall_ORANGE;
    std::shared_ptr<cugl::PolygonNode> _wall_RED;
    
    std::shared_ptr<cugl::PolygonNode> N_compass;
    std::shared_ptr<cugl::PolygonNode> NE_compass;
    std::shared_ptr<cugl::PolygonNode> NW_compass;
    std::shared_ptr<cugl::PolygonNode> S_compass;
    std::shared_ptr<cugl::PolygonNode> SE_compass;
    std::shared_ptr<cugl::PolygonNode> SW_compass;
    
    std::shared_ptr<cugl::PolygonNode> _tilt;
    bool _tiltTutorial;

	std::shared_ptr<cugl::AnimationNode> _deluge;
	float _delugeFrame;
    
	//helper functions for translating enemy y coords
	bool inRange(float y);
	float calcY(float y);
    
	/** Damage Indicators */
	std::vector<std::shared_ptr<cugl::PolygonNode>> _dmgIndicators;

public:
    // Constructors
    OilScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~OilScene() {dispose();}
    
    // Gameplay
    void update(float timestep, int direction);
	void updateEnemyModels(float timestep, int direction);

    //Pause or Resume
    void setActive(bool active, int direction);
    int switchscene;
    
    //Sets the compass image based on the direction input
    void setCompass (int direction);
    
    //Sets the wall image based on the wall health input
    void setWall (int direction);

	/**iterate through gameModel's dmg castle health and apply indicators*/
	void pollDmgIndicators();
    
    int _tiltCount;
};


#endif /* OilScene_h */
