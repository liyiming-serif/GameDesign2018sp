//
// Created by Yiming on 2/25/2018.
//
#include <stdlib.h>
#include "BallistaScene.h"

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

#define DMG_DURATION 1.0f
#define DMG_ACT_KEY "marker"
#define SPELL_DURATION 0.7f
#define SPELL_ACT_KEY "spell_b"

// Decide when to use heavy damage indicator
#define HVY_DMG 6

#define JUNGLE  5
#define SNOW  8

#define DRAW_SCALE 32
#define FONT    _assets->get<Font>("futura_levels")

#define BUTTON_SCALE .6f

#define BALLISTA_MIN_POWER 9.0f

#define BALLISTA_MAX_RANGE 680	//farthest enemy ballista scene can see
#define BALLISTA_MIN_RANGE 128	//closest enemy ballista scene can see
#define BALLISTA_END_ZONE 0	//enemies dissapear past this y-coord; set by castle wall art assets

using namespace cugl;

// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

bool BallistaScene::inRange(float y) {
	return BALLISTA_MIN_RANGE < y && y < BALLISTA_MAX_RANGE;
}

float BallistaScene::calcY(float y) {
	int range = BALLISTA_MAX_RANGE - BALLISTA_MIN_RANGE;
	int screen_range = _size.height - BALLISTA_END_ZONE;
	return (y-BALLISTA_MIN_RANGE)/range*(screen_range)+BALLISTA_END_ZONE;
}

