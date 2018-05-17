//
// Created by Yiming on 3/2/2018.
//

#ifndef BUILD_ANDROID_LOOKOUTSCENE_H
#define BUILD_ANDROID_LOOKOUTSCENE_H

#include "GameModel.h"
#include "EnemyModel.h"
#include "InputController.h"
#include <cugl/cugl.h>
#include <vector>

class LookoutScene : public cugl::Scene{
protected:
    cugl::Size _size;

    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;

	// Actions
	std::shared_ptr<cugl::FadeOut> _dmgFadeOUT;

    std::shared_ptr<cugl::Button> _lookoutTOcastle;

    std::shared_ptr<cugl::PolygonNode> _background;
    std::shared_ptr<cugl::PolygonNode> _progressBar;

	// Enemy marker textures
	std::shared_ptr<cugl::Texture> _enemyIcon;
	std::shared_ptr<cugl::Texture> _flyingIcon;
	std::shared_ptr<cugl::Texture> _warriorIcon;
	std::shared_ptr<cugl::Texture> _reaperIcon;
	std::shared_ptr<cugl::Texture> _berserkerIcon;

    std::vector<std::shared_ptr<cugl::Node>> _enemyMarkers;
    
	/** Damage Indicators */
	std::vector<std::shared_ptr<cugl::PolygonNode>> _dmgIndicators;

public:
    // Constructors
    LookoutScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    // Destructors
    void dispose();
    ~LookoutScene() {dispose();}

    // Gameplay
    void update(float timestep);

    //Pause or Resume
    void setActive(bool active);
    int switchscene;
    
    float _distance; 

	/**iterate through gameModel's dmg castle health and apply indicators*/
	void pollDmgIndicators();
};
#endif //BUILD_ANDROID_LOOKOUTSCENE_H
