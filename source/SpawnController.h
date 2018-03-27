//
// SpawnController.h
// Chaos Castle
//
// Controls the enemy spawns
//
// Created by Josh on 3/26/2018.
//

#ifndef BUILD_ANDROID_SPAWNCONTROLLER_H
#define BUILD_ANDROID_SPAWNCONTROLLER_H

#include <cugl/cugl.h>
#include <vector>
#include "GameModel.h"

class SpawnController {
private:
    /** Whether or not this input is active */
    bool _active;

    //when this equals -1, stop spawning
    int _currSpawnIndex;


protected:
    // variables
    int _totalTime;



    //enemy array, specifies air/ground and direction (N, NE, SE, S, SW, NW), will add the rest later
    //2D vector, each element has {xCoord, yCoord, type, health, spawnTime}
    std::vector<std::vector<float>> _enemyArray;

public:
    //variables

    //CONSTRUCTORS
    SpawnController(){};

    bool init(const std::shared_ptr<cugl::AssetManager>& assets);



    //DESTRUCTORS
    ~SpawnController() { dispose(); }

    /**
     * Deactivates this input controller, releasing all listeners.
     * This method will not dispose of the input controller. It can be reused
     * once it is reinitialized.
     */
    void dispose();

    //GAMEPLAY
    bool isActive( ) const { return _active; }

    void update(float deltaTime);

    void clear();


};

#endif //BUILD_ANDROID_SPAWNCONTROLLER_H
