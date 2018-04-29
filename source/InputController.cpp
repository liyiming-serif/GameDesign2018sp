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

/**
 * Creates a new input controller.
 *
 * This constructor does NOT do any initialzation.  It simply allocates the
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
		_currMaxKey(2){
}

/**
 * Install listeners and enable control schemes.
 * Either enable Mouse+Keyboard or Touch+Accelerometer
 * @return true if properly initiallized
 */
bool InputController::init(){
    bool success = true;
#ifdef CU_TOUCH_SCREEN
	// ACCELEROMETER CONTROLS
	success = Input::activate<Accelerometer>();
	/*
	if (!success) {
	CULog("accel 1");
		throw "Accelerometer not detected :(";
		CULog("accel2");
	}
	*/

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
	CULog("key1");
		throw "Keyboard not detected :(";
		CULog("key2");
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
    return success;
}

void InputController::pollInputs() {
#ifdef CU_TOUCH_SCREEN
	Accelerometer* acc = Input::get<Accelerometer>();
	_oilTilt = std::fmax(-acc->getAccelerationY(), 0);
#else
	Keyboard* keys = Input::get<Keyboard>();
	if (keys->keyDown(TILT_KEY)) {
		_oilTilt = std::fmin(_oilTilt+KEYBOARD_TILT_SENSITIVITY,KEYBOARD_MAX_TILT);
	}
	else {
		_oilTilt = std::fmax(_oilTilt - KEYBOARD_TILT_SENSITIVITY, 0);
	}
#endif
}

void InputController::update(float deltaTime) {
    _pointerVel.set(0,0);
    _justPressed = false;
    _justReleased = false;
    _vScrolling = 0;
	_hScrolling = 0;
}

/**
 * Uninstalls listeners from primary input; deactivates aux input.
 */
void InputController::dispose(){
CULog("disposing");
    if (_active) {
#ifdef CU_TOUCH_SCREEN
        CULog("deactivate");
		//TOUCH+ACCELEROMETER INPUT
		Input::deactivate<Accelerometer>();
        Touchscreen* touch = Input::get<Touchscreen>();
        touch->removeBeginListener(LISTENER_KEY);
        touch->removeEndListener(LISTENER_KEY);
        touch->removeMotionListener(LISTENER_KEY);
#else
        //MOUSE+KEYBOARD INPUT
		Input::deactivate<Keyboard>();
        Mouse* mouse = Input::get<Mouse>();
		mouse->removePressListener(LISTENER_KEY);
		mouse->removeReleaseListener(LISTENER_KEY);
		mouse->removeDragListener(LISTENER_KEY);
#endif
        _active = false;
    }
    _buttonMap.clear();
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