bool BallistaScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Set display size
    _size = Application::get()->getDisplaySize();
    _size *= GAME_WIDTH/_size.width;

    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(_size)) {
        return false;
    }

    _direction = -1;

    // Set background color
    Application::get()->setClearColor(Color4(132,180,113,255));

	//Autosort by ZOrder
	setZAutoSort(true);

    // Set the assets
    switchscene = 0;
    _assets = assets;

	// Allocate spell filter
	std::shared_ptr<Texture> filter = _assets->get<Texture>("spell_filter");
	_spellFilter = PolygonNode::allocWithTexture(filter);
	_spellFilter->setScale(_size / _spellFilter->getSize());
	_spellFilter->setAnchor(Vec2::ANCHOR_CENTER);
	_spellFilter->setPosition(_size / 2.0);
	_spellFilter->setColor(Color4::CLEAR);
	_spellFilter->setZOrder(3);
	addChild(_spellFilter);

	// Allocate spell animations
	std::shared_ptr<Texture> sp = _assets->get<Texture>("bomb_spell");
	_bombAnim = AnimationNode::alloc(sp,2,4);
	_bombAnim->setAnchor(Vec2::ANCHOR_CENTER);
	_bombAnim->setPosition(_size / 2.0);
	_bombAnim->setColor(Color4::CLEAR);
	_bombAnim->setZOrder(4);
	addChild(_bombAnim);

	sp = _assets->get<Texture>("freeze_spell");
	_freezeAnim = AnimationNode::alloc(sp, 3, 6);
	_freezeAnim->setAnchor(Vec2::ANCHOR_CENTER);
	_freezeAnim->setPosition(_size / 2.0);
	_freezeAnim->setColor(Color4::CLEAR);
	_freezeAnim->setZOrder(4);
	addChild(_freezeAnim);

	_spellAnim = Animate::alloc(0,7,SPELL_DURATION);
	_spellAnim2 = Animate::alloc(0, 17, SPELL_DURATION);

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

    // Set the background image
    std::shared_ptr<Texture> texture  = _assets->get<Texture>("weaponBG_jungle");
    std::shared_ptr<Texture> texture_s  = _assets->get<Texture>("weaponBG_snow");
    std::shared_ptr<Texture> texture_d  = _assets->get<Texture>("weaponBG_desert");
    if (gameModel.level<JUNGLE) {
        _background = PolygonNode::allocWithTexture(texture);
    }
    else if (gameModel.level<SNOW) {
        _background = PolygonNode::allocWithTexture(texture_s);
    }
    else {
        _background = PolygonNode::allocWithTexture(texture_d);
    }
    _background->setScale(.54f); // Magic number to rescale asset
    _background->setAnchor(Vec2::ANCHOR_CENTER);
    _background->setPosition(_size.width/2,_size.height/2);
	_background->setZOrder(-2);
    
    std::shared_ptr<Texture> turret  = _assets->get<Texture>("ballista_turret_GREEN");
    _ballistaTurret_GREEN = PolygonNode::allocWithTexture(turret);
    _ballistaTurret_GREEN->setScale(.54f); // Magic number to rescale asset
    _ballistaTurret_GREEN->setAnchor(Vec2::ANCHOR_CENTER);
    _ballistaTurret_GREEN->setPosition(_size.width/2,_size.height/2);
    
    std::shared_ptr<Texture> turretYELLOW  = _assets->get<Texture>("ballista_turret_YELLOW");
    _ballistaTurret_YELLOW = PolygonNode::allocWithTexture(turretYELLOW);
    _ballistaTurret_YELLOW->setScale(.54f); // Magic number to rescale asset
    _ballistaTurret_YELLOW->setAnchor(Vec2::ANCHOR_CENTER);
    _ballistaTurret_YELLOW->setPosition(_size.width/2,_size.height/2);
    
    std::shared_ptr<Texture> turretORANGE  = _assets->get<Texture>("ballista_turret_ORANGE");
    _ballistaTurret_ORANGE = PolygonNode::allocWithTexture(turretORANGE);
    _ballistaTurret_ORANGE->setScale(.54f); // Magic number to rescale asset
    _ballistaTurret_ORANGE->setAnchor(Vec2::ANCHOR_CENTER);
    _ballistaTurret_ORANGE->setPosition(_size.width/2,_size.height/2);
    
    std::shared_ptr<Texture> turretRED  = _assets->get<Texture>("ballista_turret_RED");
    _ballistaTurret_RED = PolygonNode::allocWithTexture(turretRED);
    _ballistaTurret_RED->setScale(.54f); // Magic number to rescale asset
    _ballistaTurret_RED->setAnchor(Vec2::ANCHOR_CENTER);
    _ballistaTurret_RED->setPosition(_size.width/2,_size.height/2);

    // Get the ballista image and attach it to the animated model
	_ballista = BallistaModel::alloc(Vec2(_size.width / 2, _background->getContentHeight() / 10),_assets);

	// initialize, but don't add input dragging UI assets
	_dpOrange = _assets->get<Texture>("drag-point-short");
	_dlOrange = _assets->get<Texture>("drag-line-short");
	_dpBlue = _assets->get<Texture>("drag-point");
	_dlBlue = _assets->get<Texture>("drag-line");

	_dragStart = AnimationNode::alloc(_dpOrange, 1, 4);
	_dragStart->setFrame(0);
	_dragStart->setAnchor(Vec2::ANCHOR_CENTER);
	_dragStart->setName("dragStart");
	_dragLine = PolygonNode::allocWithTexture(_dlOrange);
	_dragLine->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	_dragLine->setName("dragLine");
	_dragEnd = AnimationNode::alloc(_dpOrange, 1, 4);
	_dragEnd->setFrame(0);
	_dragEnd->setAnchor(Vec2::ANCHOR_CENTER);
	_dragEnd->setName("dragEnd");

    // Create the back button.  A button has an up image and a down image
    std::shared_ptr<Texture> castle   = _assets->get<Texture>("castle");
    _ballistaTOcastle = Button::alloc(PolygonNode::allocWithTexture(castle));
    
    // Create a callback function for the OVERWORLD button
    _ballistaTOcastle->setName("ballistaTOcastle");
    _ballistaTOcastle->setListener([=] (const std::string& name, bool down) {
        if (!down) {
            _exitCount+=1;
            switchscene = OVERWORLD;
        }
    });

    // Position the OVERWORLD button in the bottom left
    _ballistaTOcastle->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _ballistaTOcastle->setPosition(15,_size.height-18);
    _ballistaTOcastle->setScale(.6f);
    _ballistaTOcastle->setZOrder(-1);
    

    // Add children to the scene graph
    addChild(_background);
    addChild(_ballistaTurret_GREEN);
    addChild(_ballistaTurret_YELLOW);
    addChild(_ballistaTurret_ORANGE);
    addChild(_ballistaTurret_RED);
	_ballistaTurret_GREEN->setVisible(false);
	_ballistaTurret_YELLOW->setVisible(false);
	_ballistaTurret_ORANGE->setVisible(false);
	_ballistaTurret_RED->setVisible(false);
	if (_ballista != nullptr) {
		addChild(_ballista->getNode());
	}
    addChild(_ballistaTOcastle);

    // Create the arrows set
    _arrows.clear();
	_arrowsToFree.clear();

	// Create the enemy set
	_enemyArray.clear();
	_enemiesToFree.clear();

    // Create the physics world
    _world = ObstacleWorld::alloc(Rect(Vec2::ZERO, _size/DRAW_SCALE),Vec2::ZERO);
	activateWorldCollisions();

    // We can only activate a button AFTER it is added to a scene
     _ballistaTOcastle->activate(input.generateKey("ballistaTOcastle"));
    
    
    std::shared_ptr<Texture> north_compass  = _assets->get<Texture>("bigC");
    N_compass = PolygonNode::allocWithTexture(north_compass);
    N_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    N_compass->setAnchor(Vec2::ANCHOR_CENTER);
    N_compass->setPosition(950,72.2);
    addChild(N_compass);
    N_compass->setVisible(false);
    
    std::shared_ptr<Texture> northeast_compass  = _assets->get<Texture>("smallC");
    NE_compass = PolygonNode::allocWithTexture(northeast_compass);
    NE_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    NE_compass->setAnchor(Vec2::ANCHOR_CENTER);
    NE_compass->setPosition(950,72.2);
    addChild(NE_compass);
    NE_compass->setVisible(false);
    
    std::shared_ptr<Texture> northwest_compass  = _assets->get<Texture>("smallC");
    NW_compass = PolygonNode::allocWithTexture(northwest_compass);
    NW_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    NW_compass->setAnchor(Vec2::ANCHOR_CENTER);
    NW_compass->setPosition(950,72.2);
    NW_compass->setAngle(M_PI/2);
    addChild(NW_compass);
    NW_compass->setVisible(false);
    
    std::shared_ptr<Texture> south_compass  = _assets->get<Texture>("bigC");
    S_compass = PolygonNode::allocWithTexture(south_compass);
    S_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    S_compass->setAnchor(Vec2::ANCHOR_CENTER);
    S_compass->setPosition(950,72.2);
    S_compass->setAngle(M_PI);
    addChild(S_compass);
    S_compass->setVisible(false);
    
    std::shared_ptr<Texture> southeast_compass  = _assets->get<Texture>("smallC");
    SE_compass = PolygonNode::allocWithTexture(southeast_compass);
    SE_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    SE_compass->setAnchor(Vec2::ANCHOR_CENTER);
    SE_compass->setPosition(950,72.2);
    SE_compass->setAngle(-M_PI/2);
    addChild(SE_compass);
    SE_compass->setVisible(false);
    
    std::shared_ptr<Texture> southwest_compass  = _assets->get<Texture>("smallC");
    SW_compass = PolygonNode::allocWithTexture(southwest_compass);
    SW_compass->setScale(BUTTON_SCALE); // Magic number to rescale asset
    SW_compass->setAnchor(Vec2::ANCHOR_CENTER);
    SW_compass->setPosition(950,72.2);
    SW_compass->setAngle(M_PI);
    addChild(SW_compass);
    SW_compass->setVisible(false);
    


    _ammoText =Label::alloc((std::string) "                                              ", FONT);
    addChild(_ammoText);
    _ammoText->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _ammoText->setPosition(10, 30);
    _ammoText->setForeground(cugl::Color4(255,255,255,255));
    _ammoText->setScale(.5f);
    
    _lowAmmoText =Label::alloc((std::string) "LOW AMMO", FONT);
    _lowAmmoText->setAnchor(Vec2::ANCHOR_CENTER);
    _lowAmmoText->setPosition(_size.width*.5f, _size.height*.7f);
    _lowAmmoText->setForeground(cugl::Color4(209,123,104,255));
    addChild(_lowAmmoText);
    _lowAmmoText->setVisible(false);
    _lowAmmoText->setZOrder(-1);
    
    _noAmmoText =Label::alloc((std::string) "OUT OF AMMO", FONT);
    _noAmmoText->setAnchor(Vec2::ANCHOR_CENTER);
    _noAmmoText->setPosition(_size.width*.5f, _size.height*.7f);
    _noAmmoText->setForeground(cugl::Color4(178,101,99,255));
    addChild(_noAmmoText);
    _noAmmoText->setVisible(false);
    _noAmmoText->setZOrder(-1);
    
    
    std::shared_ptr<Texture> tut_swipe  = _assets->get<Texture>("tutorial_ballista");
    _ballista_swipe = PolygonNode::allocWithTexture(tut_swipe);
    _ballista_swipe->setScale(1.0); // Magic number to rescale asset
    _ballista_swipe->setAnchor(Vec2::ANCHOR_TOP_CENTER);
    _ballista_swipe->setPosition(_size.width*.5f,_size.height*.9f);
    _ballista_swipe->setAngle(M_PI_4);
    addChild(_ballista_swipe);
    
    
    std::shared_ptr<Texture> tut_tap  = _assets->get<Texture>("tutorial_tap");
    _ballista_tap = PolygonNode::allocWithTexture(tut_tap);
    _ballista_tap->setScale(.5); // Magic number to rescale asset
    _ballista_tap->setAnchor(Vec2::ANCHOR_TOP_CENTER);
    _ballista_tap->setPosition(_size.width*.07f,_size.height*.85f);
    addChild(_ballista_tap);
    
    

    
	// Add damage indicators overlay
	for (int i = 0; i < _dmgIndicators.size(); i++) {
		addChild(_dmgIndicators.at(i));
	}

    return true;
}

