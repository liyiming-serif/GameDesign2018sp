//
//  LobbyScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 3/25/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "LobbyScene.h"
#include "GameModel.h"


using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

#define BALLISTA    1
#define OVERWORLD   2
#define LOOKOUT     3
#define REPAIR      4
#define MENU        5
#define AMMO        6
#define MAGE        7
#define OIL         8
#define LEVELS      9
#define LOBBY       10

#define DURATION 40.0f
#define DISTANCE 920
#define REPEATS  3
#define ACT_KEY  "current"

#define FONT    _assets->get<Font>("futura")

bool LobbyScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }
    
    // Set background color
    Application::get()->setClearColor(Color4(255,255,255,255));
    
    // Allocate the manager and the actions
    _actions = ActionManager::alloc();

    switchscene = 0;
    
    _assets = assets;
    
    _lobby = Node::alloc();
    _avatar = Node::alloc();
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("homepage");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    addChild(_background);
    
    
    
    std::shared_ptr<Texture> c_1  = _assets->get<Texture>("cloudL");
    _cloud1 = PolygonNode::allocWithTexture(c_1);
    _cloud1->setScale(0.5625f); // Magic number to rescale asset
    _cloud1->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud1->setPosition(197,447);
    _move1 = MoveTo::alloc(Vec2(1300,447),DURATION);
    
    std::shared_ptr<Texture> c_2  = _assets->get<Texture>("cloudM");
    _cloud2 = PolygonNode::allocWithTexture(c_2);
    _cloud2->setScale(0.5625f); // Magic number to rescale asset
    _cloud2->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud2->setPosition(1013,496);
    _move2 = MoveTo::alloc(Vec2(1300,496),DURATION/5);
    
    std::shared_ptr<Texture> c_3  = _assets->get<Texture>("cloudS");
    _cloud3 = PolygonNode::allocWithTexture(c_3);
    _cloud3->setScale(0.5625f); // Magic number to rescale asset
    _cloud3->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud3->setPosition(596,500);
    _move3 = MoveTo::alloc(Vec2(1200,500),DURATION/4);
    
    
    _cloud4 = PolygonNode::allocWithTexture(c_3);
    _cloud4->setScale(-0.783f, 0.783f); // Magic number to rescale asset
    _cloud4->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud4->setPosition(740,285);
    _move4 = MoveTo::alloc(Vec2(1200,285),DURATION/4);
    
    _cloud5 = PolygonNode::allocWithTexture(c_3);
    _cloud5->setScale(0.43); // Magic number to rescale asset
    _cloud5->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud5->setPosition(464,52);
    _move5 = MoveTo::alloc(Vec2(1150,52),DURATION/3);
    
    _cloud6 = PolygonNode::allocWithTexture(c_2);
    _cloud6->setScale(0.5625f); // Magic number to rescale asset
    _cloud6->setAnchor(Vec2::ANCHOR_CENTER);
    _cloud6->setPosition(-200,185);
    _move6 = MoveTo::alloc(Vec2(1250,185),DURATION/1.5);
    
    
    addChild(_cloud1);
    addChild(_cloud2);
    addChild(_cloud3);
    addChild(_cloud4);
    addChild(_cloud5);
    addChild(_cloud6);

    
    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> menu_tex   = _assets->get<Texture>("back");
    _backButton = Button::alloc(PolygonNode::allocWithTexture(menu_tex));
    _backButton->setScale(.5f); // Magic number to rescale asset

    // Create a callback function for the Menu button
    _backButton->setName("menu");
    _backButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = MENU;
        }
    });
    
    
    
    std::shared_ptr<Texture> tex_box   = _assets->get<Texture>("lobby_box");
    _box = PolygonNode::allocWithTexture(tex_box);
    _box->setScale(.6f); // Magic number to rescale asset
    _box->setAnchor(Vec2::ANCHOR_CENTER);
    _box->setPosition(_size.width/2, _size.height/2);
    
    
    // Create the HOST button.  A button has an up image and a down image
    std::shared_ptr<Texture> tex_1   = _assets->get<Texture>("create_button");
    _createButton = Button::alloc(PolygonNode::allocWithTexture(tex_1));
    _createButton->setScale(.6f); // Magic number to rescale asset
    
    
    _createButton->setAnchor(Vec2::ANCHOR_CENTER);
    _createButton->setPosition(_size.width/2, 70);
    

    
    
    
    
    _lobby->addChild(_box);
    _lobby->addChild(_createButton);
    
    

    std::shared_ptr<Texture> tex_p1   = _assets->get<Texture>("player_1");
    _player1 = PolygonNode::allocWithTexture(tex_p1);
    _player1->setScale(.60f); // Magic number to rescale asset
    _player1->setAnchor(Vec2::ANCHOR_CENTER);
    _player1->setPosition(_size.width/8, _size.height/4);

    std::shared_ptr<Texture> tex_p2   = _assets->get<Texture>("player_2");
    _player2 = PolygonNode::allocWithTexture(tex_p2);
    _player2->setScale(1.3f); // Magic number to rescale asset
    _player2->setAnchor(Vec2::ANCHOR_CENTER);
    _player2->setPosition((_size.width/8)*3, _size.height/4);

    std::shared_ptr<Texture> tex_w   = _assets->get<Texture>("waiting");
    _waiting = PolygonNode::allocWithTexture(tex_w);
    _waiting->setScale(1.0f); // Magic number to rescale asset
    _waiting->setAnchor(Vec2::ANCHOR_CENTER);
    _waiting->setPosition(_size.width - _size.width/4,_size.height - _size.height/6 - _size.height/7);

    std::shared_ptr<Texture> tex_a1   = _assets->get<Texture>("avatar_1");
    _avatar1 = PolygonNode::allocWithTexture(tex_a1);
    _avatar1->setScale(.8f); // Magic number to rescale asset
    _avatar1->setAnchor(Vec2::ANCHOR_CENTER);
    _avatar1->setPosition(_size.width/8,_size.height - _size.height/3);

    std::shared_ptr<Texture> tex_a2   = _assets->get<Texture>("avatar_2");
    _avatar2 = PolygonNode::allocWithTexture(tex_a2);
    _avatar2->setScale(.8f); // Magic number to rescale asset
    _avatar2->setAnchor(Vec2::ANCHOR_CENTER);
    _avatar2->setPosition((_size.width/8)*3,_size.height - _size.height/6 - _size.height/8);
    
    
    _avatar->addChild(_player1);
    _avatar->addChild(_player2);
    _avatar->addChild(_avatar1);
    _avatar->addChild(_avatar2);

    std::shared_ptr<Texture> tex_p3   = _assets->get<Texture>("player_3");
    _player3 = PolygonNode::allocWithTexture(tex_p3);
    _player3->setScale(1.3f); // Magic number to rescale asset
    _player3->setAnchor(Vec2::ANCHOR_CENTER);
    _player3->setPosition((_size.width/8)*5, _size.height/4);

    std::shared_ptr<Texture> tex_p4   = _assets->get<Texture>("player_4");
    _player4 = PolygonNode::allocWithTexture(tex_p4);
    _player4->setScale(1.3f); // Magic number to rescale asset
    _player4->setAnchor(Vec2::ANCHOR_CENTER);
    _player4->setPosition((_size.width/8)*7, _size.height/4);

    std::shared_ptr<Texture> tex_a3   = _assets->get<Texture>("avatar_3");
    _avatar3 = PolygonNode::allocWithTexture(tex_a3);
    _avatar3->setScale(0.8f); // Magic number to rescale asset
    _avatar3->setAnchor(Vec2::ANCHOR_CENTER);
    _avatar3->setPosition((_size.width/8)*5,_size.height - _size.height/3);

    std::shared_ptr<Texture> tex_a4   = _assets->get<Texture>("avatar_4");
    _avatar4 = PolygonNode::allocWithTexture(tex_a4);
    _avatar4->setScale(0.8f); // Magic number to rescale asset
    _avatar4->setAnchor(Vec2::ANCHOR_CENTER);
    _avatar4->setPosition((_size.width/8)*7,_size.height - _size.height/6 - _size.height/8);


    _avatar->addChild(_player3);
    _avatar->addChild(_player4);
    _avatar->addChild(_avatar3);
    _avatar->addChild(_avatar4);

    _player2->setVisible(false);
    _player3->setVisible(false);
    _player4->setVisible(false);
    _avatar2->setVisible(false);
    _avatar3->setVisible(false);
    _avatar4->setVisible(false);

    
    // Create the play button.  A button has an up image and a down image
    std::shared_ptr<Texture> play_up   = _assets->get<Texture>("levels");
    _levelsButton = Button::alloc(PolygonNode::allocWithTexture(play_up));
    _levelsButton->setScale(0.5f); // Magic number to rescale asset
    
    
    // Create a callback function for the play button
    _levelsButton->setName("levels");
    _levelsButton->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
