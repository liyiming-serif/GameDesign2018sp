//
// JSONReader.h
// Chaos Castle
//
// Reads the JSON files
//
// Created by Josh on 3/27/2018.
//

#ifndef BUILD_ANDROID_JSONREADER_H
#define BUILD_ANDROID_JSONREADER_H

#include <cugl/cugl.h>
#include <cugl/assets/CUJsonLoader.h>

class JSONReader {
private:
    /** Whether or not this input is active */
    bool _active;

protected:


public:
    //variables

    //CONSTRUCTORS
    JSONReader(){};

    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    //DESTRUCTORS
    ~JSONReader() { dispose(); }

    /**
     * Deactivates this input controller, releasing all listeners.
     * This method will not dispose of the input controller. It can be reused
     * once it is reinitialized.
     */
    void dispose();

    //GAMEPLAY
    bool isActive( ) const { return _active; }

    void update(float deltaTime);


};

#endif //BUILD_ANDROID_SPAWNCONTROLLER_H
