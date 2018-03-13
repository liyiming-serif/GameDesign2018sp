//
//  RepairScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 3/9/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef RepairScene_h
#define RepairScene_h

#include <stdio.h>
#include <cugl/cugl.h>

class RepairScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _overworld_button3;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    
    
    
public:
    // Constructors
    RepairScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~RepairScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active);
    int switchscene;
};



#endif /* RepairScene_h */
