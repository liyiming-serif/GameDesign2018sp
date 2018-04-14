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
    bool networked;
    int _arrowAmmo[2];


	//oil cooldown manager. Ready when == 0 for a particular wall
	std::vector<int> _oilCooldown;

public:

    // Constructors
    GameModel() {};

    bool init();

    //Keeps track of all enemies.
	//{N, NW, SW, S, SE, NE} each have a hashmap of enemy data
    std::vector<std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>> _enemyArrayMaster;
    std::vector<std::vector<std::string>> _enemiesToFreeMaster;

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
		assert(wall < _oilCooldown.size());
		return _oilCooldown[wall];
	}

private:
    std::string getStateChange();

    void updateState(const char* read_byte_buffer);

    void updateState();

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