#if CU_PLATFORM == CU_PLATFORM_ANDROID
            serverStopAccepting();
#endif
#if CU_PLATFORM == CU_PLATFORM_ANDROID
            if (gameModel.isServer() && gameModel.isNetworked()) {
                int message = sendState(return_buffer(produceACKServer(1)));
                int cleared = clearServerACKs();
                (cleared == 0) ? CULog("Server clear success"):CULog("Server clear failure");
                (message == 0) ? CULog("Level write success"):CULog("Level write failure");
            }
#endif
        }
    });

    // Create a callback function for HOST
    _createButton->setName("create");
    _createButton->setListener([=] (const std::string& name, bool down) {
        if (!down) {
#if CU_PLATFORM == CU_PLATFORM_ANDROID
			setupBluetoothServer();
            gameModel.setServer(true);
            gameModel.setNetworked(true);
            gameModel.setNoPlayers(1);
            //gameModel.setPlayerAvatar(0, 1);
            LobbyScene::changeCanvas("avatar");
            CULog("create");
#endif
        }
    });

//    std::shared_ptr<Texture> tex_refresh   = _assets->get<Texture>("refresh");
//    _refreshButton = Button::alloc(PolygonNode::allocWithTexture(play_up));
//    _refreshButton->setScale(1.0f); // Magic number to rescale asset
//
//    // Create a callback function for HOST
//    _refreshButton->setName("refresh");
//    _refreshButton->setListener([=] (const std::string& name, bool down) {
//        if (!down) {
//            refreshRooms();
//            CULog("refresh");
//        }
//    });
//
//    _refreshButton->setAnchor(Vec2::ANCHOR_CENTER);
//    _refreshButton->setPosition(40, 40);

    _backButton->setAnchor(Vec2::ANCHOR_CENTER);
    _backButton->setPosition(40,575);
    
    _levelsButton->setAnchor(Vec2::ANCHOR_CENTER);
    _levelsButton->setPosition(_size.width-50, 50);
    
    // Add the logo and button to the scene graph
    addChild(_backButton);
    addChild(_lobby);
    addChild(_avatar);
    _avatar->addChild(_levelsButton);
