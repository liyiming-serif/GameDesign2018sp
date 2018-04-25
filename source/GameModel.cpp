//
// Created by Josh on 3/15/2018.
//

#include "GameModel.h"

#define DRAW_SCALE 12
#define GAME_WIDTH 1024
#define BASE_SPEED 0.5f/3


using namespace cugl;

bool GameModel::init(){
    clock = 0;
    gameModel.networked = false;
    gameModel._arrowAmmo[0] = 30;
    gameModel._arrowAmmo[1] = 0;
    gameModel._arrowAmmo[2] = 0;
    gameModel._deltaAmmo[0] = 0;
    gameModel._deltaAmmo[1] = 0;
    gameModel._deltaAmmo[2] = 0;

    for (int i = 0; i < 6; ++i) {
        gameModel._castleHealth[i] = 80;
        gameModel._deltaCastleHealth[i] = 0;
        gameModel._oilPoured[i] = 0;
        gameModel._oilCooldown[i] = 0;
    }
    gameModel._noPlayers = 1;
    gameModel._playerAvatars = new int[gameModel._noPlayers];
    gameModel._playerRooms = new int[gameModel._noPlayers];
    for (int i = 0; i < gameModel._noPlayers; ++i) {
        gameModel._playerRooms[i] = 0;
    }

    gameModel._playerID = 0;

    gameModel._currentRoom = 0;

    gameModel.server = false;

    return true;
}

void GameModel::dispose() {
	for (int i = 0; i < gameModel._enemyArrayMaster.size(); i++) {
		gameModel._enemyArrayMaster[i].clear();
	}
	for (int i = 0; i < gameModel._enemiesToFreeMaster.size(); i++) {
		gameModel._enemiesToFreeMaster[i].clear();
	}
}

