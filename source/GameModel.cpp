//
// Created by Josh on 3/15/2018.
//

#include "GameModel.h"

#define DRAW_SCALE 12
#define GAME_WIDTH 1024
#define LOOKOUT_ROOM 0

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
    _playerAvatars = new int[_noPlayers];
    _playerRooms = new int[_noPlayers];
    for (int i = 0; i < _noPlayers; ++i) {
        _playerRooms[i] = 0;
    }
    isServer = true;
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
        if (clock == 200) {
            if (isServer) {
                //TODO: Read from network
                //const char *read_byte_buffer = readNetwork();
                char **read_buffers = ConsumeStateServer();
                const char *read_byte_buffer = random_buffer();
                CULog("RandNet State Change %s \n", read_byte_buffer);
                updateStateServer(read_buffers);

                const char *write_byte_buffer = return_buffer(produceStateChangeServer());
                //TODO: Write to network
                CULog("State Change %s \n", write_byte_buffer);

                clock = 0;
                delete[] write_byte_buffer;
                delete[] read_buffers;
            }
            else {
                const char *write_byte_buffer = return_buffer(produceStateChangeClient());
                //TODO: Write to network
                CULog("State Change %s \n", write_byte_buffer);
                //TODO: Read from network
                //const char *read_byte_buffer = readNetwork();
                char *read_buffer = ConsumeStateClient();
                const char *read_byte_buffer = random_buffer();
                CULog("RandNet State Change %s \n", read_byte_buffer);
                updateStateClient(read_buffer);
                clock = 0;
                delete[] write_byte_buffer;
                delete[] read_buffer;
            }
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

void GameModel::setWallHealth(int wall, int amt) {
    _castleHealth[wall] = amt;
}

int GameModel::getPlayerAvatar(int player) {
    return _playerAvatars[player];
}

void GameModel::setPlayerAvatar(int player, int avatar) {
    _playerAvatars[player] = avatar;
}

std::string GameModel::produceStateChangeServer() {
    std::string _tmpHealthString = "";
    std::string _tmpAmmoString = "";
    std::string _tmpEnemyString = "";
    std::string _tmpPlayerString = "";
    std::string _tmpOilString = "";

    for (int i = 0; i < 6; ++i) {
        _tmpHealthString+= to_string(_castleHealth[i]) + " ";
        for (auto it = _enemyArrayMaster[i].begin(); it != _enemyArrayMaster[i].end(); ++it) {
            _tmpEnemyString += it->second.toString() + " ";
        }
        _tmpOilString += to_string(_oilCooldown[i]) + " ";
    }

    for (int i = 0; i < 3; ++i) {
        _tmpAmmoString += to_string(_arrowAmmo[i]) + " ";
    }

    for (int i = 0; i < _noPlayers; ++i) {
        _tmpPlayerString += to_string(_playerAvatars[i]) + ":" + to_string(_playerRooms[i]) + " ";
    }

    _tmpHealthString.pop_back();
    _tmpAmmoString.pop_back();
    _tmpEnemyString.pop_back();
    _tmpPlayerString.pop_back();
    _tmpOilString.pop_back();

    //TODO: Game recovery state (currserver, gameclock, etc)
    std::string premessage = _tmpHealthString + "|" + _tmpEnemyString + "|" +
            _tmpAmmoString + "|" + _tmpPlayerString + "|" + _tmpOilString;

    int premessageSize = premessage.length();
    int postmessageSize = premessageSize + to_string(premessageSize).length();
    int totalmessageSize = premessageSize + to_string(postmessageSize).length();
    return to_string(totalmessageSize) + "|" + premessage;
}

std::string GameModel::produceStateChangeClient() {

}

char** GameModel::ConsumeStateServer() {
    char *tmp[_noPlayers-1];
    for (int i = 0; i < _noPlayers-1; ++i) {
        tmp[i] = ConsumeState();
    }
    return tmp;
}

char* GameModel::ConsumeStateClient() {
    return ConsumeState();
}

void GameModel::updateStateServer(char** ConsumedStates) {

    // Break down all read states into component changes to apply
    char *tmpHealthChanges[_noPlayers-1];
    char *tmpAmmoChanges[_noPlayers-1];
    char *tmpEnemyChanges[_noPlayers-1];
    char *tmpPlayerChanges[_noPlayers-1];
    char *tmpOilChanges[_noPlayers-1];
    for (int i = 0; i < _noPlayers-1; ++i) {
        char* copy = strdup(ConsumedStates[i]);
        const char s[2] = "|";
        char *token;
        char* castleHealthToken;
        char* ammoToken;
        char* enemyToken;
        char* sizeToken;
        char* oilToken;
        char* playerInfoToken;
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
        tmpPlayerChanges[i] = playerInfoToken;
        tmpOilChanges[i] = oilToken;
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
                std::unordered_map<std::string,std::shared_ptr<EnemyDataModel>>::iterator got = _enemyArrayMaster[j].find (enemyName);

                if ( got == _enemyArrayMaster[j].end() )
                    std::cout << "not found";
                else {
                    EnemyDataModel thisEnemy = got->second;
                    thisEnemy.setHealth(thisEnemy.getHealth() + std::stoi(enemyDamage));
                }
            }
            enemySubToken = strtok(NULL, " :");
        }
    }

    // Aggregate player info changes; if players occupy the same room and it's not overworld, kick player with higher ID
    for (int i = 0; i < _noPlayers-1; ++i) {
        char *roomToken = strtok(tmpPlayerChanges[i], ":");
        int playerID = std::stoi(roomToken);
        roomToken = strtok(NULL, ":");
        int playerRoom = std::stoi(roomToken);
        bool occupied = false;
        for (int j = 0; j < _noPlayers; ++j) {
            if (playerRoom == _playerRooms[j]) {
                occupied = true;
            }
        }
        if (occupied) {
            _playerRooms[playerID] = 0;
        } else {
            _playerRooms[playerID] = playerRoom;
        }
    }

    // Start oilcooldowns for all walls that have poured
    for (int i = 0; i < _noPlayers-1; ++i) {
        char* pourToken = strtok(tmpOilChanges[i], " ");
        int section = 0;
        while (pourToken != NULL) {
            int poured = std::stoi(pourToken);

            if (poured == 1 && _oilCooldown[section] == 0) {
                _oilCooldown[section] = 420;
            }
            pourToken = strtok(NULL, " ");
            section++;
        }
    }

}

