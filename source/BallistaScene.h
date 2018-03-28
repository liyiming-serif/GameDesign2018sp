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
    std::set<std::shared_ptr<ArrowModel>> _arrows;
	std::set<std::shared_ptr<ArrowModel>> _arrowsToFree;

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
    
    

    //enemies
    std::vector<std::shared_ptr<EnemyModel>> _enemyArray;

public:
    // Constructors
    BallistaScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    // Destructors
    void dispose();
    ~BallistaScene() {dispose();}

    // Gameplay
    void update(float deltaTime, int direction);

	void updateEnemyModels(int direction);

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