void GameModel::update(float deltaTime){
    _noPlayers = gameModel._noPlayers;
    if (gameModel.networked) {
        if (clock == 300) {
            if (gameModel.server) {
                //TODO: Read from network
                //Prints the messages from the clients
                char **read_buffers = ConsumeStateServer();
//                char *read_buffers[_noPlayers-1];
//                for (int k = 0; k < _noPlayers-1; ++k) {
//                    read_buffers[k] = random_buffer_client(k);
//                    CULog("RandNet State Change %d %s \n", k, read_buffers[k]);
//                }
                //CULog("RandNet State Change %s \n", read_byte_buffer);
                if (read_buffers[0] != NULL) {
                    updateStateServer(read_buffers);

                    char *write_byte_buffer = return_buffer(produceStateChangeServer());
                    //TODO: Write to network

                    CULog("State Change %s \n", write_byte_buffer);

                    if (sendState(write_byte_buffer) == 1){
                        CULog("At least one write failure");
                    } else {
                        CULog("Write success");
                    }
                    clock = 0;
                    delete[] write_byte_buffer;
                    //delete read_buffers;
                }
            }
            else {
                const char *write_byte_buffer = return_buffer(produceStateChangeClient());
                //TODO: Write to network
                CULog("State Change: %s \n", write_byte_buffer);
                //TODO: Read from network
                char *read_buffer = ConsumeStateClient();
                CULog("Read Server State: %s \n", read_buffer);
                if (read_buffer != NULL) {
                    updateStateClient(read_buffer);
                }
                clock = 0;
                delete[] write_byte_buffer;
                delete[] read_buffer;
            }
        }
        else {
            clock++;
        }
    }

	//update enemies
	for (int wall = 0; wall<gameModel._enemyArrayMaster.size(); wall++) {
		for (std::pair<std::string, std::shared_ptr<EnemyDataModel>> enemy : gameModel._enemyArrayMaster[wall]) {
			Vec2 pos = enemy.second->getPos();
			if (pos.y <= 0) {
				//enemy collided with wall; mark for deletion
				gameModel._enemiesToFreeMaster[wall].push_back(enemy.first);
                if (enemy.second->getType() == 1) {
                    gameModel.addEnemyChange(enemy.first, 0-enemy.second->getHealth());
                }
				gameModel.changeWallHealth(wall, -enemy.second->getDamage());
			}
			else if(pos.y>=enemy.second->getAtkRange()){
				// move enemy
				enemy.second->setPos(Vec2(pos.x,pos.y-(BASE_SPEED*enemy.second->getSpeed())));
			}
			else {
				// enemy in position; begin attacking
				if (enemy.second->getAtkCounter() <= 0) {
					gameModel.changeWallHealth(wall, -enemy.second->getDamage());
					enemy.second->setAtkCounter(enemy.second->getAtkSpeed());
				}
				else {
					enemy.second->setAtkCounter(enemy.second->getAtkCounter() - 1);
				}
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

	//decrease oil cooldown
	for (int wall = 0; wall < 6; wall++) {
		if (gameModel._oilCooldown[wall] > 0) {
			gameModel._oilCooldown[wall] -= 1;
		}
		else {
			gameModel._oilCooldown[wall] = 0;
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
		this->_castleHealth[wall] += amt;
	}
}

void GameModel::setWallHealth(int wall, int amt) {
    //cap health between 0 and 100
    if (amt > 100) {
        _castleHealth[wall] = 100;
    }
    else if (amt < 0) {
        _castleHealth[wall] = 0;
    }
    else {
        _castleHealth[wall] = amt;
    }
}

int GameModel::getPlayerAvatar(int player) {
    return _playerAvatars[player];
}

void GameModel::setPlayerAvatar(int player, int avatar) {
    _playerAvatars[player] = avatar;
}

void GameModel::setOilCooldown(int wall, int amount) {
    if (amount >= 0) {
        _oilCooldown[wall] = amount;
    }
    else {
        _oilCooldown[wall] = 0;
    }
}

void GameModel::addDeltaHealth(int wall, int repair) {
    gameModel._deltaCastleHealth[wall] += repair;
}

int GameModel::getDeltaHealth(int wall) {
    return gameModel._deltaCastleHealth[wall];
}

void GameModel::addDeltaAmmo(int type, int amt) {
    gameModel._deltaAmmo[type] += amt;
}

void GameModel::addEnemyChange(std::string name, int damage) {
    gameModel._enemyChanges += name+ ":" + to_string(damage) + " ";
}

std::string GameModel::produceStateChangeServer() {
    std::string _tmpHealthString = "";
    std::string _tmpAmmoString = "";
    std::string _tmpEnemyString = "";
    std::string _tmpPlayerString = "";
    std::string _tmpOilString = "";

    for (int i = 0; i < 6; ++i) {
        _tmpHealthString+= to_string(gameModel._castleHealth[i]) + " ";
        for (auto it = gameModel._enemyArrayMaster[i].begin(); it != gameModel._enemyArrayMaster[i].end(); ++it) {
            _tmpEnemyString += it->second->toString() + " ";
        }
        _tmpOilString += to_string(gameModel._oilCooldown[i]) + " ";
    }

    for (int i = 0; i < 3; ++i) {
        _tmpAmmoString += to_string(gameModel._arrowAmmo[i]) + " ";
    }

    for (int i = 0; i < gameModel._noPlayers; ++i) {
        if (i==0) {
            _tmpPlayerString += to_string(gameModel._playerAvatars[i]) + ":" + to_string(gameModel._currentRoom) + " ";
        }
        else {
            _tmpPlayerString += to_string(gameModel._playerAvatars[i]) + ":" + to_string(gameModel._playerRooms[i]) + " ";
        }

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
    int messageSizeSize = to_string(premessageSize + to_string(premessageSize).length()).length();
    int totalmessageSize = premessageSize + messageSizeSize;
    return to_string(totalmessageSize) + "|" + premessage;
}

std::string GameModel::produceStateChangeClient() {
    std::string _tmpHealthString = "";
    std::string _tmpAmmoString = "";
    std::string _tmpEnemyString = "";
    std::string _tmpPlayerString = "";
    std::string _tmpOilString = "";

    for (int i = 0; i < 6; ++i) {
        _tmpHealthString+= to_string(gameModel._castleHealth[i]) + ":" + to_string(gameModel._deltaCastleHealth[i]) + " ";
        gameModel._deltaCastleHealth[i] = 0;
        _tmpEnemyString += gameModel._enemyChanges;
        gameModel._enemyChanges = "";
        _tmpOilString += to_string(gameModel._oilPoured[i]) + " ";
        gameModel._oilPoured[i] = 0;
    }

    for (int i = 0; i < 3; ++i) {
        _tmpAmmoString += to_string(gameModel._deltaAmmo[i]) + " ";
        gameModel._deltaAmmo[i] = 0;
    }

    _tmpPlayerString += to_string(gameModel._playerAvatars[gameModel._playerID]) + ":" + to_string(gameModel._currentRoom);

    _tmpHealthString.pop_back();
    _tmpAmmoString.pop_back();
    _tmpEnemyString.pop_back();
    _tmpOilString.pop_back();

    //TODO: Game recovery state (currserver, gameclock, etc)
    std::string premessage = _tmpHealthString + "|" + _tmpEnemyString + "|" +
                             _tmpAmmoString + "|" + _tmpPlayerString + "|" + _tmpOilString;

    int premessageSize = premessage.length();
    int postmessageSize = premessageSize + to_string(premessageSize).length();
    int totalmessageSize = premessageSize + to_string(postmessageSize).length();
    return to_string(totalmessageSize) + "|" + premessage;
}

char** GameModel::ConsumeStateServer() {
    char *tmp[gameModel._noPlayers-1];
    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        tmp[i] = consumeState();
        CULog("State from Client %i: %s", i, tmp[i]);
    }
    return tmp;
}

char* GameModel::ConsumeStateClient() {
    return consumeState();
}

void GameModel::updateStateServer(char** ConsumedStates) {

    // Break down all read states into component changes to apply
    CULog("Breaking down ConsumedStates");
    char * tmpHealthChanges[gameModel._noPlayers-1];
    char * tmpAmmoChanges[gameModel._noPlayers-1];
    char * tmpEnemyChanges[gameModel._noPlayers-1];
    char * tmpPlayerChanges[gameModel._noPlayers-1];
    char * tmpOilChanges[gameModel._noPlayers-1];
    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (ConsumedStates[i] != NULL) {
            char* copy = strdup(ConsumedStates[i]);
            const char s[2] = "|";
            char* token;
            char* ammoToken;
            char* enemyToken;
            char* sizeToken;
            char* oilToken;
            char* playerInfoToken;
            char* castleHealthToken;
            int section = 0;
            token = strtok(copy, s);
            while (token != NULL) {
                if (section == 0) {
                    sizeToken = strdup(token);
                } else if (section == 1) {
                    tmpHealthChanges[i] = strdup(token);
                } else if (section == 2) {
                    tmpEnemyChanges[i] = strdup(token);
                } else if (section == 3) {
                    tmpAmmoChanges[i] = strdup(token);
                } else if (section == 4) {
                    tmpPlayerChanges[i] = strdup(token);
                } else if (section == 5) {
                    tmpOilChanges[i] = strdup(token);
                }
                //CULog("RandNet Token %d %s \n", section, token);
                token = strtok(NULL,s);
                section++;
            }
            free(copy);
        }
    }

    // Start aggregating castle health changes by wall
    CULog("Aggregating Castle Health");
    char* subtoken;

    char* tmpHealthN[gameModel._noPlayers-1];
    char* tmpHealthNW[gameModel._noPlayers-1];
    char* tmpHealthSW[gameModel._noPlayers-1];
    char* tmpHealthS[gameModel._noPlayers-1];
    char* tmpHealthSE[gameModel._noPlayers-1];
    char* tmpHealthNE[gameModel._noPlayers-1];

    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpHealthChanges[i] != NULL) {
            subtoken = strtok(tmpHealthChanges[i], " ");
            int j = 0;
            while (subtoken != NULL) {
                if (j == 0) {
                    tmpHealthN[i] = subtoken;
                } else if (j == 1) {
                    tmpHealthNW[i] = subtoken;
                } else if (j == 2) {
                    tmpHealthSW[i] = subtoken;
                } else if (j == 3) {
                    tmpHealthS[i] = subtoken;
                } else if (j == 4) {
                    tmpHealthSE[i] = subtoken;
                } else if (j == 5) {
                    tmpHealthNE[i] = subtoken;
                }
                subtoken = strtok(NULL, " ");
                j++;
            }
            free(tmpHealthChanges[i]);
        }
    }

    // Optimistically apply new health as max of all wall healths (minus deltas), plus the sum of all deltas
    int castleHealthUpdate[6];
    char* tottoken;
    char* deltoken;

    // North Wall
    int currmax = 0;
    int deltasum = 0;

    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpHealthN[i] != NULL) {
            tottoken = strtok(tmpHealthN[i], ":");
            //CULog("RandNet Token %d %s \n", i, tottoken);
            deltoken = strtok(NULL, ":");
            //CULog("RandNet Token %d %s \n", i, deltoken);
            int repairdelt = std::stoi(deltoken);
            int currplayerhealth = std::stoi(tottoken);
            if (currplayerhealth > currmax) {
                currmax = currplayerhealth;
            }
            deltasum += repairdelt;
        }
    }
    if (gameModel.getWallHealth(0) > currmax) {
        currmax = gameModel.getWallHealth(0);
    }
    castleHealthUpdate[0] = deltasum + currmax;

    // NorthWest Wall
    currmax = 0;
    deltasum = 0;

    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpHealthNW[i] != NULL) {
            tottoken = strtok(tmpHealthNW[i], ":");
            deltoken = strtok(NULL, ":");
            int currplayerhealth = std::stoi(tottoken);
            int repairdelt = std::stoi(deltoken);
            if (currplayerhealth > currmax) {
                currmax = currplayerhealth;
            }
            deltasum += repairdelt;
        }
    }
    if (gameModel.getWallHealth(1) > currmax) {
        currmax = gameModel.getWallHealth(1);
    }
    castleHealthUpdate[1] = deltasum + currmax;

    //SouthWest Wall
    currmax = 0;
    deltasum = 0;

    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpHealthSW[i] != NULL) {
            tottoken = strtok(tmpHealthSW[i], ":");
            deltoken = strtok(NULL, ":");
            int currplayerhealth = std::stoi(tottoken);
            int repairdelt = std::stoi(deltoken);
            if (currplayerhealth > currmax) {
                currmax = currplayerhealth;
            }
            deltasum += repairdelt;
        }
    }
    if (gameModel.getWallHealth(2) > currmax) {
        currmax = gameModel.getWallHealth(2);
    }
    castleHealthUpdate[2] = deltasum + currmax;

    //South Wall
    currmax = 0;
    deltasum = 0;

    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpHealthS[i] != NULL) {
            tottoken = strtok(tmpHealthS[i], ":");
            deltoken = strtok(NULL, ":");
            int currplayerhealth = std::stoi(tottoken);
            int repairdelt = std::stoi(deltoken);
            if (currplayerhealth > currmax) {
                currmax = currplayerhealth;
            }
            deltasum += repairdelt;
        }

    }
    if (gameModel.getWallHealth(3) > currmax) {
        currmax = gameModel.getWallHealth(3);
    }
    castleHealthUpdate[3] = deltasum + currmax;

    // SouthEast Wall
    currmax = 0;
    deltasum = 0;

    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpHealthSE[i] != NULL) {
            tottoken = strtok(tmpHealthSE[i], ":");
            deltoken = strtok(NULL, ":");
            int currplayerhealth = std::stoi(tottoken);
            int repairdelt = std::stoi(deltoken);
            if (currplayerhealth > currmax) {
                currmax = currplayerhealth;
            }
            deltasum += repairdelt;
        }
    }
    if (gameModel.getWallHealth(4) > currmax) {
        currmax = gameModel.getWallHealth(4);
    }
    castleHealthUpdate[4] = deltasum + currmax;

    // NorthEast Wall
    currmax = 0;
    deltasum = 0;

    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpHealthNE[i] != NULL) {
            tottoken = strtok(tmpHealthNE[i], ":");
            deltoken = strtok(NULL, ":");
            int currplayerhealth = std::stoi(tottoken);
            int repairdelt = std::stoi(deltoken);
            if (currplayerhealth > currmax) {
                currmax = currplayerhealth;
            }
            deltasum += repairdelt;
        }
    }
    if (gameModel.getWallHealth(5) > currmax) {
        currmax = gameModel.getWallHealth(5);
    }
    castleHealthUpdate[5] = deltasum + currmax;

    // Apply castle health changes
    for (int i = 0; i < 6; ++i) {
        //CULog("wallHealth: %d \n", getWallHealth(i));
        gameModel.setWallHealth(i, castleHealthUpdate[i]);
        //CULog("wallHealth: %d \n", getWallHealth(i));
        CULog("total: %i, update: %i", gameModel._castleHealth[i], castleHealthUpdate[i]);
    }

    // Sum all arrow changes
    char* arrowsubtoken;
    int deltaArrow1 = 0;
    int deltaArrow2 = 0;
    int deltaArrow3 = 0;
    CULog("Applying arrow changes");

    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpAmmoChanges[i] != NULL) {
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
            free(tmpAmmoChanges[i]);
        }
    }
    gameModel.setArrowAmmo(0, gameModel.getArrowAmmo(0)+deltaArrow1);
    gameModel.setArrowAmmo(1, gameModel.getArrowAmmo(1)+deltaArrow2);
    gameModel.setArrowAmmo(2, gameModel.getArrowAmmo(2)+deltaArrow3);


    // Apply all damage created by all players to the respective monsters
    CULog("Applying enemy changes");
    char* enemySubToken;
    char* enemyName;
    char* enemyDamage;

    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpEnemyChanges[i] != NULL) {
            enemySubToken = strtok(tmpEnemyChanges[i], " :");
            while (enemySubToken != NULL) {
                strcpy(enemyName, enemySubToken);
                enemySubToken = strtok(NULL, " :");
                strcpy(enemyDamage, enemySubToken);
                for (int j = 0; j<6; ++j) {
                    std::unordered_map<std::string,std::shared_ptr<EnemyDataModel>>::iterator got = gameModel._enemyArrayMaster[j].find (enemyName);
                    if ( got != gameModel._enemyArrayMaster[j].end() ) {
                        std::shared_ptr<EnemyDataModel> thisEnemy = got->second;
                        thisEnemy->setHealth(thisEnemy->getHealth() + std::stoi(enemyDamage));
                    }
                }
                enemySubToken = strtok(NULL, " :");
            }
            free(tmpEnemyChanges[i]);
        }
    }

    // Aggregate player info changes; if players occupy the same room and it's not overworld, kick player with higher ID
    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpPlayerChanges[i] != NULL) {
            char *roomToken = strtok(tmpPlayerChanges[i], ":");
            int playerID = std::stoi(roomToken);
            roomToken = strtok(NULL, ":");
            int playerRoom = std::stoi(roomToken);
            bool occupied = false;
            for (int j = 0; j < gameModel._noPlayers; ++j) {
                if (playerRoom == gameModel._playerRooms[j]) {
                    occupied = true;
                }
            }
            if (occupied) {
                _playerRooms[playerID] = 0;
            } else {
                _playerRooms[playerID] = playerRoom;
            }
            free(tmpPlayerChanges[i]);
        }
    }

    // Start oilcooldowns for all walls that have poured
    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        if (tmpOilChanges[i] != NULL) {
            char* pourToken = strtok(tmpOilChanges[i], " ");
            int section = 0;
            while (pourToken != NULL) {
                int poured = std::stoi(pourToken);
                if (poured == 1 && gameModel._oilCooldown[section] == 0) {
                    gameModel._oilCooldown[section] = 420;
                }
                pourToken = strtok(NULL, " ");
                section++;
            }
            free(tmpOilChanges[i]);
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
        gameModel._castleHealth[section] = std::stoi(wallHealthToken);
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
    if (enemyDataToken != NULL) {
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
            std::unordered_map<std::string,std::shared_ptr<EnemyDataModel>>::iterator got = gameModel._enemyArrayMaster[std::stoi(enemyWall)].find (enemyName);

            if ( got == gameModel._enemyArrayMaster[std::stoi(enemyWall)].end() ) {
                // Allocate a new EnemyDataModel in memory
                std::shared_ptr<EnemyDataModel> e =
                        EnemyDataModel::alloc(enemyName,std::stoi(enemyHealth),
                                              Vec2(std::stof(enemyPosX), std::stof(enemyPosY)),
                                              std::stoi(enemyType), std::stoi(enemyWall));

                if (e != nullptr) {
                    gameModel._enemyArrayMaster[std::stoi(enemyWall)][enemyName] = e;
                }
            }
            else {
                std::shared_ptr<EnemyDataModel> thisEnemy = got->second;
                thisEnemy->setHealth(thisEnemy->getHealth() + std::stoi(enemyHealth));
                thisEnemy->setPos(Vec2(std::stof(enemyPosX), std::stof(enemyPosY)));
            }
            enemyDataToken = strtok(NULL, " ");
            section++;
        }
    }


    // Update arrow ammo with new values
    char* arrowToken = strtok(ammoToken, " ");
    section = 0;
    while (arrowToken != NULL) {
        gameModel._arrowAmmo[section] = std::stoi(arrowToken);
        arrowToken = strtok(NULL, " ");
        section++;
    }

    // Update player rooms with new values
    char* playerRoom = strtok(playerInfoToken, " ");
    section = 0;
    while (playerRoom != NULL) {
        gameModel._playerRooms[section] = std::stoi(playerRoom);
        playerRoom = strtok(NULL, " ");
        section++;
    }

    // Update oilcooldowns to match server
    char* cooldown = strtok(oilToken, " ");
    section = 0;
    while (cooldown != NULL) {
        gameModel._oilCooldown[section] = std::stoi(cooldown);
        cooldown = strtok(NULL, " ");
        section++;
    }

    free(copy);
}

