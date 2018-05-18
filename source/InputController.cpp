//
// Created by Yiming on 3/11/2018.
//

#include "InputController.h"

using namespace cugl;

#define SWIPE_SENSITIVITY 30
#define LISTENER_KEY 1

#define RESET_KEY KeyCode::R
#define DEBUG_KEY KeyCode::D
#define TILT_KEY KeyCode::ARROW_UP
#define KEYBOARD_MAX_TILT 1.0f
#define KEYBOARD_TILT_SENSITIVITY 0.01f

#define GESTURE_ERROR 0.2f

/**
 * Creates a new input controller.
 *
 * This constructor does NOT do any initialization.  It simply allocates the
 * object. This makes it safe to use this class without a pointer.
 */
InputController::InputController() :
		_active(false),
		_pointerVel(0, 0),
		_dTouch(0, 0),
		_justPressed(false),
		_justReleased(false),
		_isPressed(false),
		_pointerPos(0, 0),
		_vScrolling(0),
		_hScrolling(0),
		_oilTilt(0.0),
		_currMaxKey(2),
		_castFreeze(false),
		_castBomb(false),
		_castBarrier(false){
}

/**
 * Install listeners and enable control schemes.
 * Either enable Mouse+Keyboard or Touch+Accelerometer
 * @return true if properly initialized
 */
bool InputController::init(){
    bool success = true;
#ifdef CU_TOUCH_SCREEN
	// ACCELEROMETER CONTROLS
	success = Input::activate<Accelerometer>();
	if (!success) {
		throw "Accelerometer not detected :(";
	}

	// TOUCH CONTROLS
    Touchscreen* touch = Input::get<Touchscreen>();

    touch->addBeginListener(LISTENER_KEY,[=](const TouchEvent& event, bool focus) {
        this->touchBeginCB(event, focus);
    });
    touch->addMotionListener(LISTENER_KEY,[=](const TouchEvent& event, const Vec2& prev, bool focus) {
        this->touchDragCB(event,prev,focus);
    });
    touch->addEndListener(LISTENER_KEY,[=](const TouchEvent& event, bool focus) {
        this->touchReleaseCB(event, focus);
    });
#else
	//KEYBOARD CONTROLS
	success = Input::activate<Keyboard>();
	if (!success) {
		throw "Keyboard not detected :(";
	}

    //MOUSE CONTROLS
	Mouse* mouse = Input::get<Mouse>();

	mouse->setPointerAwareness(Mouse::PointerAwareness::DRAG);
	mouse->addPressListener(LISTENER_KEY, [=](const MouseEvent& event, Uint8 clicks, bool focus) {
		this->mouseDownCB(event, clicks, focus);
	});
	mouse->addReleaseListener(LISTENER_KEY, [=](const MouseEvent& event, Uint8 clicks, bool focus) {
		this->mouseUpCB(event, clicks, focus);
	});
	mouse->addDragListener(LISTENER_KEY, [=](const MouseEvent& event, const Vec2& previous, bool focus) {
		this->mouseDragCB(event, previous, focus);
	});
#endif
    _active = success;

	//GESTURE CONTROLS
	GestureInput* gest = Input::get<GestureInput>();
	gest->setTolerance(GESTURE_ERROR);
	gest->addMatchListener(LISTENER_KEY, [=](const GestureEvent& event, bool focus) {
		this->gestureMatchCB(event, focus);
	});
	//gest->addStateListener(LISTENER_KEY, [=](GestureState old, GestureState curr) {
	//	this->gestureStateCB(old, curr);
	//});
	gest->loadAsset("Gestures");
	gest->pause();
	_makeGestureTimer = 100;
	
	// Allocate the Action Manager
	_actions = ActionManager::alloc();

    return success;
}

void InputController::pollInputs() {
#ifdef CU_TOUCH_SCREEN
	Accelerometer* acc = Input::get<Accelerometer>();
    if(acc == nullptr){
        CULog("no accel");
    }
    else {
        _oilTilt = std::fmax(-acc->getAccelerationY(), 0);
    }
#else
	Keyboard* keys = Input::get<Keyboard>();
	if (keys->keyDown(TILT_KEY)) {
		_oilTilt = std::fmin(_oilTilt+KEYBOARD_TILT_SENSITIVITY,KEYBOARD_MAX_TILT);
	}
	else {
		_oilTilt = std::fmax(_oilTilt - KEYBOARD_TILT_SENSITIVITY, 0);
	}
#endif
	GestureInput* gest = Input::get<GestureInput>();
	if (gest->ready()) {
		if (_makeGestureTimer == 100) {
			CULog("Gestures Ready!");
			std::vector<std::string> gests = gest->getGestures();
		}
		if (_makeGestureTimer > 0) {
			_makeGestureTimer--;
		}
		else if (_makeGestureTimer == 0) {
			/*GestureInput* g = Input::get<GestureInput>();
			CULog("NOW RECORDING GESTURE FOR BARRIER1 SPELL");
			g->record("barrier2");*/
			_makeGestureTimer--;
		}
	}
}

