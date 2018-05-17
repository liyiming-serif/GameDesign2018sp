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
    int _deltaCastleHealth[6]; //delta for networking
	std::array<int, 6> _dmgCastleHealth; //dmg for indicators
    int clock;
    int _noPlayers;
    bool networked;
    bool server;
    int _arrowAmmo[3];
    std::string _enemyChanges;
    int _oilPoured[6];
    int _deltaAmmo[3];
    int _currentRoom;
    
    bool _swipeTutorial;

	//oil cooldown manager. Ready when == 0 for a particular wall
	int _oilCooldown[6];

public:
	/**
	 * NEED TO REFER TO GAMEMODEL OBJECT
	 */
    // Constructors
    GameModel() {};

    bool init();
    bool _winFlag=false;
    bool _loseFlag=true;

    int level;

    std::unordered_map<std::string, bool> _unlockedRooms;

    float _endTime;

    float _currentTime;

    //Keeps track of all enemies.
	//{N, NW, SW, S, SE, NE} each have a hashmap of enemy data
    std::array<std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>, 6> _enemyArrayMaster;
    std::array<std::vector<std::string>, 6> _enemiesToFreeMaster;

    // Destructors
    void dispose();

    ~GameModel() { dispose(); }

    // Gameplay
    void update(float deltaTime);
	//init(); dispose(); without resetting multiplayer
	void reset();

    /**
	 * GETTERS AND SETTERS: DON'T NEED TO REFER TO GAMEMODEL OBJECT
	 */
	float getCurrentTime();

	void setCurrentTime(float time);

	float getEndTime();

	void setEndTime(float time);

	int getLevel(){return level;}

	void setLevel(int inputLevel){level = inputLevel;}

    int getWallHealth(int wall);

    void setWallHealth(int wall, int amt);

    void changeWallHealth(int wall, int damage);

    void addDeltaHealth(int wall, int repair);

    int getDeltaHealth(int wall);

	int getDmgHealth(int wall) { return _dmgCastleHealth[wall]; }

	void resetWallDmg();

    int getPlayerAvatar(int player);

    int getNoPlayers();

    void setNoPlayers(int _noPlayers) {
        this->_noPlayers = _noPlayers;
    }

    void setPlayerID(int ID) {
        this->_playerID = ID;
    }

    int getPlayerID() {
        return this->_playerID;
    }

    void setPlayerAvatar(int player, int avatar);

    void addEnemyChange(std::string name, int damage, int wall);

    void setAmmo(int type, int amt);

    void addDeltaAmmo(int type, int amt);

    bool isNetworked();

    void setNetworked(bool networked);

    bool isServer();

    void setServer(bool server);

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
		assert(wall < 6);
		return _oilCooldown[wall];
	}

	void setOilCooldown(int wall, int amount);

    void setOilPoured(int wall) {
        _oilPoured[wall] = 1;
    }

    void setCurrentRoom(int room);

    void suspendClient();

private:
#if CU_PLATFORM == CU_PLATFORM_ANDROID
    std::string produceStateChangeServer();

    std::string produceStateChangeClient();

    char** ConsumeStateServer();

    char* ConsumeStateClient();

    void updateStateServer(char** ConsumedStates);

    void updateStateClient(const char* read_byte_buffer);
#endif

    char* return_buffer(const std::string& string);

#if CU_PLATFORM == CU_PLATFORM_ANDROID
    //TODO: Make sure JNI wrapper code is correct
    char* consumeState() {
        // Set up parameters for JNI call
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "consumeState",
                                               "()[B");

        jbyteArray array = (jbyteArray) env->CallObjectMethod(activity, method_id);

        if (array == NULL) {
            //CULog("Read Byte Array is Null");
            env->DeleteLocalRef(activity);
            env->DeleteLocalRef(clazz);
            return NULL;
        }
        else {
            //CULog("Read Byte Array is not Null");
            jbyte* buffer = env->GetByteArrayElements(array, NULL);
            jsize size = env->GetArrayLength(array);
            char *byte_buffer = new char[size];

            for(int i = 0; i < size; i++) {
                byte_buffer[i] = buffer[i];
            }
            env->ReleaseByteArrayElements(array, buffer, JNI_ABORT);

            // Free local references
            env->DeleteLocalRef(array);
            env->DeleteLocalRef(activity);
            env->DeleteLocalRef(clazz);
            return byte_buffer;
        }


    }

    //TODO: Make sure JNI wrapper code is correct
    int sendState(char* byte_buffer) {
        // Set up parameters for JNI call
        JNIEnv *env = (JNIEnv *) SDL_AndroidGetJNIEnv();
        jobject activity = (jobject) SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "sendState",
                                               "([B)I");

        if (byte_buffer != NULL) {
            // Call the Java method
            //CULog("Attempting to send across JNI");
            int n=0;
            char* p = byte_buffer;
            while(*p++){
                //CULog("N is currently: %i", n);
                n++;
            } if(n<=0) {
                CULog("sendState: byte_buffer not big enough");
                return NULL;
            }
            else {
                CULog("Size of byte_buffer passed to send State is %i", n);
            }
            jbyteArray arr = env->NewByteArray(n);
            env->SetByteArrayRegion(arr,0,n, (jbyte*)byte_buffer);

            int response = env->CallIntMethod(activity, method_id, arr);

            // Free local references
            //env->ReleaseByteArrayElements(arr, (jbyte*)byte_buffer, JNI_ABORT);
            //free(p);
            env->DeleteLocalRef(arr);
            env->DeleteLocalRef(activity);
            env->DeleteLocalRef(clazz);
            return response;
        }
        else {
            CULog("sendState: attempt to send Null message");
            env->DeleteLocalRef(activity);
            env->DeleteLocalRef(clazz);
            return NULL;
        }
    }
#endif
};

extern GameModel gameModel;

#endif //BUILD_ANDROID_GAMEMODEL_H
