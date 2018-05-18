//
// Created by Josh on 3/26/2018.
//

#include "SpawnController.h"

using namespace cugl;

#define ENEMY_NAME_LEN 4

/**Helper function for generating random enemy keys
 * Source: Carl (stackoverflow)
 */
std::string genRandName(int len)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		return charset[rand() % (sizeof(charset) - 1)];
	};
	std::string str(len, 0);
	std::generate_n(str.begin(), len, randchar);
	return str;
}

/**
 * Creates a spawn controller
 */
bool SpawnController::init(const std::shared_ptr<AssetManager>& assets, std::vector<std::vector<float>> initialEnemies){
    //when this equals -1, stop spawning
    _active = true;
    _currSpawnIndex = 0;
    _totalTime = 0;

    _enemyArray = initialEnemies;

    _endless = false;

    return true;
}

bool SpawnController::endlessInit(const std::shared_ptr<AssetManager>& assets, int numPlayers){
    _active = true;
    _totalTime = 0;

    _endless = true;
    _numPlayers = numPlayers;
    _spawnChance = numPlayers*0.003f;
    gameModel.setEndTime(FLT_MAX/2);
    gameModel._unlockedRooms["lookout"] = true;
    gameModel._unlockedRooms["oil"] = true;
    gameModel._unlockedRooms["mage"] = true;
    gameModel._unlockedRooms["ballista"] = true;
    gameModel._unlockedRooms["repair"] = true;
    gameModel._unlockedRooms["ammo"] = true;

    return true;
}