char* GameModel::random_buffer_client(int player) {
    std::string _tmpHealthString = "";
    std::string _tmpAmmoString = "";
    std::string _tmpEnemyString = "";
    std::string _tmpPlayerString = "";
    std::string _tmpOilString = "";

    for (int i = 0; i < 6; ++i) {
        _tmpHealthString+= to_string(rand()%80) + ":" + to_string(rand()%5) + " ";

        int randomoil= rand()%5;
        if (randomoil < 4) {
            randomoil = 0;
        }
        else {
            randomoil = 1;
        }
        _tmpOilString += to_string(randomoil) + " ";
    }

    _tmpEnemyString += gameModel._enemyChanges + " ";
    gameModel._enemyChanges = "";

    for (int i = 0; i < 3; ++i) {
        _tmpAmmoString += to_string(rand()%10-5) + " ";
    }

    _tmpPlayerString += to_string(player) + ":" + to_string(rand()%17);

    _tmpHealthString.pop_back();
    _tmpAmmoString.pop_back();
    _tmpOilString.pop_back();

    //TODO: Game recovery state (currserver, gameclock, etc)
    std::string premessage = _tmpHealthString + "|" + _tmpEnemyString + "|" +
                             _tmpAmmoString + "|" + _tmpPlayerString + "|" + _tmpOilString;

    int premessageSize = premessage.length();
    int postmessageSize = premessageSize + to_string(premessageSize).length();
    int totalmessageSize = premessageSize + to_string(postmessageSize).length();
    std::string tmp_string = to_string(totalmessageSize) + "|" + premessage;

    return return_buffer(tmp_string);
}

