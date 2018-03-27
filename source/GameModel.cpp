//
// Created by Josh on 3/15/2018.
//

#include <cugl/cugl.h>
#include "GameModel.h"
#include "CastleApp.h"

#define DRAW_SCALE 12
#define GAME_WIDTH 1024

using namespace cugl;

bool GameModel::init(const std::shared_ptr<AssetManager>& assets){
    // Set display size
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    _networkStateOutPath = Application::get()->getSaveDirectory() + "networkStateOutgoing.txt";
    _networkStateInPath = Application::get()->getSaveDirectory() + "networkStateIncoming.txt";
    _networkJSONWriter = TextWriter::alloc(_networkStateOutPath);
    _networkJSONReader = TextReader::alloc(_networkStateOutPath);



    _assets = assets;

    _spawnTimer = 360;

    int sum = 0;

    for (int i = 0; i < 6; ++i) {
        _castleHealth[i] = sum;
        _prevCastleHealth[i] = sum;
        sum +=1;
    }

    // Create the physics world
    //_world = ObstacleWorld::alloc(Rect(Vec2::ZERO, _size/DRAW_SCALE),Vec2::ZERO);


    //testing enemy code
    /*
        std::shared_ptr<EnemyModel> enemy1 = EnemyModel::alloc(Vec2(200, 200), 0, 1, DRAW_SCALE, _assets);
            if(enemy1 != nullptr) {
                _enemyArrayGroundN.insert(enemy1);
                _world->addObstacle(enemy1);
            }
        std::shared_ptr<EnemyModel> enemy2 = EnemyModel::alloc(Vec2(200, 400), 0, 1, DRAW_SCALE, _assets);
            if(enemy2 != nullptr) {
                _enemyArrayGroundN.insert(enemy2);
                _world->addObstacle(enemy2);
            }
            */
    return true;
}

void GameModel::dispose() {
    //if (_world != nullptr) {
    //    _world->clear();
    //    _world = nullptr;
    //}
    _assets = nullptr;
    _enemyArrayGroundN.clear();
	_enemiesToFree.clear();
}

void GameModel::update(float deltaTime){
	//// Update enemies and mark out of bound ones for deletion
	//Rect bounds(Vec2::ZERO, _size / DRAW_SCALE);
	//for (auto it = _enemyArrayGroundN.begin(); it != _enemyArrayGroundN.end(); it++) {
	//	std::shared_ptr<EnemyModel> e = *it;
	//	if (e != nullptr) {
	//		e->update(deltaTime);
	//	}
	//	if (!bounds.contains(e->getPosition())) {
	//		_enemiesToFree.insert(e);
	//	}
	//}

	//// Delete the arrows here because you can't remove elements while iterating
	//for (auto it = _enemiesToFree.begin(); it != _enemiesToFree.end(); it++) {
	//	std::shared_ptr<EnemyModel> e = *it;
	//	_world->removeObstacle(e.get());
	//	removeChild(e->getNode());
	//	_enemyArrayGroundN.erase(e);
	//	CULog("%d\n", _enemyArrayGroundN.size());
	//}
	//_enemiesToFree.clear();
    _networkJSONWriter->write(getStateChange());
    CULog("updating");
    updateState();
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
    int _tmpHealth[6];
    for (int i = 0; i < 6; ++i) {
        _tmpHealth[i] = _castleHealth[i] - _prevCastleHealth[i];
    }
    std::string _tmpHealthString = "{ Health: " + to_string(_tmpHealth, 6) + "}";
    std::string _tmpAvatarString = "{ Avatars: " + to_string(_playerAvatars,6) + "}";
    return _tmpHealthString + ", " + _tmpAvatarString;
}

void GameModel::updateState() {
    std::string _tmpStateString = _networkJSONReader->readLine();
    const char *cstr = _tmpStateString.c_str();
    CULog("% \n", cstr);
    for (int i = 0; i < 6; ++i) {
        _prevCastleHealth[i] = _castleHealth[i];
    }
}