void BallistaScene::dispose() {
    if (_active) {
        if (_world != nullptr) {
            _world->clear();
            _world = nullptr;
        }
        removeAllChildren();
		_dragStart = nullptr;
		_dragLine = nullptr;
		_dragEnd = nullptr;
		_dpOrange = nullptr;
		_dlOrange = nullptr;
		_dpBlue = nullptr;
		_dlBlue = nullptr;
		_ballista = nullptr;
		_background = nullptr;
		_ballistaTurret_GREEN = nullptr;
		_ballistaTurret_YELLOW = nullptr;
		_ballistaTurret_ORANGE = nullptr;
		_ballistaTurret_RED = nullptr;
		_ballistaTOcastle = nullptr;
		N_compass = nullptr;
		NE_compass = nullptr;
		NW_compass = nullptr;
		S_compass = nullptr;
		SE_compass = nullptr;
		SW_compass = nullptr;
		_ammoText = nullptr;
        _arrows.clear();
		_arrowsToFree.clear();
        _active = false;
        _enemyArray.clear();
		_enemiesToFree.clear();
		_dmgFadeOUT = nullptr;
		_dmgIndicators.clear();

		//spells
		_spellAnim = nullptr;
		_spellAnim2 = nullptr;
		_spellFilter = nullptr;
		_bombAnim = nullptr;
		_freezeAnim = nullptr;

        _ballista_tap=nullptr;
        _ballista_swipe=nullptr;

    }
}

