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

    // Break down all read states into component changes to apply
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
        free(copy);
    }

    // Start aggregating castle health changes by wall
    char* subtoken;
    char* tmpHealthN[_noPlayers-1];
    char* tmpHealthNW[_noPlayers-1];
    char* tmpHealthSW[_noPlayers-1];
    char* tmpHealthS[_noPlayers-1];
    char* tmpHealthSE[_noPlayers-1];
    char* tmpHealthNE[_noPlayers-1];

    for (int i = 0; i < _noPlayers-1; ++i) {
        subtoken = strtok(tmpHealthChanges[i], " ");
        int j = 0;
        while (subtoken != NULL) {
            if (j == 0) {
                tmpHealthN[i] = subtoken;
                subtoken = strtok(NULL, " ");
            } else if (j == 1) {
                tmpHealthNW[i] = subtoken;
                subtoken = strtok(NULL, " ");
            } else if (j == 2) {
                tmpHealthSW[i] = subtoken;
                subtoken = strtok(NULL, " ");
            } else if (j == 3) {
                tmpHealthS[i] = subtoken;
                subtoken = strtok(NULL, " ");
            } else if (j == 4) {
                tmpHealthSE[i] = subtoken;
                subtoken = strtok(NULL, " ");
            } else if (j == 5) {
                tmpHealthNE[i] = subtoken;
                subtoken = strtok(NULL, " ");
            }
            j++;
        }
    }

    // Optimistically apply new health as max of all wall healths (minus deltas), plus the sum of all deltas
    int castleHealthUpdate[6];
    char* tottoken;
    char* deltoken;

    // North Wall
    int currmax = 0;
    int deltasum = 0;
    for (int i = 0; i < _noPlayers-1; ++i) {
        tottoken = strtok(tmpHealthN[i], ":");
        deltoken = strtok(NULL, ":");
        int repairdelt = std::stoi(deltoken);
        int currplayerhealth = std::stoi(tottoken) - repairdelt;
        if (currplayerhealth > currmax) {
            currmax = currplayerhealth;
        }
        deltasum += repairdelt;
    }
    if (getWallHealth(0) > currmax) {
        currmax = getWallHealth(0);
    }
    castleHealthUpdate[0] = deltasum + currmax;

    // NorthWest Wall
    currmax = 0;
    deltasum = 0;
    for (int i = 0; i < _noPlayers-1; ++i) {
        tottoken = strtok(tmpHealthNW[i], ":");
        deltoken = strtok(NULL, ":");
        int currplayerhealth = std::stoi(tottoken);
        int repairdelt = std::stoi(deltoken);
        if (currplayerhealth > currmax) {
            currmax = currplayerhealth;
        }
        deltasum += repairdelt;
    }
    if (getWallHealth(1) > currmax) {
        currmax = getWallHealth(1);
    }
    castleHealthUpdate[1] = deltasum + currmax;

    //SouthWest Wall
    currmax = 0;
    deltasum = 0;
    for (int i = 0; i < _noPlayers-1; ++i) {
        tottoken = strtok(tmpHealthSW[i], ":");
        deltoken = strtok(NULL, ":");
        int currplayerhealth = std::stoi(tottoken);
        int repairdelt = std::stoi(deltoken);
        if (currplayerhealth > currmax) {
            currmax = currplayerhealth;
        }
        deltasum += repairdelt;
    }
    if (getWallHealth(2) > currmax) {
        currmax = getWallHealth(2);
    }
    castleHealthUpdate[2] = deltasum + currmax;

    //South Wall
    currmax = 0;
    deltasum = 0;
    for (int i = 0; i < _noPlayers-1; ++i) {
        tottoken = strtok(tmpHealthS[i], ":");
        deltoken = strtok(NULL, ":");
        int currplayerhealth = std::stoi(tottoken);
        int repairdelt = std::stoi(deltoken);
        if (currplayerhealth > currmax) {
            currmax = currplayerhealth;
        }
        deltasum += repairdelt;
    }
    if (getWallHealth(3) > currmax) {
        currmax = getWallHealth(3);
    }
    castleHealthUpdate[3] = deltasum + currmax;

    // SouthEast Wall
    currmax = 0;
    deltasum = 0;
    for (int i = 0; i < _noPlayers-1; ++i) {
        tottoken = strtok(tmpHealthSE[i], ":");
        deltoken = strtok(NULL, ":");
        int currplayerhealth = std::stoi(tottoken);
        int repairdelt = std::stoi(deltoken);
        if (currplayerhealth > currmax) {
            currmax = currplayerhealth;
        }
        deltasum += repairdelt;
    }
    if (getWallHealth(4) > currmax) {
        currmax = getWallHealth(4);
    }
    castleHealthUpdate[4] = deltasum + currmax;

    // NorthEast Wall
    currmax = 0;
    deltasum = 0;
    for (int i = 0; i < _noPlayers-1; ++i) {
        tottoken = strtok(tmpHealthNE[i], ":");
        deltoken = strtok(NULL, ":");
        int currplayerhealth = std::stoi(tottoken);
        int repairdelt = std::stoi(deltoken);
        if (currplayerhealth > currmax) {
            currmax = currplayerhealth;
        }
        deltasum += repairdelt;
    }
    if (getWallHealth(5) > currmax) {
        currmax = getWallHealth(5);
    }
    castleHealthUpdate[5] = deltasum + currmax;

    // Apply castle health changes
    for (int i = 0; i < 6; ++i) {
        changeWallHealth(i, castleHealthUpdate[i]);
        _prevCastleHealth[i] = _castleHealth[i];
    }

    // Sum all arrow changes
    char* arrowsubtoken;
    int deltaArrow1 = 0;
    int deltaArrow2 = 0;
    int deltaArrow3 = 0;

    for (int i = 0; i < _noPlayers-1; ++i) {
        arrowsubtoken = strtok(tmpAmmoChanges[i], " ");
        int j = 0;
        while (arrowsubtoken != NULL) {
            if (j == 0) {
                deltaArrow1 += std::stoi(arrowsubtoken);
                arrowsubtoken = strtok(NULL, " ");
            } else if (j == 1) {
                deltaArrow2 += std::stoi(arrowsubtoken);
                arrowsubtoken = strtok(NULL, " ");
            } else if (j == 2) {
                deltaArrow3 += std::stoi(arrowsubtoken);
                arrowsubtoken = strtok(NULL, " ");
            }
            j++;
        }
    }
    setArrowAmmo(0, getArrowAmmo(0)+deltaArrow1);
    setArrowAmmo(1, getArrowAmmo(1)+deltaArrow2);
    setArrowAmmo(2, getArrowAmmo(2)+deltaArrow3);


    // Apply all damage created by all players to the respective monsters
    char* enemySubToken;
    std::string enemyName;
    char* enemyDamage;
    for (int i = 0; i < _noPlayers-1; ++i) {
        enemySubToken = strtok(tmpEnemyChanges[i], " :");
        while (enemySubToken != NULL) {
            strcpy(enemyName, enemySubToken);
            enemySubToken = strtok(NULL, " :");
            strcpy(enemyDamage, enemySubToken);
            for (int j = 0; j<6; ++j) {
                std::unordered_map<std::string,std::shared_ptr<EnemyDataModel>>::const_iterator got = _enemyArrayMaster[j].find (enemyName);

                if ( got == _enemyArrayMaster[j].end() )
                    std::cout << "not found";
                else {
                    std::shared_ptr<EnemyDataModel> thisEnemy = got->second;
                    thisEnemy.setHealth(thisEnemy.getHealth() + std::stoi(enemyDamage));
                }
            }
            enemySubToken = strtok(NULL, " :");
        }
    }
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
