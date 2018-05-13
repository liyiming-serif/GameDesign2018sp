//
//  MageScene.cpp
//  ChaosCastle
//
//  Created by Noah Sterling on 3/22/18.
//  Copyright © 2018 Game Design Initiative at Cornell. All rights reserved.
//

#include "MageScene.h"

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
#define WIN         11
#define LOSE        12

#define FLOOR_SCALE    0.55f
#define BUTTON_SCALE    2.0f

#define GESTURE_TIMEOUT 240
#define HEX_CANVAS_SIZE 200 //length of hexagon side

#define DMG_DURATION 1.0f
#define DMG_ACT_KEY "marker"

// Decide when to use heavy damage indicator
#define HVY_DMG 6


bool MageScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;
    
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }

    // Set background color
    Application::get()->setClearColor(Color4(132,180,113,255));
    
    switchscene = 0;

	_spellTimer = GESTURE_TIMEOUT;
    
    _assets = assets;
    
	// Allocate the damage indicators
	std::shared_ptr<Texture> dmg_img = _assets->get<Texture>("dmg_indicator_n");
	std::shared_ptr<PolygonNode> dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_nw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_sw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_s");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_se");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("dmg_indicator_ne");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_n");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_nw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_sw");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_s");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_se");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);
	dmg_img = _assets->get<Texture>("hvydmg_indicator_ne");
	dmg_ind = PolygonNode::allocWithTexture(dmg_img);
	dmg_ind->setScale(_size / dmg_ind->getSize());
	dmg_ind->setAnchor(Vec2::ANCHOR_CENTER);
	dmg_ind->setPosition(_size / 2.0);
	dmg_ind->setColor(Color4::CLEAR);
	dmg_ind->setZOrder(5);
	_dmgIndicators.push_back(dmg_ind);

	_dmgFadeOUT = FadeOut::alloc(DMG_DURATION);

    _hex = Node::alloc();
    _hex = Node::alloc();
    _hex->setScale(.70); // Magic number to rescale asset
    _hex->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
    _hex->setPosition(_size.width/2.75f,_size.height*.06f);
    
    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("mage_background");
    _background = PolygonNode::allocWithTexture(texture);
    _background->setScale(0.5625f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
    addChild(_background);
    

    
    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    _mageTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    _mageTOcastle->setScale(.6f); // Magic number to rescale asset
    
    // Create a callback function for the OVERWORLD button
    _mageTOcastle->setName("mageTOcastle");
    _mageTOcastle->setListener([=] (const std::string& name, bool down) {
        // Only quit when the button is released
        if (!down) {
            switchscene = OVERWORLD;
        }
    });
    
    
    
    
        // Wall Layers
        std::shared_ptr<Texture> plainFloor_texture  = _assets->get<Texture>("mage_hex");
        plain_floor = PolygonNode::allocWithTexture(plainFloor_texture);
        plain_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        plain_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        plain_floor->setPosition(_size.width*.85f,_size.height/1.95);
        addChild(plain_floor);
    
        std::shared_ptr<Texture> northWall_texture  = _assets->get<Texture>("mage_hex_N");
        northWall_floor = PolygonNode::allocWithTexture(northWall_texture);
        northWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        northWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        northWall_floor->setPosition(_size.width*.85f,_size.height/1.95);
        northWall_floor->setVisible(false);
        addChild(northWall_floor);
    
        std::shared_ptr<Texture> northeastWall_texture  = _assets->get<Texture>("mage_hex_NE");
        northeastWall_floor = PolygonNode::allocWithTexture(northeastWall_texture);
        northeastWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        northeastWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        northeastWall_floor->setPosition(_size.width*.85f,_size.height/1.95);
        northeastWall_floor->setVisible(false);
        addChild(northeastWall_floor);
    
        std::shared_ptr<Texture> southeastWall_texture  = _assets->get<Texture>("mage_hex_SE");
        southeastWall_floor = PolygonNode::allocWithTexture(southeastWall_texture);
        southeastWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        southeastWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        southeastWall_floor->setPosition(_size.width*.85f,_size.height/1.95);
        southeastWall_floor->setVisible(false);
        addChild(southeastWall_floor);
    
        std::shared_ptr<Texture> southWall_texture  = _assets->get<Texture>("mage_hex_S");
        southWall_floor = PolygonNode::allocWithTexture(southWall_texture);
        southWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        southWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        southWall_floor->setPosition(_size.width*.85f,_size.height/1.95);
        southWall_floor->setVisible(false);
        addChild(southWall_floor);
    
        std::shared_ptr<Texture> southwestWall_texture  = _assets->get<Texture>("mage_hex_SW");
        southwestWall_floor = PolygonNode::allocWithTexture(southwestWall_texture);
        southwestWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        southwestWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        southwestWall_floor->setPosition(_size.width*.85f,_size.height/1.95);
        southwestWall_floor->setVisible(false);
        addChild(southwestWall_floor);
    
        std::shared_ptr<Texture> northwestWall_texture  = _assets->get<Texture>("mage_hex_NW");
        northwestWall_floor = PolygonNode::allocWithTexture(northwestWall_texture);
        northwestWall_floor->setScale(FLOOR_SCALE); // Magic number to rescale asset
        northwestWall_floor->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        northwestWall_floor->setPosition(_size.width*.85f,_size.height/1.95);
        northwestWall_floor->setVisible(false);
        addChild(northwestWall_floor);
    
    
        // Buttons
        std::shared_ptr<Texture> image_up   = _assets->get<Texture>("mage_transparent");
        _northWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
        _northeastWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
        _southeastWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
        _southWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
        _southwestWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
        _northwestWallButton = Button::alloc(PolygonNode::allocWithTexture(image_up));
    
        // Create a callback function for the Ballista buttons
        _northWallButton->setName("fade in N");
        _northWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down) {
                CULog("NORTH");
                MageScene::setSide("N");
            }
        });
    
        _northeastWallButton->setName("fade in NE");
        _northeastWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down) {
                CULog("NORTHEAST");
                MageScene::setSide("NE");
            }
        });
    
    
        _southeastWallButton->setName("fade in SE");
        _southeastWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down) {
                CULog("SOUTHEAST");
                MageScene::setSide("SE");
            }
        });
        _southWallButton->setName("fade in S");
        _southWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down) {
                CULog("SOUTH");
                MageScene::setSide("S");
            }
        });
        _southwestWallButton->setName("fade in SW");
        _southwestWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down) {
                CULog("SOUTHWEST");
                MageScene::setSide("SW");
            }
        });
        _northwestWallButton->setName("fade in NW");
        _northwestWallButton->setListener([=] (const std::string& name, bool down) {
            // Only switch scenes when the button is released
            if (!down) {
                CULog("NORTHWEST");
                MageScene::setSide("NW");
            }
        });
    
    
    
    
        //Positions the wall Buttons
        float centerX = plain_floor->getContentSize().width/2;
        float centerY = plain_floor->getContentSize().height/2;
        _northWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northWallButton->setPosition(centerX,centerY+.36*plain_floor->getContentHeight());
    
        _northeastWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northeastWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northeastWallButton->setPosition(centerX+.295*plain_floor->getContentWidth(),centerY+.19*plain_floor->getContentHeight());
        _northeastWallButton->setAngle(-M_PI/3);
    
        _southeastWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _southeastWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _southeastWallButton->setPosition(centerX+.290*plain_floor->getContentWidth(),centerY-.187*plain_floor->getContentHeight());
        _southeastWallButton->setAngle(-5*M_PI/3);
    
        _southWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _southWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _southWallButton->setPosition(centerX,centerY-.35*plain_floor->getContentHeight());
    
        _southwestWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _southwestWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _southwestWallButton->setPosition(centerX-.28*plain_floor->getContentWidth(),centerY-.18*plain_floor->getContentHeight());
        _southwestWallButton->setAngle(-4*M_PI/3);
    
        _northwestWallButton->setScale(BUTTON_SCALE); // Magic number to rescale asset
        _northwestWallButton->setAnchor(Vec2::ANCHOR_CENTER);
        _northwestWallButton->setPosition(centerX-.28*plain_floor->getContentWidth(),centerY+.18*plain_floor->getContentHeight());
        _northwestWallButton->setAngle(-2*M_PI/3);
    

    _northWallButton->setVisible(false);
    _northeastWallButton->setVisible(false);
    _northwestWallButton->setVisible(false);
    _southWallButton->setVisible(false);
    _southeastWallButton->setVisible(false);
    _southwestWallButton->setVisible(false);

    
    // Position the overworld button in the bottom left
    _mageTOcastle->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _mageTOcastle->setPosition(15,_size.height-18);
    
    // Add the logo and button to the scene graph
    addChild(_mageTOcastle);
    addChild(_hex);
    
    
    _hex->addChild(_northWallButton);
    _hex->addChild(_northeastWallButton);
    _hex->addChild(_southeastWallButton);
    _hex->addChild(_southWallButton);
    _hex->addChild(_southwestWallButton);
    _hex->addChild(_northwestWallButton);
    
    
    // We can only activate a button AFTER it is added to a scene
    _mageTOcastle->activate(input.generateKey("mageTOcastle"));
    
    _northWallButton->activate(input.generateKey("northWallButton_mage"));
    _northeastWallButton->activate(input.generateKey("northeastWallButton_mage"));
    _southeastWallButton->activate(input.generateKey("southeastWallButton_mage"));
    _southWallButton->activate(input.generateKey("southWallButton_mage"));
    _southwestWallButton->activate(input.generateKey("southwestWallButton_mage"));
    _northwestWallButton->activate(input.generateKey("northwestWallButton_mage"));

	//Alloc the canvas for drawing spells
	float hexheight = HEX_CANVAS_SIZE*sqrtf(3);
	_hexCanvas = std::make_shared<Rect>(_size.width*0.6375f-HEX_CANVAS_SIZE/2.0f,_size.height*0.5125f-hexheight/2.0f,HEX_CANVAS_SIZE,hexheight);

	//Alloc the path node that traces the spell gesture
	_spellPathVertices.clear();
	_spellPath = PathNode::alloc();
	_spellPath->setStroke(2.0f);
	_spellPath->setClosed(false);
	_spellPath->setJoint(PathJoint::ROUND);
	_spellPath->setCap(PathCap::ROUND);
	_spellPath->setColor(Color4(0, 0, 0));
	_spellPath->setAbsolute(true);
	_spellPath->setName("spellPath");
	addChild(_spellPath);

	// Add damage indicators overlay
	for (int i = 0; i < _dmgIndicators.size(); i++) {
		addChild(_dmgIndicators.at(i));
	}

    return true;
}

