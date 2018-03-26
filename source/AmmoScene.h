//
//  AmmoScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AmmoScene_h
#define AmmoScene_h

#include <stdio.h>
#include "GameModel.h"
#include "InputController.h"
#include <cugl/cugl.h>

class AmmoScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _ammoTOcastle;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    
    
    
public:
    // Constructors
    AmmoScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~AmmoScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active);
    int switchscene;
};

#endif /* AmmoScene_h */
