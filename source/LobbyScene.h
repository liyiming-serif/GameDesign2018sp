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

    std::vector<std::string> serverDevices;
    
    // asset manager
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::Button> _backButton;
    std::shared_ptr<cugl::Button> _createButton;
    std::shared_ptr<cugl::Button>* _enterButtons;
    std::shared_ptr<cugl::Button> _refreshButton;
    std::shared_ptr<cugl::Label>* _enterTexts;

    std::shared_ptr<cugl::Button> _levelsButton;
    std::shared_ptr<cugl::PolygonNode> _avatar1;
    std::shared_ptr<cugl::PolygonNode> _avatar2;
    std::shared_ptr<cugl::Button> _avatar3;
    std::shared_ptr<cugl::Button> _avatar4;
    
    std::shared_ptr<cugl::PolygonNode> _background;
    std::shared_ptr<cugl::PolygonNode> _box;
    std::shared_ptr<cugl::PolygonNode> _player1;
    std::shared_ptr<cugl::PolygonNode> _player2;
    std::shared_ptr<cugl::PolygonNode> _waiting;
    
    std::shared_ptr<cugl::Node>  _lobby;
    std::shared_ptr<cugl::Node>  _avatar;
    
    
    std::shared_ptr<cugl::PolygonNode> _cloud1;
    std::shared_ptr<cugl::PolygonNode> _cloud2;
    std::shared_ptr<cugl::PolygonNode> _cloud3;
    std::shared_ptr<cugl::PolygonNode> _cloud4;
    std::shared_ptr<cugl::PolygonNode> _cloud5;
    std::shared_ptr<cugl::PolygonNode> _cloud6;
    
    
    /** Manager to process the animation actions */
    std::shared_ptr<cugl::ActionManager> _actions;
    
    /** The movement actions */
    std::shared_ptr<cugl::MoveTo> _move1;
    std::shared_ptr<cugl::MoveTo> _move2;
    std::shared_ptr<cugl::MoveTo> _move3;
    std::shared_ptr<cugl::MoveTo> _move4;
    std::shared_ptr<cugl::MoveTo> _move5;
    std::shared_ptr<cugl::MoveTo> _move6;
    
    bool move1 = true;
    bool move2 = true;
    bool move3 = true;
    bool move4 = true;
    bool move5 = true;
    bool move6 = true;

    size_t length;

    int LobbyClock = 0;

    bool clientReady = false;
    bool serverReady = false;

    
    
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
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove2(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove3(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove4(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove5(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    /**
     * Performs a move action
     *
     * @param action The move action
     */
    void doMove6(const std::shared_ptr<cugl::MoveTo>& action, std::shared_ptr<cugl::PolygonNode> object);
    
    void setButtonActive ( std::shared_ptr<cugl::Button> button, std::string actKey);
    
    void changeCanvas (std::string canvas);

    std::shared_ptr<cugl::Button> createServerRoomButton(int device);

    std::shared_ptr<cugl::Label> createServerRoomText(int device);

    void refreshRooms();


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

    void setupBluetoothClient(int serverDevice) {
        // Set up parameters for JNI call
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "setupBluetoothClient",
                                               "(I)V");

        // Call the Java method
        env->CallVoidMethod(activity, method_id, serverDevice);

        // Free local references
        env->DeleteLocalRef(activity);
        env->DeleteLocalRef(clazz);
    }

    std::vector<std::string> getServerDevices() {
        // Set up parameters for JNI call
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "getServerDevices",
                                               "()[Ljava/lang/String;");

        // Call the Java method
        jobjectArray array = (jobjectArray) env->CallObjectMethod(activity, method_id);
        std::vector<std::string> Devices;

        if (array == NULL) {
            env->DeleteLocalRef(activity);
            env->DeleteLocalRef(clazz);
            env->DeleteLocalRef(array);

            return Devices;
        }
        else {
            int stringCount = env->GetArrayLength(array);


            for (int i=0; i<stringCount; i++) {
                jstring string = (jstring) (env->GetObjectArrayElement(array, i));
                const char *rawString = env->GetStringUTFChars(string, 0);
                Devices.push_back((std::string)rawString);
                // Don't forget to call `ReleaseStringUTFChars` when you're done.
                env->ReleaseStringUTFChars(string, rawString);
                env->DeleteLocalRef(string);
            }

            // Free local references
            env->DeleteLocalRef(activity);
            env->DeleteLocalRef(clazz);
            env->DeleteLocalRef(array);

            return Devices;
        }

    }

    int getPlayers() {
        // Set up parameters for JNI call
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "getPlayers",
                                               "()I");

        // Call the Java method
        int players = env->CallIntMethod(activity, method_id);

        // Free local references
        env->DeleteLocalRef(activity);
        env->DeleteLocalRef(clazz);

        return players;
    }

    void serverStopAccepting() {
        // Set up parameters for JNI call
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "serverStopAccepting",
                                               "()V");

        // Call the Java method
        env->CallVoidMethod(activity, method_id);

        // Free local references
        env->DeleteLocalRef(activity);
        env->DeleteLocalRef(clazz);
    }

    char* consumeACK() {
        // Set up parameters for JNI call
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "consumeACK",
                                               "()[B");

        jbyteArray array = (jbyteArray) env->CallObjectMethod(activity, method_id);

        if (array == NULL) {
            //CULog("Read Byte Array is Null");
            env->DeleteLocalRef(activity);
            env->DeleteLocalRef(clazz);
            return NULL;
        }
        else {
            //CULog("Read Byte Array is not Null");
            jbyte* buffer = env->GetByteArrayElements(array, NULL);
            jsize size = env->GetArrayLength(array);
            char *byte_buffer = new char[size];

            for(int i = 0; i < size; i++) {
                byte_buffer[i] = buffer[i];
            }
            env->ReleaseByteArrayElements(array, buffer, JNI_ABORT);

            // Free local references
            env->DeleteLocalRef(array);
            env->DeleteLocalRef(activity);
            env->DeleteLocalRef(clazz);
            return byte_buffer;
        }
    }

