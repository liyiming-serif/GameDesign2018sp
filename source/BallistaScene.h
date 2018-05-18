//
// Created by Yiming on 2/25/2018.
//

#ifndef BUILD_ANDROID_BALLISTASCENE_H
#define BUILD_ANDROID_BALLISTASCENE_H
#include <cugl/cugl.h>
#include "ArrowModel.h"
#include "BallistaModel.h"
#include "InputController.h"
#include "EnemyModel.h"
#include "EnemyDataModel.h"
#include "GameModel.h"
#include <set>
#include <vector>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>

class BallistaScene : public cugl::Scene{
protected:
    cugl::Size _size;

    int _direction;

    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;

    // Physics manager
    std::shared_ptr<cugl::ObstacleWorld> _world;

    // Memory Managers
	//arrows
    std::set<std::shared_ptr<ArrowModel>> _arrows;
	std::set<std::shared_ptr<ArrowModel>> _arrowsToFree;
	//enemies
	std::unordered_map<std::string,std::shared_ptr<EnemyModel>> _enemyArray;
	std::set<std::shared_ptr<EnemyModel>> _enemiesToFree;

	// Actions
	std::shared_ptr<cugl::FadeOut> _dmgFadeOUT;
	std::shared_ptr<cugl::Animate> _spellAnim;
	std::shared_ptr<cugl::Animate> _spellAnim2;

    // Models
    std::shared_ptr<BallistaModel> _ballista;
    std::shared_ptr<cugl::PolygonNode> _background;
    std::shared_ptr<cugl::PolygonNode> _ballistaTurret_GREEN;
    std::shared_ptr<cugl::PolygonNode> _ballistaTurret_YELLOW;
    std::shared_ptr<cugl::PolygonNode> _ballistaTurret_ORANGE;
    std::shared_ptr<cugl::PolygonNode> _ballistaTurret_RED;
    std::shared_ptr<cugl::Button> _ballistaTOcastle;
    
    std::shared_ptr<cugl::PolygonNode> N_compass;
    std::shared_ptr<cugl::PolygonNode> NE_compass;
    std::shared_ptr<cugl::PolygonNode> NW_compass;
    std::shared_ptr<cugl::PolygonNode> S_compass;
    std::shared_ptr<cugl::PolygonNode> SE_compass;
    std::shared_ptr<cugl::PolygonNode> SW_compass;
    
    // Dragging UI
	std::shared_ptr<cugl::Texture> _dpOrange;
	std::shared_ptr<cugl::Texture> _dlOrange;
	std::shared_ptr<cugl::Texture> _dpBlue;
	std::shared_ptr<cugl::Texture> _dlBlue;

	std::shared_ptr<cugl::AnimationNode> _dragStart;
	std::shared_ptr<cugl::AnimationNode> _dragEnd;
	std::shared_ptr<cugl::PolygonNode> _dragLine;

	// Spell Animations
	std::shared_ptr<cugl::AnimationNode> _bombAnim;
	std::shared_ptr<cugl::AnimationNode> _freezeAnim;

    //text
    std::shared_ptr<cugl::Label> _ammoText;
    
    //tutorial
    std::shared_ptr<cugl::PolygonNode> _ballista_swipe;
    std::shared_ptr<cugl::PolygonNode> _ballista_tap;
    bool _swipeTutorial;
    std::shared_ptr<cugl::Label> _lowAmmoText;
    std::shared_ptr<cugl::Label> _noAmmoText;

	//helper method for translating enemy y coords
	bool inRange(float y);
	float calcY(float y);

	/** Damage Indicators */
	std::vector<std::shared_ptr<cugl::PolygonNode>> _dmgIndicators;

	//Spell filter
	std::shared_ptr<cugl::PolygonNode> _spellFilter;

public:
    // Constructors
    BallistaScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    // Destructors
    void dispose();
    ~BallistaScene() {dispose();}

    // Gameplay
    void update(float deltaTime, int direction);

	void updateEnemyModels(float deltaTime, int direction);

	void animateSpells(const std::string & spellName);

    //Pause or Resume
    void setActive(bool active, int direction, std::string spellName="none");
    int switchscene;

	//Call to activate collisions for this world
	void activateWorldCollisions();

	//Collision Callbacks
	void beginContact(b2Contact* contact);
    
    //Sets the compass image based on the direction input
    void setCompass (int direction);
    
    //Sets the wall image based on the wall health input
    void setWall (int direction);

	/**iterate through gameModel's dmg castle health and apply indicators*/
	void pollDmgIndicators();
    
    int _shots;
    int _exitCount;
};

#endif //BUILD_ANDROID_BALLISTASCENE_H
