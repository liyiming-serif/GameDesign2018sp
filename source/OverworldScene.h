//
//  OverworldScene.h
//  HelloWorld
//
//  Created by Noah Sterling on 3/1/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef OverworldScene_h
#define OverworldScene_h

#include <stdio.h>
#include <cugl/cugl.h>


class OverworldScene : public cugl::Scene{
protected:
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    //bakground floorplan
    std::shared_ptr<cugl::Node>  _background;
    
    // Controllers
    void touchDragCB(const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus);
    
    // Models
    std::shared_ptr<cugl::PolygonNode> _ballista;
    
public:
    // Constructors
    OverworldScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~OverworldScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
};


#endif /* OverworldScene_h */
