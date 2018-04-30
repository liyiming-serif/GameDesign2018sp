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
	int _oilCooldown[6];

public:
	/**
	 * NEED TO REFER TO GAMEMODEL OBJECT
	 */
    // Constructors
    GameModel() {};

    bool init();

    int level;

    std::unordered_map<std::string, bool> _unlockedRooms;

    //Keeps track of all enemies.
	//{N, NW, SW, S, SE, NE} each have a hashmap of enemy data
    std::array<std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>, 6> _enemyArrayMaster;
    std::array<std::vector<std::string>, 6> _enemiesToFreeMaster;

    // Destructors
    void dispose();

    ~GameModel() { dispose(); }

    // Gameplay
    void update(float deltaTime);


    /**
	 * GETTERS AND SETTERS: DON'T NEED TO REFER TO GAMEMODEL OBJECT
	 */
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
        if (amount > 909) {
            _arrowAmmo[type] = 999;
        }
        else if (amount < 0) {
            _arrowAmmo[type] = 0;
        }
        else {
            _arrowAmmo[type] = amount;
        }
    }

	int getOilCooldown(int wall) {
		return _oilCooldown[wall];
	}

	void setOilCooldown(int wall, int amount);

private:
    std::string getStateChange();

    void updateState(const char* read_byte_buffer);

    void updateState();

    char* return_buffer(const std::string& string);

    char* random_buffer();

#if CU_PLATFORM == CU_PLATFORM_ANDROID
//    //TODO: Make sure JNI wrapper code is correct
//    char* readNetwork() {
//        // Set up parameters for JNI call
//        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
//        jobject activity = (jobject)SDL_AndroidGetActivity();
//
//        jclass clazz(env->GetObjectClass(activity));
//        jmethodID method_id = env->GetMethodID(clazz, "readNetwork",
//                                               "()V");
//
//        // Call the Java method
//        env->CallVoidMethod(activity, method_id);
//
//        // Free local references
//        env->DeleteLocalRef(activity);
//        env->DeleteLocalRef(clazz);
//    }
//
//    //TODO: Make sure JNI wrapper code is correct
//    void writeNetwork(char* byte_buffer) {
//        // Set up parameters for JNI call
//        JNIEnv *env = (JNIEnv *) SDL_AndroidGetJNIEnv();
//        jobject activity = (jobject) SDL_AndroidGetActivity();
//
//        jclass clazz(env->GetObjectClass(activity));
//        jmethodID method_id = env->GetMethodID(clazz, "writeNetwork",
//                                               "()V");
//
//        // Call the Java method
//        env->CallVoidMethod(activity, method_id);
//
//        // Free local references
//        env->DeleteLocalRef(activity);
//        env->DeleteLocalRef(clazz);
//    }
#endif
};

extern GameModel gameModel;

#endif //BUILD_ANDROID_GAMEMODEL_H
