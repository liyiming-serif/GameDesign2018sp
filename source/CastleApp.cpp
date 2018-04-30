//
//  CastleApp.cpp
//  Cornell University Game Library (CUGL)
//
//  This is the implementation file for the custom application. This is the
//  definition of your root (and in this case only) class.
//
//  CUGL zlib License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Walker White
//  Version: 1/8/17
//
// Include the class header, which includes all of the CUGL classes
#include "CastleApp.h"

#define BALLISTA    1
#define OVERWORLD   2
#define LOOKOUT     3
#define REPAIR      4
#define MENU        5
#define AMMO        6
#define MAGE        7
#define OIL         8
#define LEVELS      9
#define LOBBY       10


// This keeps us from having to write cugl:: all the time
using namespace cugl;

//global variables
InputController input;
GameModel gameModel;
/**
 * The method called after OpenGL is initialized, but before running the application.
 *
 * This is the method in which all user-defined program intialization should
 * take place.  You should not create a new init() method.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to FOREGROUND,
 * causing the application to run.
 */
void CastleApp::onStartup() {

    // Create a sprite batch to render the scene
    _batch = SpriteBatch::alloc();
    // Create an asset manager to load all assets
    _assets = AssetManager::alloc();
    
    // Activate mouse or touch screen input as appropriate
    // We have to do this BEFORE the scene, because the scene has a button
#ifdef CU_TOUCH_SCREEN
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
#endif
	input.init();

	gameModel.init();

    // You have to attach the individual loaders for each asset type
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    _assets->attach<Node>(SceneLoader::alloc()->getHook());
    _assets->attach<Sound>(SoundLoader::alloc()->getHook());
    _assets->attach<Music>(MusicLoader::alloc()->getHook());
    _assets->attach<JSONReader>(GenericLoader<JSONReader>::alloc()->getHook());
    
//   From original code // This reads the given JSON file and uses it to load all other assets
//    _assets->loadDirectory("json/assets.json");

    // Create a "loading" screen
    _loaded = false;
    _loadingScene.init(_assets);
    // Queue up the other assets
    _assets->loadDirectoryAsync("json/assets.json",nullptr);
    _assets->loadAsync<JSONReader>("slevels", "json/levels.json", nullptr);
    _direction = -1;
    _players = -1;
    Application::onStartup(); // YOU MUST END with call to parent

}

/**
 * The method called when the application is ready to quit.
 *
 * This is the method to dispose of all resources allocated by this
 * application.  As a rule of thumb, everything created in onStartup()
 * should be deleted here.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to NONE,
 * causing the application to be deleted.
 */
void CastleApp::onShutdown() {
    // Delete all smart pointers
    _menuScene.dispose();
    _loadingScene.dispose();
    _levelScene.dispose();
    _overworldScene.dispose();
    _ballistaScene.dispose();
    _lookoutScene.dispose();
    _repairScene.dispose();
    _mageScene.dispose();
    _ammoScene.dispose();
    _oilScene.dispose();
    _lobbyScene.dispose();
    gameModel.dispose();
    _spawnController.dispose();
    _batch = nullptr;
    _assets = nullptr;

    // Deativate input
	input.dispose();
#if defined CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif

    Application::onShutdown();
}