void BallistaScene::setCompass(int direction){
    N_compass->setVisible(false);
    NE_compass->setVisible(false);
    NW_compass->setVisible(false);
    S_compass->setVisible(false);
    SE_compass->setVisible(false);
    SW_compass->setVisible(false);
    if (direction == 0) {
        N_compass->setVisible(true);
    }
    else if (direction == 1){
        NW_compass->setVisible(true);
    }
    else if (direction == 2){
        SW_compass->setVisible(true);
    }
    else if (direction == 3){
        S_compass->setVisible(true);
    }
    else if (direction == 4){
        SE_compass->setVisible(true);
    }
    else if (direction == 5){
        NE_compass->setVisible(true);
    }
}

void BallistaScene::setWall(int direction){
    _ballistaTurret_GREEN->setVisible(false);
    _ballistaTurret_YELLOW->setVisible(false);
    _ballistaTurret_ORANGE->setVisible(false);
    _ballistaTurret_RED->setVisible(false);
    if (gameModel.getWallHealth(direction) < 25){
        _ballistaTurret_RED->setVisible(true);
    }
    else if (gameModel.getWallHealth(direction) < 50){
        _ballistaTurret_ORANGE->setVisible(true);
    }
    else if (gameModel.getWallHealth(direction) < 75){
        _ballistaTurret_YELLOW->setVisible(true);
    }
    else {
        _ballistaTurret_GREEN->setVisible(true);
    }
}

