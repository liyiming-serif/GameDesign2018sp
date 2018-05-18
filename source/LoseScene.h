//
//  LoseScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 5/2/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LoseScene_h
#define LoseScene_h

#include <stdio.h>
#include "GameModel.h"
#include "InputController.h"
#include "SpawnController.h"
#include <cugl/cugl.h>

class LoseScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _loseTOmenu;
    std::shared_ptr<cugl::Button> _loseReplay;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    std::shared_ptr<cugl::Label> _score;
    
    SpawnController spawncontroller; 
    
    
    
public:
    // Constructors
    LoseScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~LoseScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active);
    int switchscene;


};

#endif /* LoseScene_h */
