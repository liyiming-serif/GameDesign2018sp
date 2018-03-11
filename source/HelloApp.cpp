//
//  HelloApp.cpp
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
#include "HelloApp.h"

#define BALLISTA    1
#define OVERWORLD   2
#define LOOKOUT     3
#define REPAIR     4

// This keeps us from having to write cugl:: all the time
using namespace cugl;


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
void HelloApp::onStartup() {

    // Create a sprite batch to render the scene
    _batch = SpriteBatch::alloc();
    // Create an asset manager to load all assets
    _assets = AssetManager::alloc();

    // You have to attach the individual loaders for each asset type
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    _assets->attach<Node>(SceneLoader::alloc()->getHook());

    // This reads the given JSON file and uses it to load all other assets
    _assets->loadDirectory("json/assets.json");

    // Activate mouse or touch screen input as appropriate
    // We have to do this BEFORE the scene, because the scene has a button
#ifdef CU_TOUCH_SCREEN
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
#endif

    // Build the scene from these assets
    _ballistaScene.init(_assets);
    _ballistaScene.setActive(false);
    _lookoutScene.init(_assets);
    _lookoutScene.setActive(false);
    _repairScene.init(_assets);
    _repairScene.setActive(false);
    _overworldScene.init(_assets);
    _currscene=OVERWORLD;

    Application::onStartup(); //call super
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
void HelloApp::onShutdown() {
    // Delete all smart pointers
    _overworldScene.dispose();
    _ballistaScene.dispose();
    _lookoutScene.dispose();
    _repairScene.dispose();
    _batch = nullptr;
    _assets = nullptr;

    // Deativate input
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
 * at all. The default implmentation does nothing.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void HelloApp::update(float timestep) {
    if(_currscene==OVERWORLD) {
        _overworldScene.update(timestep);
        if(_overworldScene.switchscene!=0){
            swapscenes(_overworldScene.switchscene);
            _overworldScene.setActive(false);
        }
    }
    else if(_currscene==BALLISTA){
        _ballistaScene.update(timestep);
        if(_ballistaScene.switchscene!=0){
            swapscenes(_ballistaScene.switchscene);
            _ballistaScene.setActive(false);
        }
    }
    else if(_currscene==LOOKOUT){
        _lookoutScene.update(timestep);
        if(_lookoutScene.switchscene!=0){
            swapscenes(_lookoutScene.switchscene);
            _lookoutScene.setActive(false);
        }
    }
    else if(_currscene==REPAIR){
        _repairScene.update(timestep);
        if(_repairScene.switchscene!=0){
            swapscenes(_repairScene.switchscene);
            _repairScene.setActive(false);
        }
    }
}

void HelloApp::swapscenes(int nextscene){
    switch(nextscene){
        case OVERWORLD:
            _overworldScene.setActive(true);
            break;
        case BALLISTA:
            _ballistaScene.setActive(true);
            break;
        case LOOKOUT:
            _lookoutScene.setActive(true);
            break;
        case REPAIR:
            _repairScene.setActive(true);
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
void HelloApp::draw() {
    // This takes care of begin/end
    if(_currscene==OVERWORLD) {
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
}