//    addChild(_refreshButton);
    
    // We can only activate a button AFTER it is added to a scene
    _backButton->activate(input.generateKey("backMULTIButton"));
    _createButton->activate(input.generateKey("createButton"));
//    _refreshButton->activate(input.generateKey("refreshButton"));
    input.generateKey("levelsMULTIButton");
    input.generateKey("_enterButton0");
    input.generateKey("_enterButton1");
    input.generateKey("_enterButton2");
    input.generateKey("_enterButton3");
    input.generateKey("_enterButton4");

    length = 0;

    clientReady = true;
    
    return true;
}

void LobbyScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _backButton = nullptr;
        _createButton = nullptr;
        _refreshButton = nullptr;
        for(int i = 0; i < length; i++) {
            _enterButtons[i] = nullptr;
            _enterTexts[i] = nullptr;
        }
        _background = nullptr;
        _cloud1 = nullptr;
        _cloud2 = nullptr;
        _cloud3 = nullptr;
        _cloud4 = nullptr;
        _cloud5 = nullptr;
        _cloud6 = nullptr;
        _active = false;
    }
}

void LobbyScene::update(float timestep){

    animateClouds();

#if CU_PLATFORM == CU_PLATFORM_ANDROID
    if (!gameModel.isServer() && gameModel.isNetworked() && !clientConnected) {
        if (isConnected()) {
            LobbyScene::changeCanvas("avatar");
            clientConnected = true;
        }
        else {
            //Animate waiting for connection
        }
    }

    if (_avatar->isVisible()) {
        runLobbyNetworking();
    }

    if (_avatar->isVisible() && gameModel.isServer()) {
        gameModel.setNoPlayers(getPlayers());
    }

    if (gameModel.isServer()) {
        if (gameModel.getNoPlayers()>3) {
            _player4->setVisible(true);
            _avatar4->setVisible(true);
        }
        if (gameModel.getNoPlayers()>2) {
            _player3->setVisible(true);
            _avatar3->setVisible(true);
        }
        if (gameModel.getNoPlayers()>1) {
            _player2->setVisible(true);
            _avatar2->setVisible(true);
        }
    }

    if (LobbyClock == 50) {
        if (!serverDevices.empty()) {
            for(int i = 0; i < serverDevices.size(); i++) {
                _enterButtons[i]->dispose();
                _enterTexts[i]->dispose();
            }
        }

        serverDevices = getServerDevices();

        // Create new enter buttons/text if canvas is lobby
        if (!_avatar->isVisible() && !serverDevices.empty()) {
            length = serverDevices.size();
            _enterButtons = new std::shared_ptr<cugl::Button>[length];
            _enterTexts = new std::shared_ptr<cugl::Label>[length];

            for (int i = 0; i < length; ++i) {
                _enterButtons[i] = createServerRoomButton(i);
                _enterTexts[i] = createServerRoomText(i);
                setButtonActive(_enterButtons[i], _enterButtons[i]->getName());
            }
        }
        LobbyClock = 0;
    }
    LobbyClock++;
#endif

    setButtonActive(_createButton,"createButton");

    _actions->update(timestep);
}

