//
// Created by Josh on 3/15/2018.
//


#include <cugl/cugl.h>
#include <stdlib.h>
#include "GameModel.h"
#include <cmath>
#include "CastleApp.h"

#define DRAW_SCALE 12
#define GAME_WIDTH 1024

#define OIL_COOLDOWN 420

using namespace cugl;

bool GameModel::init(const std::shared_ptr<AssetManager>& assets){
    // Set display size
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    clock = 0;
    networked = true;

    _arrowAmmo[0] = 30;

    _assets = assets;

    _spawnTimer = 360;

    int sum = 10;

    for (int i = 0; i < 6; ++i) {
        _castleHealth[i] = sum;
        _prevCastleHealth[i] = sum;
        sum +=10;
    }

    
    

    return true;
}

void GameModel::dispose() {
    _assets = nullptr;
    _enemyArrayMaster.clear();
    _enemiesToFreeMaster.clear();
}

void GameModel::update(float deltaTime){

    if (networked) {
        if (clock == 300) {
            const char *write_byte_buffer = return_buffer(getStateChange());
            //TODO: Write to network
            CULog("State Change %s \n", write_byte_buffer);
            //TODO: Read from network
            //const char *read_byte_buffer = readNetwork();
            const char *read_byte_buffer = random_buffer();
            CULog("RandNet State Change %s \n", read_byte_buffer);
            //TODO: Agree on state changes
            updateState(read_byte_buffer);
            clock = 0;
            delete[] write_byte_buffer;
            delete[] read_byte_buffer;
        } else {
            clock++;
        }
    }
}

int GameModel::getWallHealth(int wall) {
    return _castleHealth[wall];
}

//positive is healing, negative is damage
void GameModel::changeWallHealth(int wall, int amt) {
	//cap health between 0 and 100
	if (_castleHealth[wall] + amt > 100) {
		_castleHealth[wall] = 100;
	}
	else if (_castleHealth[wall] + amt < 0) {
		_castleHealth[wall] = 0;
	}
	else {
		_castleHealth[wall] += amt;
	}
}

int GameModel::getPlayerAvatar(int player) {
    return _playerAvatars[player];
}

void GameModel::setPlayerAvatar(int player, int avatar) {
    _playerAvatars[player] = avatar;
}

std::string GameModel::getStateChange() {
    std::string _tmpHealthString = "Health";
    int _tmpHealth[6];
    for (int i = 0; i < 6; ++i) {
        _tmpHealth[i] = _castleHealth[i] - _prevCastleHealth[i];
        _tmpHealthString += " " + to_string(_tmpHealth[i]);
    }
    std::string _tmpAvatarString = "Avatar 0";
    return to_string(_playerID) + "|" + _tmpHealthString + "|" + _tmpAvatarString;
}

void GameModel::updateState(const char* read_byte_buffer) {
    char* copy = strdup(read_byte_buffer);
    const char s[2] = "|";
    char *token;
    char* castleHealthToken;
    char* playerIDToken;
    char* playerAvatarToken;
    char* ammoToken;
    char* enemyToken;
    int section = 0;
    token = strtok(copy, s);
    while (token != NULL) {
        if (section == 0) {
            playerIDToken = token;
        } else if (section == 1) {
            castleHealthToken = token;
        } else if (section == 2) {
            playerAvatarToken = token;
        } else if (section == 3) {
            ammoToken = token;
        } else if (section == 4) {
            enemyToken = token;
        }
        token = strtok(NULL,s);
        section++;
    }


    int castleHealthUpdate[6];
    token = strtok(castleHealthToken, " ");
    int i = 0;
    while (token != NULL) {
        if (i == 0) {
            token = strtok(NULL, " ");
        } else {
            castleHealthUpdate[i-1] = std::stoi(token);
            token = strtok(NULL, " ");
        }
        i++;
    }
    for (int i = 0; i < 6; ++i) {
        changeWallHealth(i, castleHealthUpdate[i]);
        _prevCastleHealth[i] = _castleHealth[i];
    }
    free(copy);
}

char* GameModel::random_buffer() {
    std::string tmp_string = "0|Health";
    for (int i = 0; i < 6; ++i) {
        tmp_string += " " + to_string(rand()%20 - 10);
    }
    tmp_string += "|Avatar 0";
    return return_buffer(tmp_string);
}

char* GameModel::return_buffer(const std::string& string)
{
    char* return_string = new char[string.length() + 1];
    strcpy(return_string, string.c_str());
    return return_string;
}

#if CU_PLATFORM == CU_PLATFORM_ANDROID
JNIEXPORT char* JNICALL Java_edu_cornell_gdiac_chaoscastle_ChaosCastle_readNetwork
        (JNIEnv *env, jclass clazz, jbyteArray array) {
    jbyte* buffer = env->GetByteArrayElements(array, NULL);
    jsize size = env->GetArrayLength(array);
    char *byte_buffer = new char[size];

    for(int i = 0; i < size; i++) {
        byte_buffer[i] = buffer[i];
    }
    env->ReleaseByteArrayElements(array, buffer, JNI_ABORT);
    return byte_buffer;
}
#endif