void MageScene::dispose() {
    if (_active) {
        removeAllChildren();
        _assets = nullptr;
        _mageTOcastle = nullptr;
        _background = nullptr;
        _active = false;
        _hex = nullptr;
		_hexCanvas = nullptr;
		_spellPath->removeAllChildren();
		_spellPath = nullptr;
		_spellPathVertices.clear();
        plain_floor = nullptr;
        northWall_floor = nullptr;
        northeastWall_floor = nullptr;
        northwestWall_floor = nullptr;
        southWall_floor = nullptr;
        southeastWall_floor =nullptr;
        southwestWall_floor = nullptr;
		_dmgFadeOUT = nullptr;
		_dmgIndicators.clear();
    }
}

void MageScene::resetSpellPath() {
	removeChildByName("spellPath");
	_spellPathVertices.clear();
	_spellPath = nullptr;

	_spellPath = PathNode::alloc();
	_spellPath->setStroke(2.0f);
	_spellPath->setClosed(false);
	_spellPath->setJoint(PathJoint::ROUND);
	_spellPath->setCap(PathCap::ROUND);
	_spellPath->setColor(Color4(0, 0, 0));
	_spellPath->setAbsolute(true);
	_spellPath->setName("spellPath");
	addChild(_spellPath);
}

bool MageScene::inHexCanvas(const Vec2& point) {
	Vec2 axisOfRotation = Vec2(_hexCanvas->getMidX(), _hexCanvas->getMidY());
	Vec2 rotCCW = point;
	Vec2 rotCW = point;
	//translate point into axisOfRotation coords
	rotCCW.rotate(M_PI / 3.0f, axisOfRotation);
	rotCW.rotate(-1.0f*M_PI / 3.0f, axisOfRotation);
	
	return _hexCanvas->contains(point)|| _hexCanvas->contains(rotCCW)|| _hexCanvas->contains(rotCW);
}

