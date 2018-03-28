//
//  LobbyScene.h
//  ChaosCastle
//
//  Created by Noah Sterling on 3/25/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LobbyScene_h
#define LobbyScene_h

#include <stdio.h>
#include "InputController.h"
#include "GameModel.h"
#include <cugl/cugl.h>

class LobbyScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _menuButton;
    std::shared_ptr<cugl::Button> _createButton;
    std::shared_ptr<cugl::Button> _enterButton;
    std::shared_ptr<cugl::PolygonNode> _avatar1;
    std::shared_ptr<cugl::PolygonNode> _avatar2;
    std::shared_ptr<cugl::Button> _avatar3;
    std::shared_ptr<cugl::Button> _avatar4;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    std::shared_ptr<cugl::PolygonNode> _player1;
    std::shared_ptr<cugl::PolygonNode> _player2;
    std::shared_ptr<cugl::PolygonNode> _waiting;

    
    
public:
    // Constructors
    LobbyScene() : Scene() {}
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    // Destructors
    void dispose();
    ~LobbyScene() {dispose();}
    
    // Gameplay
    void update(float timestep);
    
    //Pause or Resume
    void setActive(bool active);
    int switchscene;
};


#endif /* LobbyScene_h */
