//
// EnemyModel.h
//
// EnemyModel is the PHYSICAL representation of an enemy in the oil or ballista
// room. It links the physics engine with the scene graph and handles
// collisions.
//
// Created by Josh on 3/14/2018.
//

#ifndef BUILD_ANDROID_ENEMYMODEL_H
#define BUILD_ANDROID_ENEMYMODEL_H

#include <cugl/cugl.h>

class EnemyModel : public cugl::BoxObstacle{
private:
    //always do this?
    CU_DISALLOW_COPY_AND_ASSIGN(EnemyModel);

public:

    float _dir;
    //constructors
    EnemyModel(void) : BoxObstacle() { }

    static std::shared_ptr<EnemyModel> alloc(cugl::Vec2 pos, float dir, float type, float health, int drawScale,
                                             const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<EnemyModel> ref = std::make_shared<EnemyModel>();
        return (ref->init(pos, dir, type, health, drawScale, assets) ? ref : nullptr);
    }

    bool init(cugl::Vec2 pos, float dir, float type, float health, int drawScale, const std::shared_ptr<cugl::AssetManager>& assets);


    //methods
    void update(float deltaTime) override;

    // Assume assets are already loaded, and _node is immutable after init
    const std::shared_ptr<cugl::AnimationNode> getNode() const { return _node; };

    //destructors
    void dispose();

    ~EnemyModel() {dispose();}

    int getDamage(int type);


protected:
    //private

    //This is the root scene node that corresponds to this model.
    std::shared_ptr<cugl::AnimationNode> _node;

	//pixels per meter measurement. obstacle world * _drawScale = screen size
	int _drawScale;
};

#endif //BUILD_ANDROID_ENEMYMODEL_H
