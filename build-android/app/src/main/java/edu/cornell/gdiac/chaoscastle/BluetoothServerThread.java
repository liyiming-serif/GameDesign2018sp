package edu.cornell.gdiac.chaoscastle;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.util.UUID;

/**
 * Created by Crest on 3/11/2018.
 */

class BluetoothServerThread extends Thread {
    private final BluetoothServerSocket mmServerSocket;
    public BluetoothSocket mmSocket;

    BluetoothAdapter mba = BluetoothAdapter.getDefaultAdapter();
    UUID MY_UUID = UUID.fromString("757b1a35-43e2-4f69-b882-7acf1c9c6c6a");
    private static final String TAG = "SERVER";

    public BluetoothServerThread() {
        // Use a temporary object that is later assigned to mmServerSocket
        // because mmServerSocket is final.
        BluetoothServerSocket tmp = null;
        try {
            // MY_UUID is the app's UUID string, also used by the client code.
            tmp = mba.listenUsingRfcommWithServiceRecord("CHAOS_CASTLE", MY_UUID);
        } catch (IOException e) {
            Log.e(TAG, "Socket's listen() method failed", e);
        }
        mmServerSocket = tmp;
    }

    public void run() {
        BluetoothSocket socket = null;
        // Keep listening until exception occurs or a socket is returned.
        while (true) {
            try {
                socket = mmServerSocket.accept();
            } catch (IOException e) {
                Log.e(TAG, "Socket's accept() method failed", e);
                break;
            }

            if (socket != null) {
                // A connection was accepted. Perform work associated with
                // the connection in a separate thread.

                //TODO: Implement "manage my connected socket" function to handle incoming messages
                mmSocket = socket;
                Log.d(TAG, "Server connected");
                try {
                    mmServerSocket.close();
                } catch (IOException e) {
                    Log.e(TAG, "Could not close the connect socket", e);
                }
                break;
            }
        }
    }

    // Closes the connect socket and causes the thread to finish.
    public void cancel() {
        try {
            mmServerSocket.close();
        } catch (IOException e) {
            Log.e(TAG, "Could not close the connect socket", e);
        }
    }
}