void GameModel::updateStateClient(const char *ConsumedState) {
    //Get the total state message from the server
    char* copy = strdup(ConsumedState);
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
    // Starting with castle health, reset all gamestate to server state
    char* wallHealthToken = strtok(castleHealthToken, " ");
    section = 0;
    while (wallHealthToken != NULL) {
        _castleHealth[section] = std::stoi(wallHealthToken);
        wallHealthToken = strtok(NULL, " ");
        section++;
    }

    // Next, update all existing enemies with new health/pos, and spawn new enemies according to server state
    char* enemyPosX;
    char* enemyPosY;
    char* enemyWall;
    char* enemyType;
    char* enemyHealth;
    char* enemyName;
    char* enemyDataToken = strtok(enemyToken, " ");
    section = 0;
    int subsection = 0;
    while (enemyDataToken != NULL) {
        char * enemyDataPoint = strtok(enemyDataToken, ":");
        while (enemyDataPoint != NULL) {
            if (subsection == 0) {
                enemyName = enemyDataPoint;
            } else if (subsection == 1) {
                enemyHealth = enemyDataPoint;
            } else if (subsection == 2) {
                enemyPosX = enemyDataPoint;
            } else if (subsection == 3) {
                enemyPosY = enemyDataPoint;
            } else if (subsection == 4) {
                enemyType = enemyDataPoint;
            } else if (subsection == 5) {
                enemyWall = enemyDataPoint;
            }
            enemyDataPoint = strtok(NULL, ":");
            subsection++;
        }
        std::unordered_map<std::string,std::shared_ptr<EnemyDataModel>>::iterator got = _enemyArrayMaster[std::stoi(enemyWall)].find (enemyName);

        if ( got == _enemyArrayMaster[std::stoi(enemyWall)].end() ) {
            // Allocate a new EnemyDataModel in memory
            std::shared_ptr<EnemyDataModel> e =
                    EnemyDataModel::alloc(enemyName,std::stoi(enemyHealth),
                                          Vec2(std::stof(enemyPosX), std::stof(enemyPosY)),
                                          std::stoi(enemyType), std::stoi(enemyWall));

            if (e != nullptr) {
                _enemyArrayMaster[std::stoi(enemyWall)][enemyName] = e;
            }
        }
        else {
            <EnemyDataModel> thisEnemy = got->second;
            thisEnemy.setHealth(thisEnemy.getHealth() + std::stoi(enemyHealth));
            thisEnemy.setPos(Vec2(std::stof(enemyPosX), std::stof(enemyPosY)));
        }
        enemyDataToken = strtok(NULL, " ");
        section++;
    }

    // Update arrow ammo with new values
    char* arrowToken = strtok(ammoToken, " ");
    section = 0;
    while (arrowToken != NULL) {
        _arrowAmmo[section] = std::stoi(arrowToken);
        arrowToken = strtok(NULL, " ");
        section++;
    }

    // Update player rooms with new values
    char* playerRoom = strtok(playerInfoToken, " ");
    section = 0;
    while (playerRoom != NULL) {
        _playerRooms[section] = std::stoi(playerRoom);
        playerRoom = strtok(NULL, " ");
        section++;
    }

    // Update oilcooldowns to match server
    char* cooldown = strtok(oilToken, " ");
    section = 0;
    while (cooldown != NULL) {
        _oilCooldown[section] = std::stoi(cooldown);
        cooldown = strtok(NULL, " ");
        section++;
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
