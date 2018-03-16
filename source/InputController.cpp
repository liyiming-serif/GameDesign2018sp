//
// Created by Yiming on 3/11/2018.
//

#include "InputController.h"

using namespace cugl;

#define SWIPE_SENSITIVITY 30
#define LISTENER_KEY 1

/**
 * Creates a new input controller.
 *
 * This constructor does NOT do any initialzation.  It simply allocates the
 * object. This makes it safe to use this class without a pointer.
 */
InputController::InputController() :
        _active(false),
        _pointerVel(0,0),
        _justPressed(false),
        _justReleased(false),
        _isPressed(false),
        _pointerPos(0,0),
        _vScrolling(0){
}

/**
 * Install listeners and alloc any memory.
 * @return true if properly initiallized
 */
bool InputController::init(){
    bool success = true;
#ifdef CU_TOUCH_SCREEN
    Touchscreen* touch = Input::get<Touchscreen>();

    touch->addBeginListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, bool focus) {
        this->touchBeginCB(event, focus);
    });
    touch->addMotionListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, const Vec2& prev, bool focus) {
        this->touchDragCB(event,prev,focus);
    });
    touch->addEndListener(LISTENER_KEY,[=](const cugl::TouchEvent& event, bool focus) {
        this->touchReleaseCB(event, focus);
    });
#else
    //MOUSE CONTROLS
	Mouse* mouse = Input::get<Mouse>();

	mouse->setPointerAwareness(cugl::Mouse::PointerAwareness::DRAG);
	mouse->addPressListener(LISTENER_KEY, [=](const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
		this->mouseDownCB(event, clicks, focus);
	});
	mouse->addReleaseListener(LISTENER_KEY, [=](const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
		this->mouseUpCB(event, clicks, focus);
	});
	mouse->addDragListener(LISTENER_KEY, [=](const cugl::MouseEvent& event, const cugl::Vec2& previous, bool focus) {
		this->mouseDragCB(event, previous, focus);
	});
#endif
    _active = true;
    return success;
}

void InputController::update(float deltaTime) {
    _pointerVel.set(0,0);
    _justPressed = false;
    _justReleased = false;
    _vScrolling = 0;
}

/**
 * More of a disable function. Only uninstalls listeners, but doesn't disable input.
 */
void InputController::dispose(){
    if (_active) {
#ifdef CU_TOUCH_SCREEN
        Touchscreen* touch = Input::get<Touchscreen>();
        touch->removeBeginListener(LISTENER_KEY);
        touch->removeEndListener(LISTENER_KEY);
        touch->removeMotionListener(LISTENER_KEY);
#else
        //MOUSE INPUT
        Mouse* mouse = Input::get<Mouse>();
		mouse->removePressListener(LISTENER_KEY);
		mouse->removeReleaseListener(LISTENER_KEY);
		mouse->removeDragListener(LISTENER_KEY);
#endif
        _active = false;
    }
}

void InputController::clear() {
    _pointerVel.set(0,0);
    _pointerPos.set(0,0);
    _justPressed = false;
    _justReleased = false;
    _isPressed = false;
    _vScrolling = 0;
}

void InputController::touchBeginCB(const cugl::TouchEvent &event, bool focus) {
    _justPressed = true;
    _pointerPos.set(event.position);
    _isPressed = true;
}

void InputController::touchDragCB(const cugl::TouchEvent &event, const cugl::Vec2 &previous,
                                  bool focus) {
    _pointerVel += event.position - previous;
    _pointerPos.set(event.position);
    if(abs(_pointerVel.y) >= SWIPE_SENSITIVITY){
        _vScrolling = _pointerVel.y;
    }
}

void InputController::touchReleaseCB(const cugl::TouchEvent &event, bool focus) {
    _justReleased = true;
    _pointerPos.set(event.position);
    _isPressed = false;
}

void InputController::mouseDownCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus){
    _justPressed = true;
    _pointerPos.set(event.position);
    _isPressed = true;
}

void InputController::mouseDragCB(const cugl::MouseEvent& event, const cugl::Vec2& previous, bool focus){
    _pointerVel += event.position - previous;
    _pointerPos.set(event.position);
    if(abs(_pointerVel.y) >= SWIPE_SENSITIVITY){
        _vScrolling = _pointerVel.y;
    }
}

void InputController::mouseUpCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus){
    _justReleased = true;
    _pointerPos.set(event.position);
    _isPressed = false;
}