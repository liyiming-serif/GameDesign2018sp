// Animation Controller for the Ballista.
//
// Created by Yiming on 3/26/2018.
//

#ifndef BUILD_ANDROID_BALLISTAMODEL_H
#define BUILD_ANDROID_BALLISTAMODEL_H

#include <cugl/cugl.h>

class BallistaModel{
private:
	//I don't completely understand Walker's rationale, but do this for every non-singleton model.
	CU_DISALLOW_COPY_AND_ASSIGN(BallistaModel);

public:
	/**
	* Constructors
	*/
	BallistaModel(void) : _power(0.0f), _angle((float)M_PI/2.0f), isReadyToFire(true) { }

	//Alloc calls init, and returns a reference to this arrow. In scene controllers, call alloc.
	static std::shared_ptr<BallistaModel> alloc(cugl::Vec2 position, std::shared_ptr<cugl::AssetManager>& assets) {
		std::shared_ptr<BallistaModel> ref = std::make_shared<BallistaModel>();
		return (ref->init(position, assets) ? ref : nullptr);
	}

	bool init(cugl::Vec2 position, const std::shared_ptr<cugl::AssetManager>& assets);


	/**
	* Gameplay Functions
	*/
	void update(float deltaTime, bool hasAmmo);

	// Assume assets are already loaded, and _node is immutable after init
	const std::shared_ptr<cugl::AnimationNode> getNode() const { return _node; }

	float getAngle() { return _angle; }
	void setAngle(float angle);

	float getPower() { return _power; }
	void setPower(float power, bool instantaneous);

	cugl::Vec2 getPosition() { return getNode()->getPosition(); }

	//set this at the beginning of drag
	bool isReadyToFire;

	/**
	* Destructors
	*/
	void dispose();

	~BallistaModel() { dispose(); }


protected:
	/**
	* Private members
	*/

	//This is the root scene node that corresponds to this model.
	std::shared_ptr<cugl::AnimationNode> _node;

	float _angle;

	float _power;

};
#endif //BUILD_ANDROID_BALLISTAMODEL_H
