// OilModel.h
// Animation controller for the oil.
//
// Created by Yiming on 4/18/2018.
//

#ifndef BUILD_ANDROID_OILMODEL_H
#define BUILD_ANDROID_OILMODEL_H

#include <cugl/cugl.h>

class OilModel {
private:
	//I don't completely understand Walker's rationale, but do this for every non-singleton model.
	CU_DISALLOW_COPY_AND_ASSIGN(OilModel);

public:
	/**
	* Constructors
	*/
	OilModel(void) : isReloading(false) { }

	//Alloc calls init, and returns a reference to this arrow. In scene controllers, call alloc.
	static std::shared_ptr<OilModel> alloc(cugl::Vec2 position, std::shared_ptr<cugl::AssetManager>& assets) {
		std::shared_ptr<OilModel> ref = std::make_shared<OilModel>();
		return (ref->init(position, assets) ? ref : nullptr);
	}

	bool init(cugl::Vec2 position, const std::shared_ptr<cugl::AssetManager>& assets);


	/**
	* Gameplay Functions
	*/
	//pass oil cooldown and tilt as a frac of the maximum
	void update(float cooldown, float tilt);

	// Assume assets are already loaded, and _node is immutable after init
	const std::shared_ptr<cugl::AnimationNode> getNode() const { return _node; }

	cugl::Vec2 getPosition() { return getNode()->getPosition(); }

	//empty vat is swinging back after pouring (and before refilling)
	bool isReloading;

	/**
	* Destructors
	*/
	void dispose();

	~OilModel() { dispose(); }


protected:
	/**
	* Private members
	*/

	//This is the root scene node that corresponds to this model.
	std::shared_ptr<cugl::AnimationNode> _node;
};


#endif //BUILD_ANDROID_OILMODEL_H