void MageScene::update(float timestep){
	if (gameModel.getWallHealth(0) == 0 || gameModel.getWallHealth(1) == 0 || gameModel.getWallHealth(2) == 0 ||
		gameModel.getWallHealth(3) == 0 || gameModel.getWallHealth(4) == 0 || gameModel.getWallHealth(5) == 0) {
		switchscene = LOSE;
	}
	if (gameModel._currentTime > gameModel._endTime) {
		if (gameModel._enemyArrayMaster[0].size() == 0 && gameModel._enemyArrayMaster[1].size() == 0 && gameModel._enemyArrayMaster[2].size() == 0 && gameModel._enemyArrayMaster[3].size() == 0 && gameModel._enemyArrayMaster[4].size() == 0 && gameModel._enemyArrayMaster[5].size() == 0) {
			switchscene = WIN;
		}
	}

	//poll damage indicators
	pollDmgIndicators();

	//poll inputs
	if (input.justPressed()) {
		if (inHexCanvas(screenToWorldCoords(input.dTouch()))) {
			_spellPath->setPosition(screenToWorldCoords(input.pointerPos()));
			_minSpellPathX = screenToWorldCoords(input.pointerPos()).x;
			_minSpellPathY = screenToWorldCoords(input.pointerPos()).y;
		}
	}
	if (input.isPressed()) {
		if (_spellTimer <= 0) {
			//Took too long to cast spell
			resetSpellPath();
		}
		else {
			//Trace the attempted spell
			_spellTimer--;
			if (inHexCanvas(screenToWorldCoords(input.pointerPos()))&&inHexCanvas(screenToWorldCoords(input.dTouch()))) {
				//only trace if within bounds
				_spellPathVertices.push_back(screenToWorldCoords(input.pointerPos()));
				_spellPath->setPolygon(_spellPathVertices);
				_minSpellPathX = fminf(_minSpellPathX, screenToWorldCoords(input.pointerPos()).x);
				_minSpellPathY = fminf(_minSpellPathY, screenToWorldCoords(input.pointerPos()).y);
				_spellPath->setPosition(Vec2(_minSpellPathX, _minSpellPathY));
			}
		}
	}
	if (input.justReleased()) {
		//Done casting spell
		resetSpellPath();
		_spellTimer = GESTURE_TIMEOUT;
	}
}