char* GameModel::random_buffer_server() {
    std::string _tmpHealthString = "";
    std::string _tmpAmmoString = "";
    std::string _tmpEnemyString = "";
    std::string _tmpPlayerString = "";
    std::string _tmpOilString = "";

    for (int i = 0; i < 6; ++i) {
        _tmpHealthString+= to_string(rand()%100) + " ";
        _tmpOilString += to_string(rand()%420) + " ";
    }

    _tmpEnemyString += " ";

    for (int i = 0; i < 3; ++i) {
        _tmpAmmoString += to_string(rand()%50) + " ";
    }

    for (int i = 0; i < gameModel._noPlayers-1; ++i) {
        _tmpPlayerString += to_string(gameModel._playerAvatars[i]) + ":" + to_string(rand()%17) + " ";
    }

    _tmpHealthString.pop_back();
    _tmpAmmoString.pop_back();
    _tmpOilString.pop_back();
    _tmpPlayerString.pop_back();

    //TODO: Game recovery state (currserver, gameclock, etc)
    std::string premessage = _tmpHealthString + "|" + _tmpEnemyString + "|" +
                             _tmpAmmoString + "|" + _tmpPlayerString + "|" + _tmpOilString;

    int premessageSize = premessage.length();
    int postmessageSize = premessageSize + to_string(premessageSize).length();
    int totalmessageSize = premessageSize + to_string(postmessageSize).length();
    std::string tmp_string = to_string(totalmessageSize) + "|" + premessage;

    return return_buffer(tmp_string);
}

char* GameModel::return_buffer(const std::string& string) {
    char* return_string = new char[string.length() + 1];
    strcpy(return_string, string.c_str());
    return return_string;
}

bool GameModel::isNetworked(){
    return gameModel.networked;
}

void GameModel::setNetworked(bool networked){
    gameModel.networked = networked;
}

bool GameModel::isServer(){
    return gameModel.server;
}

void GameModel::setServer(bool server){
    gameModel.server = server;
}

void GameModel::setCurrentRoom(int room){
    gameModel._currentRoom = room;
}

//#if CU_PLATFORM == CU_PLATFORM_ANDROID
//JNIEXPORT char* JNICALL Java_edu_cornell_gdiac_chaoscastle_ChaosCastle_readNetwork
//        (JNIEnv *env, jclass clazz, jbyteArray array) {
//    jbyte* buffer = env->GetByteArrayElements(array, NULL);
//    jsize size = env->GetArrayLength(array);
//    char *byte_buffer = new char[size];
//
//    for(int i = 0; i < size; i++) {
//        byte_buffer[i] = buffer[i];
//    }
//    env->ReleaseByteArrayElements(array, buffer, JNI_ABORT);
//    return byte_buffer;
//}
//#endif
