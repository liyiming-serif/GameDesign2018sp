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
#include <vector>
#include <cugl/assets/CUAsset.h>
#include "GameModel.h"

using namespace cugl;

class JSONReader : public Asset{
private:
    /** Whether or not this input is active */
    bool _active;
    std::shared_ptr<cugl::JsonReader> _reader;
    std::shared_ptr<cugl::JsonValue> _json;
    std::shared_ptr<cugl::JsonValue> _modeJSON;
    std::shared_ptr<cugl::JsonValue> _levelJSON;
    std::shared_ptr<cugl::JsonValue> _roomsJSON;
    std::shared_ptr<cugl::JsonValue> _enemiesJSON;
    std::shared_ptr<cugl::JsonValue> _singleEnemyJSON;
    std::vector<std::vector<float>> _enemyArray;
    std::vector<float> _enemy;
protected:


public:
    /**
	* Creates a new game level with no source file.
	*
	* The source file can be set at any time via the setFile() method. This method
	* does NOT load the asset.  You must call the load() method to do that.
	*
	* @return  an autoreleased level file
	*/
	static std::shared_ptr<JSONReader> alloc() {
		std::shared_ptr<JSONReader> result = std::make_shared<JSONReader>();
		return (result->init("") ? result : nullptr);
	}

	/**
	* Creates a new game level with the given source file.
	*
	* This method does NOT load the level. You must call the load() method to do that.
	* This method returns false if file does not exist.
	*
	* @return  an autoreleased level file
	*/
	static std::shared_ptr<JSONReader> alloc(std::string file) {
		std::shared_ptr<JSONReader> result = std::make_shared<JSONReader>();
		return (result->init(file) ? result : nullptr);
	}

    //CONSTRUCTORS
    JSONReader();

    virtual bool preload(const std::string& file) override;
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

    //DESTRUCTORS
    virtual ~JSONReader() { dispose(); }

    /**
     * Deactivates this jsonreader, releasing all listeners.
     * This method will not dispose of the jsonreader. It can be reused
     * once it is reinitialized.
     */
    void dispose();

    //GAMEPLAY
    bool isActive( ) const { return _active; }

    std::vector<std::vector<float>> readJSON(int players, int level);

    //next two methods get the Type and Health of the enemy based on the name
    float getType(std::string name);

    float getHealth(std::string name);

    void update(float deltaTime);


};

#endif //BUILD_ANDROID_JSONREADER_H
