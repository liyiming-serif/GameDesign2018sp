//
// GameModel.h
//
// Created by Josh on 3/15/2018.
// stores enemy positions in vectors, moves enemies
//

#ifndef BUILD_ANDROID_GAMEMODEL_H
#define BUILD_ANDROID_GAMEMODEL_H

#include <cugl/cugl.h>
#include <set>

#if CU_PLATFORM == CU_PLATFORM_ANDROID
#include <jni.h>
#endif
#include <array>
#include <vector>
#include "EnemyDataModel.h"
#include "EnemyModel.h"

class GameModel {
protected:
    int _castleHealth[6];
    int _playerAvatars[6];
    int _playerID;
    int _prevCastleHealth[6];

    int clock;
    int _noPlayers;
    bool networked;
    int _arrowAmmo[2];


	//oil cooldown manager. Ready when == 0 for a particular wall
	int _oilCooldown[6];

public:

    // Constructors
    GameModel() {};

    bool init();

    //Keeps track of all enemies.
	//{N, NW, SW, S, SE, NE} each have a hashmap of enemy data
    std::array<std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>, 6> _enemyArrayMaster;
    std::array<std::vector<std::string>, 6> _enemiesToFreeMaster;

    // Destructors
    void dispose();

    ~GameModel() { dispose(); }

    // Gameplay
    void update(float deltaTime);

    // Physics manager
    //std::shared_ptr<cugl::ObstacleWorld> _world;

    int getWallHealth(int wall);

    void changeWallHealth(int wall, int damage);

    int getPlayerAvatar(int player);

    void setPlayerAvatar(int player, int avatar);

    bool getNetworked() {
        return networked;
    }

    void setNetworked(bool networked) {
        this->networked = networked;
    }
    int getArrowAmmo(int type) {
        return _arrowAmmo[type];
    }

    void setArrowAmmo(int type, int amount) {
        if (amount > 99) {
            _arrowAmmo[type] = 99;
        }
        else if (amount < 0) {
            _arrowAmmo[type] = 0;
        }
        else {
            _arrowAmmo[type] = amount;
        }
    }

	void resetOilCooldown(int wall);

	int getOilCooldown(int wall) {
		assert(wall < 6);
		return _oilCooldown[wall];
	}

private:
    std::string getStateChangeServer();

    std::string getStateChangeClient();

    char** ConsumeStateServer();

    char* ConsumeStateClient();

    void updateStateServer(char** ConsumedStates);

    void updateStateClient(const char* read_byte_buffer);

    void aggregateCastleHealth(char** tmpCastleChanges);

    void aggregateAmmo(char** tmpAmmoChanges);

    void aggregateEnemies(char** tmpEnemyChanges);

    char* return_buffer(const std::string& string);

    char* random_buffer();

#if CU_PLATFORM == CU_PLATFORM_ANDROID
    //TODO: Make sure JNI wrapper code is correct
    char* readNetwork() {
        // Set up parameters for JNI call
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "readNetwork",
                                               "()V");

        // Call the Java method
        env->CallVoidMethod(activity, method_id);

        // Free local references
        env->DeleteLocalRef(activity);
        env->DeleteLocalRef(clazz);
    }

    //TODO: Make sure JNI wrapper code is correct
    void writeNetwork(char* byte_buffer) {
        // Set up parameters for JNI call
        JNIEnv *env = (JNIEnv *) SDL_AndroidGetJNIEnv();
        jobject activity = (jobject) SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "writeNetwork",
                                               "()V");

        // Call the Java method
        env->CallVoidMethod(activity, method_id);

        // Free local references
        env->DeleteLocalRef(activity);
        env->DeleteLocalRef(clazz);
    }
#endif
};

extern GameModel gameModel;

#endif //BUILD_ANDROID_GAMEMODEL_H
