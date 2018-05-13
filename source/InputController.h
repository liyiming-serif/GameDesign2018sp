//
// InputController.h
// Chaos Castle
//
// The Input Controller effectively abstracts touch and mouse callbacks into
// specific input commands (e.g. just released, is swiping) that can be polled
// by the parent scene class per update. Vector positions are return in SCREEN
// COORDINATES!
//
// Unlike CUGL input devices, this class is not a singleton.  It must be
// allocated before use.  However, you will notice that we do not do any
// input initialization in the constructor.  This allows us to allocate this
// controller as a field without using pointers. We simply add the class to the
// header file of its owner, and delay initialization (via the method init())
// until later. This is one of the main reasons we like to avoid initialization
// in the constructor.
//
// Created by Yiming on 3/10/2018.
//

#ifndef BUILD_ANDROID_INPUTCONTROLLER_H
#define BUILD_ANDROID_INPUTCONTROLLER_H

#include <cugl/cugl.h>
#include <unordered_map>

class InputController {
private:
    /** Whether or not this input is active */
    bool _active;

	// Singleton action manager
	std::shared_ptr<cugl::ActionManager> _actions;

    //TOUCH SUPPORT
    /**Pointer velocity, used for swiping*/
    cugl::Vec2 _pointerVel;

protected:
    // READ-ONLY INPUT RESULTS
    bool _justPressed;
    bool _justReleased;
    bool _isPressed;
    cugl::Vec2 _pointerPos;
	cugl::Vec2 _dTouch;
    int _vScrolling;
	int _hScrolling;
	float _oilTilt;
    /** Wizard spell gestures go here*/
	int _makeGestureTimer;
	bool _castFreeze;
	bool _castBomb;
	bool _castBarrier;
	/** Debug and reset toggle. */
	bool _debugReleased;
	bool _resetReleased;



	// READ-ONLY BUTTON KEYGEN
	Uint32 _currMaxKey;
	/** Data structure for remembering all the button keys*/
	std::unordered_map<std::string, Uint32> _buttonMap;



	//TOUCH CALLBACKS
	void touchBeginCB(const cugl::TouchEvent& event, bool focus);

	void touchDragCB(const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus);

	void touchReleaseCB(const cugl::TouchEvent& event, bool focus);



	//MOUSE CALLBACKS
	void mouseDownCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus);

	void mouseDragCB(const cugl::MouseEvent& event, const cugl::Vec2& previous, bool focus);

	void mouseUpCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus);


	//GESTURE CALLBACKS
	void gestureMatchCB(const cugl::GestureEvent& event, bool focus);

	void gestureStateCB(cugl::GestureState old, cugl::GestureState curr);

public:
    //CONSTRUCTORS
    InputController(); // Don't initialize = stack based

    bool init();



    //DESTRUCTORS
    ~InputController() { dispose(); }

    /**
     * Deactivates this input controller, releasing all listeners.
     * This method will not dispose of the input controller. It can be reused
     * once it is reinitialized.
     */
    void dispose();



    //GAMEPLAY
    bool isActive( ) const { return _active; }

	/**
	 * Used to read non-callback inputs like keyboard and accelerometer.
	 * Call this in your scene update FIRST.
	 */
	void pollInputs();

    /**
     * Resets single frame input results (e.g. justPressed, _pointerVel).
     * Call this update in your scene LAST.
	 *
	 * Updates the action manager.
     *
     * IMPORTANT: callbacks could get called mulitple times before update;
     * be sure to account for those cases.
     */
    void update(float deltaTime);

    /**
     * Clears any buffered inputs so that we may start fresh.
     */
    void clear();



	//BUTTON KEYGEN - API for generating and keeping track of input keys
	Uint32 generateKey(const std::string & name);

	Uint32 findKey(const std::string & name);



    //INPUT RESULT GETTERS - This is what your scenes check for every update
    //Check these BEFORE updating InputController.
    bool justPressed() {return _justPressed;}

    bool justReleased() {return _justReleased;}

    bool isPressed() {return _isPressed;}

    cugl::Vec2 pointerPos() {return _pointerPos;}

	cugl::Vec2 dTouch() { return _dTouch; }

    int vScrolling() {return _vScrolling;}

	int hScrolling() { return _hScrolling; }

	float oilTilt() { return _oilTilt; }

	bool castFreeze() { return _castFreeze; }

	bool castBomb() { return _castBomb; }

	bool castBarrier() { return _castBarrier; }

	//Action Manager Getter
	std::shared_ptr<cugl::ActionManager> actions() { return _actions; }
};

extern InputController input;

#endif //BUILD_ANDROID_INPUTCONTROLLER_H