void BallistaScene::update(float deltaTime, int direction){

    if (gameModel.getWallHealth(0) == 0 || gameModel.getWallHealth(1) == 0 || gameModel.getWallHealth(2) == 0 ||
        gameModel.getWallHealth(3) == 0 || gameModel.getWallHealth(4) == 0 || gameModel.getWallHealth(5) == 0) {
        switchscene = LOSE;
    }
    if (gameModel._currentTime > gameModel._endTime){
        if (gameModel._enemyArrayMaster[0].size()== 0 && gameModel._enemyArrayMaster[1].size()== 0 && gameModel._enemyArrayMaster[2].size()== 0 && gameModel._enemyArrayMaster[3].size()== 0 && gameModel._enemyArrayMaster[4].size()== 0 && gameModel._enemyArrayMaster[5].size()== 0) {
            switchscene = WIN;
        }
    }
    _direction = direction;
	_ammoText->setText("Ammo "+ std::to_string(gameModel.getArrowAmmo(0)));
    setWall(direction);
    
    if (_shots > 2) {
        _ballista_swipe->setVisible(false);
    }
    if (_exitCount > 0) {
        _ballista_tap->setVisible(false);
    }

	//remove spell effects if they're done
	if (!input.actions()->isActive(SPELL_ACT_KEY)) {
		_bombAnim->setColor(Color4::CLEAR);
		_freezeAnim->setColor(Color4::CLEAR);
		_spellFilter->setColor(Color4::CLEAR);
	}

    if (gameModel.getArrowAmmo(0) < 11 && gameModel.getArrowAmmo(0) > 0) {
        _lowAmmoText->setVisible(true);
    }
    else {
        _lowAmmoText->setVisible(false);
    }
    
    if (gameModel.getArrowAmmo(0)== 0) {
        _noAmmoText->setVisible(true);
    }
    else {
        _noAmmoText->setVisible(false);
    }
    
	bool hasAmmo = gameModel.getArrowAmmo(0) > 0;

	//poll damage indicators
	pollDmgIndicators();

	// Poll inputs
	if (input.justPressed()) {
		if (getChildByName("dragStart") == nullptr) {
			addChild(_dragStart);
		}
		if (getChildByName("dragLine") == nullptr) {
			addChild(_dragLine);
		}
		if (getChildByName("dragEnd") == nullptr) {
			addChild(_dragEnd);
		}

		_dragStart->setPosition(screenToWorldCoords(input.dTouch()));
		_dragLine->setPosition(screenToWorldCoords(input.dTouch()));
	}
    if(input.isPressed()){
		Vec2 pointdir = screenToWorldCoords(input.dTouch()) - screenToWorldCoords(input.pointerPos());
		if (_ballista->isReadyToFire) {
			_ballista->setAngle(pointdir.getAngle());
			_ballista->setPower(2.0f*pointdir.length() / (float)DRAW_SCALE, false);
		}

		//drag UI
		_dragStart->setFrame((_dragStart->getFrame() + 1) % _dragStart->getSize());
		_dragEnd->setFrame((_dragStart->getFrame() + 1) % _dragStart->getSize());

		_dragEnd->setPosition(screenToWorldCoords(input.pointerPos()));
		_dragLine->setAngle(pointdir.getAngle() + M_PI);
		_dragLine->setScale(pointdir.length() / 36.0f, 1.0f);
		if (_ballista->getPower() < BALLISTA_MIN_POWER) { //not enough power to fire ballista
			_dragStart->setTexture(_dpOrange);
			_dragLine->setTexture(_dlOrange);
			_dragEnd->setTexture(_dpOrange);
		}
		else {
			_dragStart->setTexture(_dpBlue);
			_dragLine->setTexture(_dlBlue);
			_dragEnd->setTexture(_dpBlue);
		}
    }
    if(input.justReleased()){
		if (getChildByName("dragStart") != nullptr) {
			removeChild(_dragStart);
		}
		if (getChildByName("dragLine") != nullptr) {
			removeChild(_dragLine);
		}
		if (getChildByName("dragEnd") != nullptr) {
			removeChild(_dragEnd);
		}
		
		if (_ballista->isReadyToFire) {
            if (gameModel.getArrowAmmo(0)>0 && _ballista->getPower()>=BALLISTA_MIN_POWER) {
                // Allocate a new arrow in memory
                std::shared_ptr<ArrowModel> a = ArrowModel::alloc(_ballista->getPosition(),
					_ballista->getPower(), _ballista->getAngle(), DRAW_SCALE, _assets);

                if (a != nullptr) {
                    _arrows.insert(a);
                    _world->addObstacle(a);
                    addChild(a->getNode());
                    gameModel.setArrowAmmo(0,gameModel.getArrowAmmo(0)-1);
					if (gameModel.isNetworked() && !gameModel.isServer()) {
						gameModel.addDeltaAmmo(0,-1);
					}
                }
            }
			// Fire ballista
            _shots+=1;
			_ballista->setPower(0.0f, true);
			_ballista->isReadyToFire = false;
		}
    }

	
	_ballista->update(deltaTime, hasAmmo);

    // Update arrows and mark out of bound ones for deletion
    Rect bounds(Vec2::ZERO, _size/DRAW_SCALE);
    for(auto it = _arrows.begin(); it != _arrows.end(); it++){
        std::shared_ptr<ArrowModel> a = *it;
        if(a != nullptr) {
            a->update(deltaTime);
        }
        if(!bounds.contains(a->getPosition())){
			_arrowsToFree.insert(a);
        }
    }

	// Delete the arrows here because you can't remove elements while iterating
	for (auto it = _arrowsToFree.begin(); it != _arrowsToFree.end(); it++) {
		std::shared_ptr<ArrowModel> a = *it;
		_world->removeObstacle(a.get());
		removeChild(a->getNode());
		_arrows.erase(a);
	}
	_arrowsToFree.clear();

	//reflect enemy changes in enemy models
	updateEnemyModels(deltaTime,direction);

    //crank the physics engine
    _world->update(deltaTime);

}