void InputController::update(float deltaTime) {
    _pointerVel.set(0,0);
    _justPressed = false;
    _justReleased = false;
    _vScrolling = 0;
	_hScrolling = 0;
	_castFreeze = false;
	_castBomb = false;
	_castBarrier = false;
	_actions->update(deltaTime);
}

/**
 * Uninstalls listeners from primary input; deactivates aux input.
 */
void InputController::dispose(){
    if (_active) {
#ifdef CU_TOUCH_SCREEN
		//TOUCH+ACCELEROMETER INPUT
		//Input::deactivate<Accelerometer>();
        Touchscreen* touch = Input::get<Touchscreen>();
        touch->removeBeginListener(LISTENER_KEY);
        touch->removeEndListener(LISTENER_KEY);
        touch->removeMotionListener(LISTENER_KEY);
#else
        //MOUSE+KEYBOARD INPUT
		//Input::deactivate<Keyboard>();
        Mouse* mouse = Input::get<Mouse>();
		mouse->removePressListener(LISTENER_KEY);
		mouse->removeReleaseListener(LISTENER_KEY);
		mouse->removeDragListener(LISTENER_KEY);
#endif
		GestureInput* gest = Input::get<GestureInput>();
		gest->removeMatchListener(LISTENER_KEY);
		gest->removeStateListener(LISTENER_KEY);
        _active = false;
    }
	_actions = nullptr;
}

void InputController::clear() {
    _pointerVel.set(0,0);
    _pointerPos.set(0,0);
	_dTouch.set(0, 0);
    _justPressed = false;
    _justReleased = false;
    _isPressed = false;
    _vScrolling = 0;
	_hScrolling = 0;
	_oilTilt = 0.0;
	_currMaxKey = 0;
}

void InputController::touchBeginCB(const TouchEvent &event, bool focus) {
    _justPressed = true;
	_dTouch.set(event.position);
    _pointerPos.set(event.position);
    _isPressed = true;
}

void InputController::touchDragCB(const TouchEvent &event, const Vec2 &previous,
                                  bool focus) {
    _pointerVel += event.position - previous;
    _pointerPos.set(event.position);
    if(abs(_pointerVel.y) >= SWIPE_SENSITIVITY){
        _vScrolling = _pointerVel.y;
    }
	if (abs(_pointerVel.x) >= SWIPE_SENSITIVITY) {
		_hScrolling = _pointerVel.x;
	}
}

void InputController::touchReleaseCB(const TouchEvent &event, bool focus) {
    _justReleased = true;
    _pointerPos.set(event.position);
	_dTouch.set(0, 0);
    _isPressed = false;
}

void InputController::mouseDownCB(const MouseEvent& event, Uint8 clicks, bool focus){
    _justPressed = true;
    _pointerPos.set(event.position);
	_dTouch.set(event.position);
    _isPressed = true;
}

void InputController::mouseDragCB(const MouseEvent& event, const Vec2& previous, bool focus){
    _pointerVel += event.position - previous;
    _pointerPos.set(event.position);
    if(abs(_pointerVel.y) >= SWIPE_SENSITIVITY){
        _vScrolling = _pointerVel.y;
    }
	if (abs(_pointerVel.x) >= SWIPE_SENSITIVITY) {
		_hScrolling = _pointerVel.x;
	}
}

void InputController::mouseUpCB(const MouseEvent& event, Uint8 clicks, bool focus){
    _justReleased = true;
    _pointerPos.set(event.position);
	_dTouch.set(0, 0);
    _isPressed = false;
}

void InputController::gestureMatchCB(const GestureEvent &event, bool focus) {
	//CULog("recognized a %s spell with %f error.", event.key.c_str(), event.error);
	if (event.key == "wind2") {
		CULog("recognized FREEZE spell");
		_castFreeze = true;
	}
	else if (event.key == "barrier1" || event.key == "barrier2") {
		CULog("recognized BARRIER spell");
		_castBarrier = true;
	}
	else if (event.key == "bomb1" || event.key == "bomb2") {
		CULog("recognized BOMB spell");
		_castBomb = true;
	}
}

void InputController::gestureStateCB(GestureState old, GestureState curr) {
	if (old == GestureState::RECORDING || old == GestureState::MATCHING) {
		GestureInput* gest = Input::get<GestureInput>();
		if (gest->ready()) {
			gest->save("Gestures");
			CULog("recording session successful for BARRIER1 spell");
		}
	}
}

Uint32 InputController::generateKey(const std::string & name) {
    std::unordered_map<std::string, Uint32>::iterator i = _buttonMap.find(name);
    //New button key
    if (i == _buttonMap.end()) {
        _buttonMap[name] = _currMaxKey++;
        return _currMaxKey - 1;
    }
    //Old button key
    return i->second;
}

Uint32 InputController::findKey(const std::string & name) {
	std::unordered_map<std::string, Uint32>::iterator i = _buttonMap.find(name);
    //New button key
    if (i == _buttonMap.end()) {
        _buttonMap[name] = _currMaxKey++;
        return _currMaxKey - 1;
    }
    //Old button key
	return i->second;
}