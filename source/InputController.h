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

class InputController {
private:
    /** Whether or not this input is active */
    bool _active;



    //TOUCH SUPPORT
    /**Pointer velocity, used for swiping*/
    cugl::Vec2 _pointerVel;

protected:
    // READ-ONLY INPUT RESULTS
    bool _justPressed;
    bool _justReleased;
    bool _isPressed;
    cugl::Vec2 _pointerPos;
    int _vScrolling;
    /** Wizard spell gestures go here*/

    /** Whether the debug toggle was chosen. */
    bool _debugPressed;

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
     * Resets single frame input results (e.g. justPressed, _pointerVel).
     * Call this update in your scene LAST.
     *
     * IMPORTANT: callbacks could get called mulitple times before update;
     * be sure to account for those cases.
     */
    void update(float deltaTime);

    /**
     * Clears any buffered inputs so that we may start fresh.
     */
    void clear();



    //INPUT RESULTS - This is what your scenes check for every update
    //Check these BEFORE updating InputController.
    bool justPressed() {return _justPressed;}

    bool justReleased() {return _justReleased;}

    bool isPressed() {return _isPressed;}

    cugl::Vec2 pointerPos() {return _pointerPos;}

    int vScrolling() {return _vScrolling;}

    /**Wizard gestures go here*/



    //TOUCH CALLBACKS
    void touchBeginCB(const cugl::TouchEvent& event, bool focus);

    void touchDragCB(const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus);

    void touchReleaseCB(const cugl::TouchEvent& event, bool focus);


    //MOUSE CALLBACKS
    void mouseDownCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus);

    void mouseUpCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus);

    void mouseMovedCB(const cugl::MouseEvent& event, const cugl::Vec2& previous, bool focus);

};

#endif //BUILD_ANDROID_INPUTCONTROLLER_H
