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
    CU_DISALLOW_COPY_AND_ASSIGN(EnemyModel);

public:
    //CONSTRUCTORS
    EnemyModel(void) : BoxObstacle() { }

    static std::shared_ptr<EnemyModel> alloc(std::string name, cugl::Vec2 pos,
											int type, int drawScale,
											const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<EnemyModel> ref = std::make_shared<EnemyModel>();
        return (ref->init(name, pos, type, drawScale, assets) ? ref : nullptr);
    }

    bool init(std::string name, cugl::Vec2 pos, int type,
		int drawScale, const std::shared_ptr<cugl::AssetManager>& assets);


    //GAMEPLAY
    void update(float deltaTime) override;
	//For Berserkers: multiply walking speed as they become more enraged
	float scaleAnimSp(float factor);
	void setAtkProgress(float p) { _atkProgress = p; }

	//DEATH ANIMATION BAFFOONERY 
	bool isDying;
	bool doneDying;

    // Assume assets are already loaded, and _node is immutable after init
    const std::shared_ptr<cugl::AnimationNode> getNode() const { return _node; }

	const std::string getName() const { return _name; }

    //DESTRUCTORS
    void dispose();

    ~EnemyModel() {dispose();}


protected:
	//ANIMATION CONSTANTS
	int _rows;
	int _cols;
	int _offset;
	int _walkFrameStart;
	int _dieFrameStart;
	int _attackFrameStart;
	float _currFrame;
	float _walkAnimSp;
	float _dieAnimSp;
	//atkCounter/atkSpeed. Used to set attack animation
	float _atkProgress;

    //MEMBERS
	std::string _name;

    //This is the root scene node that corresponds to this model.
    std::shared_ptr<cugl::AnimationNode> _node;

	//pixels per meter measurement. obstacle world * _drawScale = screen size
	int _drawScale;

};

#endif //BUILD_ANDROID_ENEMYMODEL_H
