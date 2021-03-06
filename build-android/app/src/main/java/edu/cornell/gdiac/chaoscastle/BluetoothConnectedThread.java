package edu.cornell.gdiac.chaoscastle;

import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayDeque;
import java.util.Arrays;

/**
 * Created by accoo on 3/15/2018.
 */

public class BluetoothConnectedThread extends Thread {
    private final BluetoothSocket mmSocket;
    private final InputStream mmInStream;
    private final OutputStream mmOutStream;
    private byte[] mmBuffer; // mmBuffer store for the stream
    private final String TAG = "CONNECTED";
    private ArrayDeque<String> gameStates;

    public BluetoothConnectedThread(BluetoothSocket socket) {
        mmSocket = socket;
        gameStates = new ArrayDeque<String>();
        InputStream tmpIn = null;
        OutputStream tmpOut = null;

        // Get the input and output streams; using temp objects because
        // member streams are final.
        try {
            tmpIn = socket.getInputStream();
        } catch (IOException e) {
            Log.e(TAG, "Error occurred when creating input stream", e);
        }
        try {
            tmpOut = socket.getOutputStream();
        } catch (IOException e) {
            Log.e(TAG, "Error occurred when creating output stream", e);
        }

        mmInStream = tmpIn;
        mmOutStream = tmpOut;
    }

    public void run() {
        String gameState;

        // Keep listening for incoming game states until an exception occurs.
        while (true) {
            // Read from the InputStream.
            gameState = extractMessage();
            if(gameState == null){
                break;
            }
            synchronized (this){
                gameStates.add(gameState);
            }
        }
    }

    /** Extracts FULL game state messsage; keeps reading
     */
    private String extractMessage(){
        boolean success = true;
        mmBuffer = new byte[1024]; //chunk
        String acc = ""; //total
        int len; //total length of message
        int numBytes; //bytes returned from read
        do{
            try {
                numBytes = mmInStream.read(mmBuffer);
//                mmBuffer = Arrays.copyOfRange(mmBuffer,0,numBytes);
                acc += new String(mmBuffer, "UTF-8");
                //Log.d(TAG, "Read bytes: "+acc);
                String delims = "[|]";
                len = Integer.parseInt(acc.split(delims)[0]);
            } catch (IOException e) {
                Log.d(TAG, "Input stream was disconnected", e);
                success = false;
                break;
            }
        }while(acc.length()<len);

        if(success) {
            return acc;
        }
        else{
            return null;
        }
    }

    /**Call this from UI activity to poll the game state from this reader*/
    public String dequeueState(){
        String result;
        synchronized (this){
            result = gameStates.pollFirst();
        }
        return result;
    }

    /**dequeueState for client. Client always needs most up to date game state.*/
    public String popState(){
        String result;
        synchronized (this){
            result = gameStates.pollLast();
            gameStates.clear();
        }
        return result;
    }

    // Call this from the main activity to send data to the remote device.
    public int write(byte[] bytes) {
        try {
            mmOutStream.write(bytes);
            return 0;
        } catch (IOException e) {
            Log.e(TAG, "Error occurred when sending data", e);
            return 1;
        }
    }

    // Call this method from the main activity to shut down the connection.
    public void cancel() {
        try {
            mmSocket.close();
        } catch (IOException e) {
            Log.e(TAG, "Could not close the connect socket", e);
        }
    }
}