void LobbyScene::setButtonActive ( std::shared_ptr<cugl::Button> button, std::string actKey){
    if (button->getParent()->isVisible()) {
        if (!button->isActive()) {
            button->activate(input.findKey(actKey));
        }
    }
    else {
        if (button->isActive()){
            button->deactivate();
        }
    }
}

void LobbyScene::changeCanvas(std::string canvas) {
    if (canvas=="avatar"){
        _lobby->setVisible(false);
        _avatar->setVisible(true);
        if (gameModel.isServer()) {
            _levelsButton->activate(input.findKey("levelsMULTIButton"));
        }
        else {
            _levelsButton->deactivate();
            _levelsButton->setVisible(false);
        }
        _createButton->deactivate();
        for(int i = 0; i < length; i++) {
            _enterButtons[i]->deactivate();
        }
        //CULog("go to avatar");
    }
    else {
        return;
    }
        
}

void LobbyScene::doMove(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY, action, object, fcn);
}

void LobbyScene::doMove2(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+2, action, object, fcn);
}

void LobbyScene::doMove3(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+3, action, object, fcn);
}

void LobbyScene::doMove4(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+4, action, object, fcn);
}

void LobbyScene::doMove5(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+5, action, object, fcn);
}

void LobbyScene::doMove6(const std::shared_ptr<MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object) {
    auto fcn = EasingFunction::alloc(EasingFunction::Type::LINEAR);
    _actions->activate(ACT_KEY+6, action, object, fcn);
}


//Pause or Resume
void LobbyScene::setActive(bool active){
    _active = active;
    switchscene = 0;
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(255,255,255,255));
        _backButton->activate(input.findKey("backMULTIButton"));
        _levelsButton->activate(input.findKey("levelsMULTIButton"));
        _lobby->setVisible(true);
        _avatar->setVisible(false);
        _createButton->activate(input.findKey("createButton"));
    }
    else{
        _backButton->deactivate();
        _createButton->deactivate();
        for(int i = 0; i < length; i++) {
            _enterButtons[i]->deactivate();
        }
        _levelsButton->deactivate();
    }
}

std::shared_ptr<cugl::Button> LobbyScene::createServerRoomButton(int device) {
    // Create the JOIN button.  A button has an up image and a down image
    std::shared_ptr<Texture> tex_2   = _assets->get<Texture>("enter_button");
    std::shared_ptr<cugl::Button> _enterButton = Button::alloc(PolygonNode::allocWithTexture(tex_2));
    _enterButton->setScale(.6f); // Magic number to rescale asset

    _enterButton->setAnchor(Vec2::ANCHOR_CENTER);
    _enterButton->setPosition(_size.width/2, 500 - (device * 100));



    // Create a callback function for the JOIN button
    _enterButton->setName("_enterButton" + to_string(device));
    _enterButton->setListener([=] (const std::string& name, bool down) {
        if (!down) {
#if CU_PLATFORM == CU_PLATFORM_ANDROID
            setupBluetoothClient(device);
#endif
            gameModel.setNetworked(true);
            gameModel.setServer(false);
            //int roomOccup = stoi(to_string(serverDevices.at(device).at(6)));
            gameModel.setPlayerAvatar(1, 2);
            _player2->setVisible(true);
            _avatar2->setVisible(true);
            CULog("enter");
        }
    });
    _lobby->addChild(_enterButton);
    _enterButton->activate(input.findKey("_enterButton"+to_string(device)));
    return _enterButton;
}

