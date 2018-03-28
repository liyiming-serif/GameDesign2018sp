//
// Created by Josh on 3/27/2018.
//

#include "JSONReader.h"

using namespace cugl;

/**
 * Creates a spawn controller
 */
bool JSONReader::init(const std::shared_ptr<AssetManager>& assets, const std::string& file){
    _active = true;

    std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(file);
    return reading(reader->readJson());
}

bool JSONReader::reading(const std::shared_ptr<cugl::JsonValue>& json){
    if (json == nullptr) {
		CUAssertLog(false, "Failed to load level file");
		return false;
	}
}

void JSONReader::update(float deltaTime) {

}

void JSONReader::dispose(){
    _active = false;
}