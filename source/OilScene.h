//
//  OilScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef OilScene_h
#define OilScene_h

#include <stdio.h>
#include "InputController.h"
#include "GameModel.h"
#include <cugl/cugl.h>

class OilScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _oilTOcastle;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    
    std::shared_ptr<cugl::PolygonNode> N_compass;
    std::shared_ptr<cugl::PolygonNode> NE_compass;
    std::shared_ptr<cugl::PolygonNode> NW_compass;
    std::shared_ptr<cugl::PolygonNode> S_compass;
    std::shared_ptr<cugl::PolygonNode> SE_compass;
    std::shared_ptr<cugl::PolygonNode> SW_compass;
    
    
    
public:
    // Constructors
    OilScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~OilScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active);
    int switchscene;
    
    //Sets the compass image based on the direction input
    void setCompass (int direction);
};


#endif /* OilScene_h */
