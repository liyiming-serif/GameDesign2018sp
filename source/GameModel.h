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
#include <Array>
#include <jni.h>
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

public:

    // Constructors
    GameModel() {};

    bool init(const std::shared_ptr<cugl::AssetManager> &assets);

    //enemy array, specifies air/ground and direction (N, NE, SE, S, SW, NW), will add the rest later
    std::set<std::shared_ptr<EnemyModel>> _enemyArrayGroundN;
    std::set<std::shared_ptr<EnemyModel>> _enemiesToFree;

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

private:
    std::string getStateChange();

    void updateState(const char* read_byte_buffer);

    char* return_buffer(const std::string& string);

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
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "writeNetwork",
                                               "()V");

        // Call the Java method
        env->CallVoidMethod(activity, method_id);

        // Free local references
        env->DeleteLocalRef(activity);
        env->DeleteLocalRef(clazz);
    }

};

extern GameModel gameModel;

#endif //BUILD_ANDROID_GAMEMODEL_H
