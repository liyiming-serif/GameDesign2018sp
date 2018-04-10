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
#include "EnemyModel.h"

class GameModel {
protected:
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    int _castleHealth[6];
    int _playerAvatars[6];
    int _playerID;
    int _prevCastleHealth[6];
    int _spawnTimer;
    cugl::Size _size;

    int clock;
    bool networked;
    int _arrowAmmo[2];


public:

    // Constructors
    GameModel() {};

    bool init(const std::shared_ptr<cugl::AssetManager> &assets);

    //enemy array, specifies air/ground and direction (N, NE, SE, S, SW, NW), will add the rest later

    std::set<std::shared_ptr<EnemyModel>> _enemyArrayGroundN;
    std::set<std::shared_ptr<EnemyModel>> _enemiesToFree;
    //2D vector, each element has {xCoord, yCoord, type, remaining health}
    std::vector<std::vector<std::vector<float>>> _enemyArrayMaster;
    std::vector<std::vector<int>> _enemiesToFreeMaster;

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
