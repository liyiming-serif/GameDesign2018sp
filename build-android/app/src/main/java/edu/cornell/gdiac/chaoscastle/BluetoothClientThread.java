package edu.cornell.gdiac.chaoscastle;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.util.UUID;

/**
 * Created by Crest on 3/11/2018.
 */

class BluetoothClientThread extends Thread {
    private final BluetoothSocket mmSocket;
    private final BluetoothDevice mmDevice;

    ChaosCastle parent;

    BluetoothAdapter mba = BluetoothAdapter.getDefaultAdapter();
    UUID MY_UUID = UUID.fromString("757b1a35-43e2-4f69-b882-7acf1c9c6c6a");
    private static final String TAG = "CLIENT";

    private static final int TIMEOUT_MS = 5000;
    private TimeoutThread timer;

    public BluetoothClientThread(BluetoothDevice device, ChaosCastle p) {
        // Use a temporary object that is later assigned to mmSocket
        // because mmSocket is final.
        BluetoothSocket tmp = null;
        mmDevice = device;
        try {
            // Get a BluetoothSocket to connect with the given BluetoothDevice.
            // MY_UUID is the app's UUID string, also used in the server code.
            tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
        } catch (IOException e) {
            Log.e(TAG, "Socket's create() method failed", e);
        }
        parent = p;
        mmSocket = tmp;
    }

    public void run() {
        // Cancel discovery because it otherwise slows down the connection.
        mba.cancelDiscovery();

        // Set a timer which forces the socket to close after 5 sec.
        timer = new TimeoutThread(mmSocket, TIMEOUT_MS);
        timer.start();

        try {
            // Connect to the remote device through the socket. This call blocks
            // until it succeeds or throws an exception.
            mmSocket.connect();
            Log.d(TAG, "Connecting to socket");
        } catch (IOException connectException) {
            // Unable to connect; close the socket and get parent to show popup.
            Log.e(TAG, "Remote is not hosting a game.");
            parent.mHandler.obtainMessage(0,mmDevice.getName()).sendToTarget();
            try {
                mmSocket.close();
            } catch (IOException closeException) {
                Log.e(TAG, "Could not close the client socket", closeException);
            }
            return;
        }

        // The connection attempt succeeded. Perform work associated with
        // the connection in a separate thread.
        timer.interrupt();
        parent.connected(mmSocket);
    }

    // Closes the client socket and causes the thread to finish.
    public void cancel() {
        try {
            mmSocket.close();
        } catch (IOException e) {
            Log.e(TAG, "Could not close the client socket", e);
        }
    }

    //Inner class that exists only to close the client if it takes too long.
    private class TimeoutThread extends Thread{
        private int milliseconds; //socket timeout
        private BluetoothSocket clientSocket;
        public TimeoutThread(BluetoothSocket client, int ms){
            milliseconds = ms;
            clientSocket = client;
        }

        @Override
        public void run(){
            try {
                Thread.sleep(milliseconds);
                try {
                    Log.d("CLIENT", "Connection timed out!");
                    mmSocket.close();
                } catch (IOException closeException) {
                    Log.e("CLIENT", "Could not close the client socket", closeException);
                }
            }
            catch (java.lang.InterruptedException e){
                Log.d("CLIENT", "Connection successful; stopped timeout.");
            }
        }
    }
}
