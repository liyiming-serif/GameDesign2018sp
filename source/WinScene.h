//
//  WinScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 5/2/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef WinScene_h
#define WinScene_h

#include <stdio.h>
#include "GameModel.h"
#include "InputController.h"
#include <cugl/cugl.h>

class WinScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _winTOmenu;
    std::shared_ptr<cugl::Button> _winReplay;
    std::shared_ptr<cugl::Button> _winNext;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    
    
    
public:
    // Constructors
    WinScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~WinScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    bool replayFlag;
    
    //Pause or Resume
    void setActive(bool active);
    int switchscene;
};

#endif /* WinScene_h */
