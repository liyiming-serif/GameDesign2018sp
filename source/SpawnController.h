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
#include "EnemyDataModel.h"

class SpawnController {
private:
    /** Whether or not this input is active */
    bool _active;

    //when this equals -1, stop spawning
    int _currSpawnIndex;


protected:
    // variables
    int _totalTime;

    bool _endless;
    int _numPlayers;
    float _spawnChance;
    int _enemyGroup;
    int _numEnemies;
    int _xCoord;
    int _type;
    int _health;
    int _sector;

    //enemy array, specifies air/ground and direction (N, NE, SE, S, SW, NW), will add the rest later
    //for the sector, 0.0 = N, 1.0 = NE, 5.0 = NW (going clockwise)
    //2D vector, each element has {xCoord, yCoord, type, health, spawnTime, sector}
    std::vector<std::vector<float>> _enemyArray;

public:
    //variables

    //CONSTRUCTORS
    SpawnController(){};

    bool init(const std::shared_ptr<cugl::AssetManager>& assets, std::vector<std::vector<float>> initialEnemies);

    bool endlessInit(const std::shared_ptr<cugl::AssetManager>& assets, int numPlayers);



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
