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
#include <cugl/cugl.h>

class MageScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _mageTOcastle;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    
    
    
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
};

#endif /* MageScene_h */
