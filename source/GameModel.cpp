//
// Created by Josh on 3/15/2018.
//

#include "GameModel.h"

#define DRAW_SCALE 12
#define GAME_WIDTH 1024

#define OIL_COOLDOWN 420

using namespace cugl;

bool GameModel::init(){
    clock = 0;
    networked = false;

    _arrowAmmo[0] = 30;

    for (int i = 0; i < 6; ++i) {
        _castleHealth[i] = 100;
        _prevCastleHealth[i] = 100;
    }

    _noPlayers = 1;

    return true;
}

void GameModel::dispose() {
	for (int i = 0; i < _enemyArrayMaster.size(); i++) {
		_enemyArrayMaster[i].clear();
	}
	for (int i = 0; i < _enemiesToFreeMaster.size(); i++) {
		_enemiesToFreeMaster[i].clear();
	}
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

	//update enemies
	for (int wall = 0; wall<gameModel._enemyArrayMaster.size(); wall++) {
		for (std::pair<std::string, std::shared_ptr<EnemyDataModel>> enemy : gameModel._enemyArrayMaster[wall]) {
			Vec2 pos = enemy.second->getPos();
			if (pos.y < 85) {
				//enemy collided with wall; mark for deletion
				gameModel._enemiesToFreeMaster[wall].push_back(enemy.first);
				gameModel.changeWallHealth(wall, -9);
			}
			else {
				// move enemy
				enemy.second->setPos(Vec2(pos.x,pos.y-0.5f));
			}
		}
	}

	//delete enemies here to not disrupt iterator
	for (int wall = 0; wall<gameModel._enemiesToFreeMaster.size(); wall++) {
		for (int ekey = 0; ekey < gameModel._enemiesToFreeMaster[wall].size(); ekey++) {
			gameModel._enemyArrayMaster[wall].erase(gameModel._enemiesToFreeMaster[wall][ekey]);
		}
		gameModel._enemiesToFreeMaster[wall].clear();
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

std::string GameModel::getStateChangeServer() {
    std::string _tmpHealthString = "Health";
    int _tmpHealth[6];
    for (int i = 0; i < 6; ++i) {
        _tmpHealth[i] = _castleHealth[i] - _prevCastleHealth[i];
        _tmpHealthString += " " + to_string(_tmpHealth[i]);
    }
    std::string _tmpAvatarString = "Avatar 0";
    return to_string(_playerID) + "|" + _tmpHealthString + "|" + _tmpAvatarString;
}

char** GameModel::ConsumeStateServer() {
    char *tmp[_noPlayers-1];
    for (int i = 0; i < _noPlayers-1; ++i) {
        tmp[i] = ConsumeState(i);
    }
    return tmp;
}

char* GameModel::ConsumeStateClient() {
    return readNetwork();
}

void GameModel::updateStateServer(char** ConsumedStates) {
    char *tmpHealthChanges[_noPlayers-1];
    char *tmpAmmoChanges[_noPlayers-1];
    char *tmpEnemyChanges[_noPlayers-1];
    for (int i = 0; i < _noPlayers-1; ++i) {
        char* copy = strdup(ConsumedStates[i]);
        const char s[2] = "|";
        char *token;
        char* castleHealthToken;
        char* playerInfoToken;
        char* ammoToken;
        char* enemyToken;
        char* sizeToken;
        char* oilToken;
        int section = 0;
        token = strtok(copy, s);
        while (token != NULL) {
            if (section == 0) {
                sizeToken = token;
            } else if (section == 1) {
                castleHealthToken = token;
            } else if (section == 2) {
                enemyToken = token;
            } else if (section == 3) {
                ammoToken = token;
            } else if (section == 4) {
                playerInfoToken = token;
            } else if (section == 5) {
                oilToken = token;
            }
            token = strtok(NULL,s);
            section++;
        }
        tmpAmmoChanges[i] = ammoToken;
        tmpEnemyChanges[i] = enemyToken;
        tmpHealthChanges[i] = castleHealthToken;
    }
    char* subtoken;

    int castleHealthUpdate[6];
    subtoken = strtok(castleHealthToken, " ");
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