void MageScene::pollDmgIndicators() {
	for (int i = 0; i < 6; i++) {
		if (gameModel.getDmgHealth(i) > 0) {
			//turn on damage indicator for that side
			bool succ;
			if (gameModel.getDmgHealth(i) > HVY_DMG) {
				_dmgIndicators.at(i + 6)->setColor(Color4::WHITE);
				succ = input.actions()->activate(DMG_ACT_KEY + i + 6, _dmgFadeOUT, _dmgIndicators.at(i + 6));
			}
			else {
				_dmgIndicators.at(i)->setColor(Color4::WHITE);
				succ = input.actions()->activate(DMG_ACT_KEY + i, _dmgFadeOUT, _dmgIndicators.at(i));
			}
			if (succ) {
				gameModel.resetWallDmg();
			}
		}
	}
}

void MageScene::setSide(std::string side){
    plain_floor->setVisible(false);
    northWall_floor->setVisible(false);
    northwestWall_floor->setVisible(false);
    northeastWall_floor->setVisible(false);
    southWall_floor->setVisible(false);
    southwestWall_floor->setVisible(false);
    southeastWall_floor->setVisible(false);
    if (side == "N") {
        northWall_floor->setVisible(true);
    }
    else if (side == "NE") {
        northeastWall_floor->setVisible(true);
    }
    else if (side == "NW") {
        northwestWall_floor->setVisible(true);
    }
    else if (side == "S") {
        southWall_floor->setVisible(true);
    }
    else if (side == "SE") {
        southeastWall_floor->setVisible(true);
    }
    else if (side == "SW") {
        southwestWall_floor->setVisible(true);
    }
	else {
		plain_floor->setVisible(true);
	}
}


//Pause or Resume
void MageScene::setActive(bool active){
    _active = active;
    switchscene = 0;
	GestureInput* gest = Input::get<GestureInput>();
	resetSpellPath();
    if(active){
        // Set background color
        Application::get()->setClearColor(Color4(132,180,113,255));
        _mageTOcastle->activate(input.findKey("mageTOcastle"));
		gest->resume();

		_northWallButton->activate(input.findKey("northWallButton_mage"));
		_southeastWallButton->activate(input.findKey("southeastWallButton_mage"));
		_southwestWallButton->activate(input.findKey("southwestWallButton_mage"));
		_northeastWallButton->activate(input.findKey("northeastWallButton_mage"));
		_southWallButton->activate(input.findKey("southWallButton_mage"));
		_northwestWallButton->activate(input.findKey("northwestWallButton_mage"));
		setSide("empty");
    }
    else{
        _mageTOcastle->deactivate();
		gest->pause();

		_mageTOcastle->deactivate();
		_northWallButton->deactivate();
		_northeastWallButton->deactivate();
		_southeastWallButton->deactivate();
		_southWallButton->deactivate();
		_southwestWallButton->deactivate();
		_northwestWallButton->deactivate();
    }
}
