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
    int* _playerAvatars;
    int* _playerRooms;
    int _playerID;
    int _deltaCastleHealth[6];
    int clock;
    int _noPlayers;
    bool networked;
    bool server;
    int _arrowAmmo[3];
    std::string _enemyChanges;
    int _oilPoured[6];
    int _deltaAmmo[3];
    int _currentRoom;

	//oil cooldown manager. Ready when == 0 for a particular wall
	int _oilCooldown[6];

public:
	/**
	 * NEED TO REFER TO GAMEMODEL OBJECT
	 */
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


    /**
	 * GETTERS AND SETTERS: DON'T NEED TO REFER TO GAMEMODEL OBJECT
	 */
    int getWallHealth(int wall);

    void setWallHealth(int wall, int amt);

    void changeWallHealth(int wall, int damage);

    void addDeltaHealth(int wall, int repair);

    int getDeltaHealth(int wall);

    int getPlayerAvatar(int player);

    int getNoPlayers() {
        return _noPlayers;
    }

    void setNoPlayers(int _noPlayers) {
        this->_noPlayers = _noPlayers;
    }

    void setPlayerAvatar(int player, int avatar);

    void addEnemyChange(std::string name, int damage);

    void addDeltaAmmo(int type, int amt);

    bool isNetworked() {
        return networked;
    }

    void setNetworked(bool networked) {
        this->networked = networked;
    }

    bool isServer() {
        return server;
    }

    void setServer(bool server) {
        this->server = server;
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

	int getOilCooldown(int wall) {
		assert(wall < 6);
		return _oilCooldown[wall];
	}

	void setOilCooldown(int wall, int amount);

    void setOilPoured(int wall) {
        _oilPoured[wall] = 1;
    }

    void setCurrentRoom(int room) {
        _currentRoom = room;
    }

private:
    std::string produceStateChangeServer();

    std::string produceStateChangeClient();

    char** ConsumeStateServer();

    char* ConsumeStateClient();

    void updateStateServer(char** ConsumedStates);

    void updateStateClient(const char* read_byte_buffer);

    void aggregateCastleHealth(char** tmpCastleChanges);

    void aggregateAmmo(char** tmpAmmoChanges);

    void aggregateEnemies(char** tmpEnemyChanges);

    char* return_buffer(const std::string& string);

    char* random_buffer_client(int player);

    char* random_buffer_server();

#if CU_PLATFORM == CU_PLATFORM_ANDROID
    //TODO: Make sure JNI wrapper code is correct
    char* consumeState() {
        // Set up parameters for JNI call
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "readNetwork",
                                               "()[B");

        jbyteArray array = (jbyteArray) env->CallObjectMethod(activity, method_id);

        jbyte* buffer = env->GetByteArrayElements(array, NULL);
        jsize size = env->GetArrayLength(array);
        char *byte_buffer = new char[size];

        for(int i = 0; i < size; i++) {
            byte_buffer[i] = buffer[i];
        }
        env->ReleaseByteArrayElements(array, buffer, JNI_ABORT);

        // Free local references
        env->DeleteLocalRef(activity);
        env->DeleteLocalRef(clazz);
        return byte_buffer;
    }

    //TODO: Make sure JNI wrapper code is correct
    int sendState(char* byte_buffer) {
        // Set up parameters for JNI call
        JNIEnv *env = (JNIEnv *) SDL_AndroidGetJNIEnv();
        jobject activity = (jobject) SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "writeNetwork",
                                               "([B)I");

        // Call the Java method
        int response = env->CallIntMethod(activity, method_id, byte_buffer);

        // Free local references
        env->DeleteLocalRef(activity);
        env->DeleteLocalRef(clazz);
        return response;
    }
#endif
};

extern GameModel gameModel;

#endif //BUILD_ANDROID_GAMEMODEL_H
