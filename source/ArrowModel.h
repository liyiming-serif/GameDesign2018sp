//
// ArrowModel.h
// Chaos Castle
//
// An ArrowModel represents a single arrow fired by the ballista.
// Game object models extend a Box2D class and update their own node in the scene graph.
// They effectively connect physics objects to scene nodes.
//
// Created by Yiming on 3/9/2018.
//

#ifndef BUILD_ANDROID_ARROWMODEL_H
#define BUILD_ANDROID_ARROWMODEL_H

#include <cugl/2d/physics/CUCapsuleObstacle.h>
#include <cugl/cugl.h>

class ArrowModel : public cugl::CapsuleObstacle{
private:
    //I don't completely understand Walker's rationale, but do this for every non-singleton model.
    CU_DISALLOW_COPY_AND_ASSIGN(ArrowModel);

public:
    /**
     * Constructors
     */
    ArrowModel(void) : CapsuleObstacle() { }

    //Alloc calls init, and returns a reference to this arrow. In scene controllers, call alloc.
    static std::shared_ptr<ArrowModel> alloc(cugl::Vec2 pos, float speed, float dir, int drawScale,
                                             const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<ArrowModel> ref = std::make_shared<ArrowModel>();
        return (ref->init(pos, speed, dir, drawScale, assets) ? ref : nullptr);
    }

    bool init(cugl::Vec2 pos, float speed, float dir, int drawScale, const std::shared_ptr<cugl::AssetManager>& assets);


    /**
     * Gameplay Functions
     */
    void update(float deltaTime) override;

    // Assume assets are already loaded, and _node is immutable after init
    const std::shared_ptr<cugl::PolygonNode> getNode() const { return _node; }


    /**
     * Destructors
     */
    void dispose();

    ~ArrowModel() {dispose();}

	//Flag for if arrow has already collided with enemy
	bool collided;

protected:
    /**
     * Private members
     */

    //This is the root scene node that corresponds to this model.
    std::shared_ptr<cugl::PolygonNode> _node;

	//pixels per meter measurement. obstacle world * _drawScale = screen size
	int _drawScale;
};

#endif //BUILD_ANDROID_ARROWMODEL_H
