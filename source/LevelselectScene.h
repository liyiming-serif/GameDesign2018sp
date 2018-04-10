//
//  LevelselectScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 4/10/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelselectScene_h
#define LevelselectScene_h

#include "InputController.h"
#include <stdio.h>
#include <cugl/cugl.h>

class LevelselectScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _playButton;
    std::shared_ptr<cugl::Button> _backButton;
    
    std::shared_ptr<cugl::PolygonNode> _background;

    
    
    
public:
    // Constructors
    LevelselectScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~LevelselectScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active, int mode);
    int switchscene;
    int mode;
};

#endif /* LevelselectScene_h */