void SpawnController::update(float deltaTime) {
    _totalTime++;
	if (_totalTime%20 == 0) {
		//CULog("Total time in Spawn Controller %d", _totalTime);
	}
    gameModel.setCurrentTime(_totalTime);
	if (gameModel.isServer() || !gameModel.isNetworked()) {
	    if(!_endless){
	        //regular
            if(_currSpawnIndex >= 0 && _currSpawnIndex < _enemyArray.size()){
                if(_enemyArray[_currSpawnIndex][4]<_totalTime){
                    //we need to spawn one

                    //keep generating ekey until we find a unique one
                    std::string ekey;
                    do {
                        ekey = genRandName(ENEMY_NAME_LEN);
                    } while ((int)(_enemyArray[_currSpawnIndex][5])<= gameModel._enemyArrayMaster.size() &&
                             gameModel._enemyArrayMaster[(int)(_enemyArray[_currSpawnIndex][5])].find(ekey)
                             != gameModel._enemyArrayMaster[(int)(_enemyArray[_currSpawnIndex][5])].end());

                    // Allocate a new EnemyDataModel in memory
                    std::shared_ptr<EnemyDataModel> e = EnemyDataModel::alloc(ekey,_enemyArray[_currSpawnIndex][3],
                                                        Vec2(_enemyArray[_currSpawnIndex][0], _enemyArray[_currSpawnIndex][1]),
                                                        _enemyArray[_currSpawnIndex][2], _enemyArray[_currSpawnIndex][5]);

                    if (e != nullptr) {
                        gameModel._enemyArrayMaster[(int)(_enemyArray[_currSpawnIndex][5])][ekey] = e;
                    }

                    if(_currSpawnIndex != _enemyArray.size()-1){
                        //still have more enemies to spawn
                        _currSpawnIndex++;
                    }
                    else{
                        //done spawning
                        _currSpawnIndex = -1;
                    }
                    //CULog("Current Spawn Index: %d", _currSpawnIndex);
                }
            }
		}
		else{
		    //endless mode here we go
		    //change spawnChance up here
            std::shared_ptr<EnemyDataModel> e;
		    if(_totalTime==20000){
		        _spawnChance = _spawnChance+_numPlayers*0.002f;
		    }
		    else if(_totalTime>20000 && _totalTime%2000==0){
		        _spawnChance = _spawnChance+_numPlayers*0.001f;
		    }
		    if(rand()%1000<_spawnChance*1000){
		        //choose type of enemy to spawn
		        if(_totalTime<1000){
		            _enemyGroup = 1;
		        }
		        else if(_totalTime<2000){
		            _enemyGroup = rand()%2+1;
		        }
		        else if(_totalTime<3000){
		            _enemyGroup = rand()%3+1;
		        }
		        else if(_totalTime<4000){
		            _enemyGroup = rand()%4+1;
		        }
		        else if(_totalTime<5000){
		            _enemyGroup = rand()%5+1;
		        }
		        else if(_totalTime<7000){
		            _enemyGroup = rand()%3+3;
		        }
		        else if(_totalTime<9000){
		            _enemyGroup = rand()%5+3;
		        }
		        else if(_totalTime<12000){
                    _enemyGroup = rand()%4+6;
                }
		        else if(_totalTime<15000){
                    _enemyGroup = rand()%4+8;
                }
                else if(_totalTime<17000){
                    _enemyGroup = rand()%5+8;
                }
		        else{
		            _enemyGroup = rand()%8+8;
		        }
                //EnemyDataModel: (ekey, health, vecPos, type, sector)
		        //define enemyGroups
		        switch (_enemyGroup) {
                    case 1:
                    //one skeleton
                        _numEnemies = 1;
                        _sector = rand()%6+1;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(rand()%950+50, 700), 1, _sector);

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 2:
                    //one archer
                        _numEnemies = 1;
                        _sector = rand()%6+1;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(rand()%950+50, 700), 2, _sector);

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 3:
                    //one warrior
                        _numEnemies = 1;
                        _sector = rand()%6+1;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            e = EnemyDataModel::alloc(ekey, 2, Vec2(rand()%950+50, 700), 3, _sector);

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 4:
                    //one reaper
                        _numEnemies = 1;
                        _sector = rand()%6+1;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            e = EnemyDataModel::alloc(ekey, 5, Vec2(rand()%950+50, 700), 4, _sector);

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 5:
                    //one berserker
                        _numEnemies = 1;
                        _sector = rand()%6+1;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            e = EnemyDataModel::alloc(ekey, 3, Vec2(rand()%950+50, 700), 5, _sector);

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 6:
                        //three skeletons +-200
                        _numEnemies = 3;
                        _sector = rand()%6+1;
                        _xCoord = rand()%550+250;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            if(_numEnemies == 3){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(_xCoord, 700), 1, _sector);
                            }
                            else if(_numEnemies == 2){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(_xCoord+200, 700), 1, _sector);
                            }
                            else{
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(_xCoord-200, 700), 1, _sector);
                            }

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 7:
                        //three archers 250 apart
                        _numEnemies = 3;
                        _sector = rand()%6+1;
                        _xCoord = rand()%450+300;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            if(_numEnemies == 3){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(_xCoord, 700), 2, _sector);
                            }
                            else if(_numEnemies == 2){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(_xCoord+250, 700), 2, _sector);
                            }
                            else{
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(_xCoord-250, 700), 2, _sector);
                            }

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 8:
                        //two warriors 250 apart
                        _numEnemies = 2;
                        _sector = rand()%6+1;
                        _xCoord = rand()%700+50;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            if(_numEnemies == 2){
                            e = EnemyDataModel::alloc(ekey, 2, Vec2(_xCoord, 700), 3, _sector);
                            }
                            else{
                            e = EnemyDataModel::alloc(ekey, 2, Vec2(_xCoord+250, 700), 3, _sector);
                            }

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 9:
                        //two berserkers 400 apart
                        _numEnemies = 2;
                        _sector = rand()%6+1;
                        _xCoord = rand()%550+50;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            if(_numEnemies == 2){
                            e = EnemyDataModel::alloc(ekey, 3, Vec2(_xCoord, 700), 5, _sector);
                            }
                            else{
                            e = EnemyDataModel::alloc(ekey, 3, Vec2(_xCoord+400, 700), 5, _sector);
                            }

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 10:
                        //reaper + 2 archers 250 apart
                        _numEnemies = 3;
                        _sector = rand()%6+1;
                        _xCoord = rand()%450+300;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            if(_numEnemies == 3){
                            e = EnemyDataModel::alloc(ekey, 5, Vec2(_xCoord, 700), 4, _sector);
                            }
                            else if(_numEnemies == 2){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(_xCoord+250, 700), 2, _sector);
                            }
                            else{
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(_xCoord-250, 700), 2, _sector);
                            }

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 11:
                        //5 skeletons
                        _numEnemies = 5;
                        _sector = rand()%6+1;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            if(_numEnemies == 5){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(50, 700), 1, _sector);
                            }
                            else if(_numEnemies == 4){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(250, 700), 1, _sector);
                            }
                            else if(_numEnemies == 3){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(500, 700), 1, _sector);
                            }
                            else if(_numEnemies == 2){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(750, 700), 1, _sector);
                            }
                            else{
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(1000, 700), 1, _sector);
                            }

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 12:
                        //5 archers
                        _numEnemies = 5;
                        _sector = rand()%6+1;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            if(_numEnemies == 5){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(50, 700), 2, _sector);
                            }
                            else if(_numEnemies == 4){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(250, 700), 2, _sector);
                            }
                            else if(_numEnemies == 3){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(500, 700), 2, _sector);
                            }
                            else if(_numEnemies == 2){
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(750, 700), 2, _sector);
                            }
                            else{
                            e = EnemyDataModel::alloc(ekey, 1, Vec2(1000, 700), 2, _sector);
                            }

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 13:
                        //4 warriors
                        _numEnemies = 4;
                        _sector = rand()%6+1;
                        _xCoord = rand()%200+50;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            if(_numEnemies == 4){
                            e = EnemyDataModel::alloc(ekey, 2, Vec2(_xCoord, 700), 3, _sector);
                            }
                            else if(_numEnemies == 3){
                            e = EnemyDataModel::alloc(ekey, 2, Vec2(_xCoord+250, 700), 3, _sector);
                            }
                            else if(_numEnemies == 2){
                            e = EnemyDataModel::alloc(ekey, 2, Vec2(_xCoord+500, 700), 3, _sector);
                            }
                            else{
                            e = EnemyDataModel::alloc(ekey, 2, Vec2(_xCoord+750, 700), 3, _sector);
                            }

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 14:
                        //3 berserkers
                        _numEnemies = 3;
                        _sector = rand()%6+1;
                        _xCoord = rand()%450+50;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            if(_numEnemies == 3){
                            e = EnemyDataModel::alloc(ekey, 3, Vec2(_xCoord, 700), 5, _sector);
                            }
                            else if(_numEnemies == 2){
                            e = EnemyDataModel::alloc(ekey, 3, Vec2(_xCoord+250, 700), 5, _sector);
                            }
                            else{
                            e = EnemyDataModel::alloc(ekey, 3, Vec2(_xCoord+500, 700), 5, _sector);
                            }

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    case 15:
                        //3 reapers
                        _numEnemies = 3;
                        _sector = rand()%6+1;
                        _xCoord = rand()%450+50;
                        while(_numEnemies>0){
                            std::string ekey;
                            do {
                                ekey = genRandName(ENEMY_NAME_LEN);
                            } while ((_sector)<= gameModel._enemyArrayMaster.size() &&
                                     gameModel._enemyArrayMaster[_sector].find(ekey)
                                     != gameModel._enemyArrayMaster[_sector].end());
                            if(_numEnemies == 3){
                            e = EnemyDataModel::alloc(ekey, 5, Vec2(_xCoord, 700), 4, _sector);
                            }
                            else if(_numEnemies == 2){
                            e = EnemyDataModel::alloc(ekey, 5, Vec2(_xCoord+250, 700), 4, _sector);
                            }
                            else{
                            e = EnemyDataModel::alloc(ekey, 5, Vec2(_xCoord+500, 700), 4, _sector);
                            }

                            if (e != nullptr) {
                                gameModel._enemyArrayMaster[_sector][ekey] = e;
                            }
                            _numEnemies--;
                        }
                        break;
                    }
		    }
		}
	}
}

void SpawnController::dispose(){
    _active = false;
    _enemyArray.clear();
}