void BallistaScene::pollDmgIndicators() {
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

void BallistaScene::updateEnemyModels(float deltaTime, int direction) {
	//add or update enemy models from master array
	for (std::pair<std::string, std::shared_ptr<EnemyDataModel>> epair : gameModel._enemyArrayMaster[direction]) {
		std::string k = epair.first;
		std::shared_ptr<EnemyDataModel> e = epair.second;

		std::unordered_map<std::string, std::shared_ptr<EnemyModel>>::iterator i =
			_enemyArray.find(k);
		if (i == _enemyArray.end()) { //new enemy appeared
			if (inRange(e->getPos().y)) {
				Vec2 pos = Vec2(e->getPos().x, calcY(e->getPos().y));
				std::shared_ptr<EnemyModel> en = EnemyModel::alloc(k, pos, e->getType(), DRAW_SCALE, _assets);
				if (en != nullptr) {
					_enemyArray[k] = en;
					_world->addObstacle(en);
					addChild(en->getNode());
				}
			}
		}
		else { //update existing enemy model
			if (e->getFreezeStep() > 0) {
				i->second->getNode()->setColor(Color4::BLUE);
			}
			else {
				i->second->getNode()->setColor(Color4::WHITE);
			}
			Vec2 pos = Vec2(e->getPos().x, calcY(e->getPos().y));
			i->second->setPosition(pos/DRAW_SCALE);
			float atkProgress = (float)e->getAtkCounter() / (float)e->getAtkSpeed();
			i->second->setAtkProgress(atkProgress);
			i->second->update(deltaTime);
		}
	}

	//delete enemy models too close or not found in the master that have finished dying
	for (std::pair<std::string, std::shared_ptr<EnemyModel>> e : _enemyArray) {
		std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>::iterator i =
			gameModel._enemyArrayMaster[direction].find(e.first);
		if (i == gameModel._enemyArrayMaster[direction].end()) {
			if (!e.second->isDying) {
				//remove hit box for enemies playing death animation
				_world->removeObstacle(e.second.get());
				e.second->isDying = true;
			}
			else {
				//move death animation forward
				e.second->update(deltaTime);
			}
			if(e.second->doneDying) {
				//mark enemies with finished death animation for removal
				_enemiesToFree.insert(e.second);
			}
		}
		else if (!inRange(i->second->getPos().y)) {
			//mark enemy models for deletion
			_enemiesToFree.insert(e.second);
			_world->removeObstacle(e.second.get());
		}
	}
	// Delete the enemies here because you can't remove elements while iterating
	for (auto it = _enemiesToFree.begin(); it != _enemiesToFree.end(); it++) {
		std::shared_ptr<EnemyModel> e = *it;
		removeChild(e->getNode());
		_enemyArray.erase(e->getName());
	}
	_enemiesToFree.clear();
}

void BallistaScene::animateSpells(const std::string & spellName) {
	//Animate spells
	if (spellName == "freeze") {
		bool succ = input.actions()->activate(SPELL_ACT_KEY, _spellAnim2, _freezeAnim);
		if (succ) {
			_freezeAnim->setColor(Color4::WHITE);
			_spellFilter->setColor(Color4::WHITE);
		}
	}
	else if (spellName == "bomb") {
		bool succ = input.actions()->activate(SPELL_ACT_KEY, _spellAnim, _bombAnim);
		if (succ) {
			_bombAnim->setColor(Color4::WHITE);
			_spellFilter->setColor(Color4::WHITE);
		}
	}
}

//Pause or Resume
void BallistaScene::setActive(bool active, int direction, std::string spellName){
    _direction = direction;
    _active = active;
    switchscene = 0;
    
    
    if (gameModel.level==1){
        _swipeTutorial=true;
    }
    else {
        _swipeTutorial=false;
    }
    if (_swipeTutorial && _shots < 2) {
        _ballista_swipe->setVisible(true);
    }
    if (_swipeTutorial && _exitCount < 1) {
        _ballista_tap->setVisible(true);
    }
    if (!_swipeTutorial) {
        _ballista_swipe->setVisible(false);
        _ballista_tap->setVisible(false);
    }

	//empty the arrow arrays to prevent data leaks
	for (auto it = _arrows.begin(); it != _arrows.end(); it++) {
		std::shared_ptr<ArrowModel> a = *it;
		_world->removeObstacle(a.get());
		removeChild(a->getNode());
		//refund their arrows
		gameModel.setArrowAmmo(0, gameModel.getArrowAmmo(0) + 1);
	}
    _arrows.clear();
	for (auto it = _arrowsToFree.begin(); it != _arrowsToFree.end(); it++) {
		std::shared_ptr<ArrowModel> a = *it;
		_world->removeObstacle(a.get());
		removeChild(a->getNode());
	}
	_arrowsToFree.clear();

	//empty the enemy arrays to prevent data leaks
	for (std::pair<std::string, std::shared_ptr<EnemyModel>> epair : _enemyArray) {
		std::shared_ptr<EnemyModel> e = epair.second;
		if (!e->isDying) {
			_world->removeObstacle(e.get());
		}
		removeChild(e->getNode());
	}
	_enemyArray.clear();

	for (auto it = _enemiesToFree.begin(); it != _enemiesToFree.end(); it++) {
		std::shared_ptr<EnemyModel> e = *it;
		removeChild(e->getNode());
		_enemyArray.erase(e->getName());
	}
	_enemiesToFree.clear();

    if(active){
        Application::get()->setClearColor(Color4(255,255,255,255));
        _ballistaTOcastle->activate(input.findKey("ballistaTOcastle"));
        BallistaScene::setCompass(direction);
        BallistaScene::setWall(direction);

		std::shared_ptr<Texture> texture = _assets->get<Texture>("weaponBG_jungle");
		std::shared_ptr<Texture> texture_s = _assets->get<Texture>("weaponBG_snow");
		std::shared_ptr<Texture> texture_d = _assets->get<Texture>("weaponBG_desert");
		if (gameModel.level<JUNGLE) {
			_background->setTexture(texture);
		}
		else if (gameModel.level<SNOW) {
			_background->setTexture(texture_s);
		}
		else {
			_background->setTexture(texture_d);
		}

		animateSpells(spellName);
    }
    else{
        _ballistaTOcastle->deactivate();

		//wipe residual action animations
		for (auto const& it : _dmgIndicators) {
			it->setColor(Color4::CLEAR);
			input.actions()->clearAllActions(it);
		}

		//wipe spell fx
		_spellFilter->setColor(Color4::CLEAR);
		_bombAnim->setColor(Color4::CLEAR);
		_freezeAnim->setColor(Color4::CLEAR);
		input.actions()->clearAllActions(_bombAnim);
		input.actions()->clearAllActions(_freezeAnim);
		
    }
}

void BallistaScene::activateWorldCollisions() {
	_world->activateCollisionCallbacks(true);
	_world->onBeginContact = [this](b2Contact* contact) {
		beginContact(contact);
	};
}

//TODO: DISABLE PHYSICS ENGINE DURING COLLISIONS
void BallistaScene::beginContact(b2Contact* contact) {
	b2Body* body1 = contact->GetFixtureA()->GetBody();
	b2Body* body2 = contact->GetFixtureB()->GetBody();

	bool contactWithArrow = false;
	for (auto it = _arrows.begin(); it != _arrows.end(); it++) {
		std::shared_ptr<ArrowModel> a = *it;
		if ((body1->GetUserData() == a.get() || body2->GetUserData() == a.get())) {
			_arrowsToFree.insert(a);
			if (!a->collided) {
				contactWithArrow = true;
				a->collided = true;
			}
			break;
		}
	}

	if (contactWithArrow) {
		for (std::pair<std::string, std::shared_ptr<EnemyModel>> epair : _enemyArray) {
			std::shared_ptr<EnemyModel> e = epair.second;
			if (body1->GetUserData() == e.get() || body2->GetUserData() == e.get()) {
				std::string k = epair.first;
				std::unordered_map<std::string, std::shared_ptr<EnemyDataModel>>::iterator i =
					gameModel._enemyArrayMaster[_direction].find(k);
				if (i != gameModel._enemyArrayMaster[_direction].end()) {
					std::shared_ptr<EnemyDataModel> ed = i->second;
					if (ed->getHealth() <= 1) {
						gameModel._enemiesToFreeMaster[_direction].push_back(k);
					}
					else {
						if (ed->getFreezeStep() > 0) {
							ed->setHealth(ed->getHealth() - 2);
						}
						else {
							ed->setHealth(ed->getHealth() - 1);
						}
					}
					if (gameModel.isNetworked() && !gameModel.isServer()) {
						if (ed->getFreezeStep() > 0) {
							gameModel.addEnemyChange(k, -2, ed->getWall());
						}
						else {
							gameModel.addEnemyChange(k, -1, ed->getWall());
						}
					}
				}
				break;
			}
		}
	}
}