std::shared_ptr<cugl::Label> LobbyScene::createServerRoomText(int device) {
    std::shared_ptr<cugl::Label> _buttonText = Label::alloc((std::string) "                                                                        ", FONT);
    _enterButtons[device]->addChild(_buttonText);
    _buttonText->setAnchor(Vec2::ANCHOR_CENTER);
    _buttonText->setPosition(_enterButtons[device]->getContentWidth()/2,_enterButtons[device]->getContentHeight()/2);
    _buttonText->setForeground(cugl::Color4(233,225,212,255));
    std::string roomName = serverDevices.at(device);
    //CULog("%s", roomName.c_str());
    _buttonText->setText(roomName);
    return _buttonText;
}

void LobbyScene::refreshRooms() {

}

void LobbyScene::animateClouds() {
    // Animate
    if (!_actions->isActive(ACT_KEY) && move1 ){
        doMove(_move1, _cloud1);
        move1=false;
    }
    else if (!_actions->isActive(ACT_KEY) && !move1 ){
        _cloud1->setPosition(-500,447);
        _move1 = MoveTo::alloc(Vec2(1300,447),1.5*DURATION);
        doMove(_move1, _cloud1);
    }


    if (!_actions->isActive(ACT_KEY+2) && move2 ){
        doMove2(_move2, _cloud2);
        move2=false;
    }
    else if (!_actions->isActive(ACT_KEY+2) && !move2 ){
        _cloud2->setPosition(-400,496);
        _move2 = MoveTo::alloc(Vec2(1300,496),DURATION);
        doMove2(_move2, _cloud2);
    }


    if (!_actions->isActive(ACT_KEY+3) && move3 ){
        doMove3(_move3, _cloud3);
        move3=false;
    }
    else if (!_actions->isActive(ACT_KEY+3) && !move3 ){
        _cloud3->setPosition(-200,500);
        _move3 = MoveTo::alloc(Vec2(1200,500),DURATION/1.5);
        doMove3(_move3, _cloud3);
    }


    if (!_actions->isActive(ACT_KEY+4) && move4 ){
        doMove4(_move4, _cloud4);
        move4=false;
    }
    else if (!_actions->isActive(ACT_KEY+4) && !move4 ){
        _cloud4->setPosition(-150,285);
        _move4 = MoveTo::alloc(Vec2(1200,285),DURATION/2);
        doMove4(_move4, _cloud4);
    }


    if (!_actions->isActive(ACT_KEY+5) && move5 ){
        doMove5(_move5, _cloud5);
        move5=false;
    }
    else if (!_actions->isActive(ACT_KEY+5) && !move5 ){
        _cloud5->setPosition(-75,52);
        _move5 = MoveTo::alloc(Vec2(1150,52),DURATION/3);
        doMove5(_move5, _cloud5);
    }

    if (!_actions->isActive(ACT_KEY+6) && move6 ){
        doMove6(_move6, _cloud6);
        move6=false;
    }
    else if (!_actions->isActive(ACT_KEY+6) && !move6 ){
        _cloud6->setPosition(-200,185);
        _move6 = MoveTo::alloc(Vec2(1250,185),DURATION/1.5);
        doMove6(_move6, _cloud6);
    }
}