private:
    std::string produceACKClient();
    std::string produceACKServer();
    char* consumeACKClient();
    char** consumeACKServer();
    void applyACKClient(char* ACK);
    void applyACKServer(char** ACKs);

    char* return_buffer(const std::string& string);

    void runLobbyNetworking();

    int sendState(char* byte_buffer) {
        // Set up parameters for JNI call
        JNIEnv *env = (JNIEnv *) SDL_AndroidGetJNIEnv();
        jobject activity = (jobject) SDL_AndroidGetActivity();

        jclass clazz(env->GetObjectClass(activity));
        jmethodID method_id = env->GetMethodID(clazz, "sendState",
                                               "([B)I");

        if (byte_buffer != NULL) {
            // Call the Java method
            //CULog("Attempting to send across JNI");
            int n=0;
            char* p = byte_buffer;
            while(*p++){
                //CULog("N is currently: %i", n);
                n++;
            } if(n<=0) {
                CULog("sendState: byte_buffer not big enough");
                return NULL;
            }
            else {
                CULog("Size of byte_buffer passed to send State is %i", n);
            }
            jbyteArray arr = env->NewByteArray(n);
            env->SetByteArrayRegion(arr,0,n, (jbyte*)byte_buffer);

            int response = env->CallIntMethod(activity, method_id, arr);

            // Free local references
            //env->ReleaseByteArrayElements(arr, (jbyte*)byte_buffer, JNI_ABORT);
            //free(p);
            env->DeleteLocalRef(arr);
            env->DeleteLocalRef(activity);
            env->DeleteLocalRef(clazz);
            return response;
        }
        else {
            CULog("sendState: attempt to send Null message");
            env->DeleteLocalRef(activity);
            env->DeleteLocalRef(clazz);
            return NULL;
        }
    }
#endif
    void animateClouds();

};


#endif /* LobbyScene_h */
