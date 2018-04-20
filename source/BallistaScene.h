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

    // Models
    std::shared_ptr<BallistaModel> _ballista;
    std::shared_ptr<cugl::PolygonNode> _background;
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

    //text
    std::shared_ptr<cugl::Label> _ammoText;

	//helper method for translating enemy y coords
	bool inRange(float y);
	float calcY(float y);

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

    //Pause or Resume
    void setActive(bool active, int direction);
    int switchscene;

	//Call to activate collisions for this world
	void activateWorldCollisions();

	//Collision Callbacks
	void beginContact(b2Contact* contact);
    
    //Sets the compass image based on the direction input
    void setCompass (int direction);

};

#endif //BUILD_ANDROID_BALLISTASCENE_H