#if CU_PLATFORM == CU_PLATFORM_ANDROID
void LobbyScene::runLobbyNetworking() {
    if (gameModel.isNetworked()) {
        if (gameModel.isServer() && LobbyClock2 == 10) {
            //TODO: Read from network
            //Prints the messages from the clients
            char **read_buffers = consumeACKServer();

            bool nothingToRead = false;
            for (int l = 0; l<gameModel.getNoPlayers()-1; l++) {
                if (read_buffers[l] == NULL) {
                    nothingToRead = true;
                }
            }
            if (!nothingToRead) {
                applyACKServer(read_buffers);
            }

            for (int l = 0; l<gameModel.getNoPlayers()-1; l++) {
                //delete[] read_buffers[l];
            }

            char *write_byte_buffer = return_buffer(produceACKServer(0));

            CULog("State Change %s \n", write_byte_buffer);

            if (sendState(write_byte_buffer) == 1){
                CULog("At least one write failure");
            } else {
                CULog("Write success");
            }
            LobbyClock2 = 0;
        }
        else if (!gameModel.isServer() && LobbyClock2 == 0) {
            //TODO: Read from network
            char *read_buffer = consumeACKClient();
            CULog("Read Server State: %s \n", read_buffer);
            if (read_buffer != NULL) {
                applyACKClient(read_buffer);
            }
            //delete[] read_buffer;
            //CULog("Lobby Clock in client read %i", LobbyClock2);
            LobbyClock2++;
        }
        else if (!gameModel.isServer() && LobbyClock2 == 10) {
            if (!clientACKSent) {
                char *write_byte_buffer = return_buffer(produceACKClient());
                //TODO: Write to network
                CULog("State Change: %s \n", write_byte_buffer);
                if (sendState(write_byte_buffer) == 1){
                    CULog("At least one write failure");
                } else {
                    CULog("Write success");
                }
                delete[] write_byte_buffer;
            }
            //CULog("Lobby Clock in client write %i", LobbyClock2);
            LobbyClock2 = 0;
        }
        else {
            LobbyClock2++;
        }
    }
}
#endif

#if CU_PLATFORM == CU_PLATFORM_ANDROID

std::string LobbyScene::produceACKClient() {
    if(clientReady) {
        clientACKSent = true;
        return "3|1";
    } else {
        return "3|0";
    }
}

std::string LobbyScene::produceACKServer(int mode) {
    if (mode == 1) {
        return "8|" + to_string(gameModel.getNoPlayers())+"|1|11";
    }
    else {
        if(serverReady) {
            return "8|" + to_string(gameModel.getNoPlayers())+"|1|-1";
        } else {
            return "8|" + to_string(gameModel.getNoPlayers())+"|0|-1";
        }
    }

}

char* LobbyScene::consumeACKClient() {
    return consumeACK();
}

char** LobbyScene::consumeACKServer() {
    char *tmp[gameModel.getNoPlayers()-1];
    for (int i = 0; i < gameModel.getNoPlayers()-1; ++i) {
        tmp[i] = consumeACK();
        //CULog("State from Client %i: %s", i, tmp[i]);
    }
    return tmp;
}

void LobbyScene::applyACKClient(char *ACK) {
    char* copy = strdup(ACK);
    const char s[2] = "|";
    char* messageSize;
    char* numPlayers;
    char* serverStatus;
    char* serverLevel;
    char* token;
    token = strtok(copy, s);
    messageSize = token;
    token = strtok(NULL, s);
    numPlayers = token;
    token = strtok(NULL, s);
    serverStatus = token;
    token = strtok(NULL, s);
    serverLevel = token;
    gameModel.setNoPlayers(std::stoi(numPlayers));
    if (!obtainedID) {
        gameModel.setPlayerID(gameModel.getNoPlayers());
        CULog("Player ID: %i", gameModel.getPlayerID());
        obtainedID = true;
    }
    int sLevel = std::stoi(serverLevel);
    CULog("received level %s", serverLevel);
    if (sLevel > 0) {
        switchscene = OVERWORLD;
    }
}

void LobbyScene::applyACKServer(char **ACKs) {
    bool allReady = true;
    for (int i = 0; i < gameModel.getNoPlayers()-1; ++i) {
        if (ACKs[i]!=NULL) {
            int ready = std::stoi(ACKs[i]);
            if (ready == 0) allReady = false;
        } else {
            allReady = false;
        }
    }
    allReady ? _levelsButton->activate(input.findKey("levelsMULTIButton")):_levelsButton->deactivate();
}

char* LobbyScene::return_buffer(const std::string &string) {
    char* return_string = new char[string.length() + 1];
    strcpy(return_string, string.c_str());
    return return_string;
}

#endif
