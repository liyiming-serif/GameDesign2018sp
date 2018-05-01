//
//  MageScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef MageScene_h
#define MageScene_h

#include <stdio.h>
#include "GameModel.h"
#include "InputController.h"
#include <cugl/cugl.h>

using namespace cugl;

class MageScene : public cugl::Scene{
protected:
    cugl::Size _size;
	int _spellTimer;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _mageTOcastle;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    
    std::shared_ptr<cugl::Node>  _hex;
	std::shared_ptr<cugl::Rect> _hexCanvas;
	std::shared_ptr<cugl::PathNode> _spellPath;
	std::vector<cugl::Vec2> _spellPathVertices;
    
    std::shared_ptr<cugl::Button> _northWallButton;
    std::shared_ptr<cugl::Button> _northeastWallButton;
    std::shared_ptr<cugl::Button> _southeastWallButton;
    std::shared_ptr<cugl::Button> _southWallButton;
    std::shared_ptr<cugl::Button> _southwestWallButton;
    std::shared_ptr<cugl::Button> _northwestWallButton;
    
    std::shared_ptr<PolygonNode> plain_floor;
    std::shared_ptr<PolygonNode> northWall_floor;
    std::shared_ptr<PolygonNode> northeastWall_floor;
    std::shared_ptr<PolygonNode> southeastWall_floor;
    std::shared_ptr<PolygonNode> southWall_floor;
    std::shared_ptr<PolygonNode> southwestWall_floor;
    std::shared_ptr<PolygonNode> northwestWall_floor;
    
    // Helpers
	void resetSpellPath();

public:
    // Constructors
    MageScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~MageScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active);
    int switchscene;
    
    void setSide (std::string side);
};

#endif /* MageScene_h */