/**
 * The method called to update the application data.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should contain any code that is not an OpenGL call.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implementation does nothing.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void CastleApp::update(float timestep) {
	input.pollInputs();
    if (!_loaded && _loadingScene.isActive()) { //is loading
        _loadingScene.update(0.01f);
        if (_loadingScene.isPending()){
            _loadingScene.dispose(); // Disables the input listeners in this mode
            _ballistaScene.init(_assets);
            _ballistaScene.setActive(false, 0);
            _lookoutScene.init(_assets);
            _lookoutScene.setActive(false);
            _repairScene.init(_assets);
            _repairScene.setActive(false);
            _overworldScene.init(_assets);
            _overworldScene.setActive(false);
            _mageScene.init(_assets);
            _mageScene.setActive(false);
            _ammoScene.init(_assets);
            _ammoScene.setActive(false);
            _oilScene.init(_assets);
            _oilScene.setActive(false, 0);
            _lobbyScene.init(_assets);
            _lobbyScene.setActive(false);
            _levelScene.init(_assets);
            _levelScene.setActive(false, 0);
            _menuScene.init(_assets);
            _currscene=MENU;
            _loaded=true;
        }
    } else {
        if(_currscene==MENU) {
            _menuScene.update(timestep);
            if(_menuScene.switchscene!=0){
                swapscenes(_menuScene.switchscene, 0);
                _menuScene.setActive(false);
            }
        }
        else if(_currscene==LOBBY){
            _lobbyScene.update(timestep);
            if(_lobbyScene.switchscene!=0){
                swapscenes(_lobbyScene.switchscene, 0);
                _lobbyScene.setActive(false);
            }
        }
        else if(_currscene==LEVELS){
            _levelScene.update(timestep);
            if(_levelScene.switchscene!=0){
                swapscenes(_levelScene.switchscene, 0);
                _levelScene.setActive(false, _direction);
            }
        }
        else{ //gameplay update loop
            if(_currscene==OVERWORLD) {
                _overworldScene.update(timestep);
                if(_overworldScene.switchscene!=0){
                    swapscenes(_overworldScene.switchscene, _overworldScene.direction);
                    _overworldScene.setActive(false);
                }
            }
            else if(_currscene==BALLISTA){
                _ballistaScene.update(timestep, _direction);
                if(_ballistaScene.switchscene!=0){
                    swapscenes(_ballistaScene.switchscene, 0);
                    _ballistaScene.setActive(false, _direction);
                }
            }
            else if(_currscene==LOOKOUT){
                _lookoutScene.update(timestep);
                if(_lookoutScene.switchscene!=0){
                    swapscenes(_lookoutScene.switchscene, 0);
                    _lookoutScene.setActive(false);
                }
            }
            else if(_currscene==REPAIR){
                _repairScene.update(timestep);
                if(_repairScene.switchscene!=0){
                    swapscenes(_repairScene.switchscene, 0);
                    _repairScene.setActive(false);
                }
            }
            else if(_currscene==MAGE){
                _mageScene.update(timestep);
                if(_mageScene.switchscene!=0){
                    swapscenes(_mageScene.switchscene, 0);
                    _mageScene.setActive(false);
                }
            }
            else if(_currscene==AMMO){
                _ammoScene.update(timestep);
                if(_ammoScene.switchscene!=0){
                    swapscenes(_ammoScene.switchscene, 0);
                    _ammoScene.setActive(false);
                }
            }
            else if(_currscene==OIL){
                _oilScene.update(timestep, _direction);
                if(_oilScene.switchscene!=0){
                    swapscenes(_oilScene.switchscene, 0);
                    _oilScene.setActive(false, _direction);
                }
            }
            _spawnController.update(timestep);
            gameModel.update(timestep);
        }
    }
    //refresh the input controller
	input.update(timestep);

}

void CastleApp::swapscenes(int nextscene, int direction){
    _direction = direction;
    if (_currscene == MENU && nextscene == LEVELS){
        _players = 1;
    }
    if (_currscene == LOBBY && nextscene == LEVELS){
        _players = _lobbyScene.getNumPlayers();
    }
	if (_currscene == LEVELS && nextscene == OVERWORLD) {
		_spawnController.init(_assets, _assets->get<JSONReader>("slevels")->readJSON(_players, _levelScene.level));
        gameModel.level=_levelScene.level;
	}
	if(_currscene == OVERWORLD && nextscene == MENU){
	    reset();
	}
    switch(nextscene){
        case MENU:
            _menuScene.setActive(true);
            break;
        case OVERWORLD:
            _overworldScene.setActive(true);
            break;
        case BALLISTA:
            _ballistaScene.setActive(true, _direction);
            break;
        case LOOKOUT:
            _lookoutScene.setActive(true);
            break;
        case REPAIR:
            _repairScene.setActive(true);
            break;
        case MAGE:
            _mageScene.setActive(true);
            break;
        case AMMO:
            _ammoScene.setActive(true);
            break;
        case OIL:
            _oilScene.setActive(true, _direction);
            break;
        case LOBBY:
            _lobbyScene.setActive(true);
            break;
        case LEVELS:
			_levelScene.setActive(true,_players);
			break;
    }
    _currscene = nextscene;
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void CastleApp::draw() {
    // This takes care of begin/end
    if (!_loaded) {
        _loadingScene.render(_batch);
    } else {
        if(_currscene==MENU) {
            _menuScene.render(_batch);
        }
        else if(_currscene==OVERWORLD) {
            _overworldScene.render(_batch);
        }
        else if(_currscene==BALLISTA){
            _ballistaScene.render(_batch);
        }
        else if(_currscene==LOOKOUT){
            _lookoutScene.render(_batch);
        }
        else if(_currscene==REPAIR){
            _repairScene.render(_batch);
        }
        else if(_currscene==MAGE){
            _mageScene.render(_batch);
        }
        else if(_currscene==AMMO){
            _ammoScene.render(_batch);
        }
        else if(_currscene==OIL){
            _oilScene.render(_batch);
        }
        else if(_currscene==LOBBY){
            _lobbyScene.render(_batch);
        }
        else if(_currscene==LEVELS){
            _levelScene.render(_batch);
        }
    }
}
//only reset from overworld scene
void CastleApp::reset(){
    _menuScene.setActive(true);
    _currscene = MENU;
    _overworldScene.setActive(false);

    _levelScene.dispose();
    _overworldScene.dispose();
    _ballistaScene.dispose();
    _lookoutScene.dispose();
    _repairScene.dispose();
    _mageScene.dispose();
    _ammoScene.dispose();
    _oilScene.dispose();
    _lobbyScene.dispose();
    _spawnController.dispose();
    gameModel.dispose();
    input.dispose();
    #ifdef CU_TOUCH_SCREEN
        Input::activate<Touchscreen>();
    #else
        Input::activate<Mouse>();
    #endif
    input.init();
    gameModel.init();
    _ballistaScene.init(_assets);
    _ballistaScene.setActive(false, 0);
    _lookoutScene.init(_assets);
    _lookoutScene.setActive(false);
    _repairScene.init(_assets);
    _repairScene.setActive(false);
    _overworldScene.init(_assets);
    _overworldScene.setActive(false);
    _mageScene.init(_assets);
    _mageScene.setActive(false);
    _ammoScene.init(_assets);
    _ammoScene.setActive(false);
    _oilScene.init(_assets);
    _oilScene.setActive(false, 0);
    _lobbyScene.init(_assets);
    _lobbyScene.setActive(false);
    _levelScene.init(_assets);
    _levelScene.setActive(false, 0);
}
