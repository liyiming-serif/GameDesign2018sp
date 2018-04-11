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
#if CU_PLATFORM == CU_PLATFORM_ANDROID
#include <jni.h>
#endif

class LobbyScene : public cugl::Scene{
protected:
    cugl::Size _size;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _backButton;
    std::shared_ptr<cugl::Button> _createButton;
    std::shared_ptr<cugl::Button> _enterButton;
    std::shared_ptr<cugl::Button> _levelsButton;
	bool _deactivateCreate;
	bool _deactivateEnter;
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

#if CU_PLATFORM == CU_PLATFORM_ANDROID
    void setupBluetoothServer() {
        // Set up parameters for JNI call
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "setupBluetoothServer",
                                               "()V");

        // Call the Java method
        env->CallVoidMethod(activity, method_id);

        // Free local references
        env->DeleteLocalRef(activity);
        env->DeleteLocalRef(clazz);
   }

    void setupBluetoothClient() {
        // Set up parameters for JNI call
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "setupBluetoothClient",
                                               "()V");

        // Call the Java method
        env->CallVoidMethod(activity, method_id);

        // Free local references
        env->DeleteLocalRef(activity);
        env->DeleteLocalRef(clazz);
    }
#endif
};


#endif /* LobbyScene_h */
