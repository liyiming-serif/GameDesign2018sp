package edu.cornell.gdiac.chaoscastle;

import org.libsdl.app.*;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothClass;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.*;
import android.util.Log;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;
import java.io.UnsupportedEncodingException;


/**
 * Default application entry point
 */
public class ChaosCastle extends SDLActivity {

	private static final int REQUEST_ENABLE_BT = 1;
	private static final int MAX_PLAYERS = 5;

	BluetoothServerThread bServer;
	BluetoothClientThread bClient;
	BluetoothAdapter mba;
	String origName;
	ArrayList<BluetoothDevice> pairedServers;
	BluetoothConnectedThread bConnected;
	ArrayList<BluetoothConnectedThread> bConnectedRing;

	boolean isServer = false;
	int currClientIndex = -1;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
	    // Make sure this is before calling super.onCreate
	    setTheme(R.style.CUGLTheme);
	    super.onCreate(savedInstanceState);
		IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
		registerReceiver(mReceiver, filter);
		mba = BluetoothAdapter.getDefaultAdapter();
		origName = mba.getName();
	}

	//TODO: Use this method to read from the reading-connected thread (pass bytes to C side)

	/** Call consumeState from C++ to get one COMPLETE game state
	 *  from the bluetooth socket.
	 */
	public byte[] consumeState(){
	    if(isServer){
            if(bConnectedRing==null || bConnectedRing.size()==0){
                return null;
            }
            try {
                currClientIndex = (currClientIndex+1)%bConnectedRing.size();
                byte[] result;
                synchronized (this) {
                    result = bConnectedRing.get(currClientIndex).dequeueState().getBytes("UTF-8");
                }
                return result;
            } catch (UnsupportedEncodingException ex) {
                return null;
            }
        }
        else {
            if (bConnected == null) {
                return null;
            }
            try {
                byte[] result;
                synchronized (this) {
                    result = bConnected.dequeueState().getBytes("UTF-8");
                }
                return result;
            } catch (UnsupportedEncodingException ex) {
                return null;
            }
        }
	}

	/** Call sendState from C++ to send one COMPLETE game state
	 *  to the bluetooth socket.
	 */
	public int sendState(byte[] byte_buffer) {
		int status = 0;
	    if(isServer){
	    	if(bConnectedRing==null || bConnectedRing.size()==0){
	    		return 1;
			}
	        synchronized (this) {
                for (BluetoothConnectedThread b : bConnectedRing) {
                    status = b.write(byte_buffer);
                }
            }
        }
        else {
            if (bConnected == null) {
                return 1;
            }
            synchronized (this) {
                status = bConnected.write(byte_buffer);
            }
        }
        return status;
	}

	private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			if (BluetoothDevice.ACTION_FOUND.equals(action)) {
				// Discovery has found a device. Get the BluetoothDevice
				// object and its info from the Intent.
				BluetoothDevice newDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
				String deviceName = newDevice.getName();
				String deviceHardwareAddress = newDevice.getAddress(); // MAC address
			}
		}
	};

	public void enableBluetooth() {
		if (mba == null) {
			System.out.print("Bluetooth Unavailable");
		}

		if (!mba.isEnabled()) {
			Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
		}
	}

	/**
	 * Called by C++. Updates paired devices, then returns its toString.
	 */
	public String[] getServerDevices() {
		updatePairedDevices();

		if (pairedServers.size() > 0) {
			// There are open servers. Enumerate and return their names.
			String[] result = new String[pairedServers.size()];
			int i = 0;
			for (BluetoothDevice device : pairedServers) {
				result[i] = device.getName();
				i++;
			}
			return result;
		}
		else {
			return null;
		}
	}

	/**
	 * Filter bonded devices by phone, then by SERVER.
	 * Populates pairedServers with updated devices.
	 */
	public void updatePairedDevices() {
		Set<BluetoothDevice> pairedDevices = mba.getBondedDevices();
		pairedServers = new ArrayList<BluetoothDevice>(pairedDevices.size());

		if (pairedDevices.size() > 0) {
			// There are paired devices. Get the name and address of each paired device.
			for (BluetoothDevice device : pairedDevices) {
				int deviceClass = device.getBluetoothClass().getDeviceClass();
				if (deviceClass == BluetoothClass.Device.PHONE_SMART &&
						device.getName().matches("SERVER[0-9].*")) {
					pairedServers.add(device);
				}
			}
		}
	}

	public void findNewDevices() {
		if (!mba.startDiscovery()) {
			System.out.print("Discovery Failed");
		}
	}

	public void makeDiscoverable() {
		Intent discoverableIntent =
				new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
		//discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
		startActivity(discoverableIntent);
	}

	public void changeServerName(int numPlayers){
		if(isServer){ //is already a server, just updating numPlayers
			String newName = "SERVER"+numPlayers+mba.getName().substring(7);
			mba.setName(newName);
		}
		else{ //first time setting up this server
			String newName = "SERVER"+numPlayers+mba.getName();
			mba.setName(newName);
		}
	}

	public void setupBluetoothServer() {
	    changeServerName(1);
	    isServer = true;
	    bConnectedRing = new ArrayList<BluetoothConnectedThread>(5);
		bServer = new BluetoothServerThread(this);
		bServer.start();
	}

	public void setupBluetoothClient(int serverIndx) {
		if(pairedServers==null){
			Log.e("CLIENT", "Forgot to call paired devices");
			return;
		}
		if(serverIndx < 0 || serverIndx >=pairedServers.size()){
			Log.e("CLIENT", "Server "+serverIndx+" not found.");
			return;
		}
		Log.d("CLIENT", "Bluetooth Device Paired " +pairedServers.get(serverIndx).getName());
		bClient = new BluetoothClientThread(pairedServers.get(serverIndx), this);
		bClient.start();
	}

	public boolean connected(BluetoothSocket mmSocket) {
		Log.d("CONNECTED", "Starting connection");

		// Start the thread to manage the connection and perform transmissions
        if(isServer){
            if(bConnectedRing.size()>=MAX_PLAYERS){
                return false;
            }
            synchronized (this) {
            	//change server name to reflect number of players
				changeServerName(bConnectedRing.size()+1);
                BluetoothConnectedThread b = new BluetoothConnectedThread(mmSocket);
                bConnectedRing.add(b);
                b.start();
                Log.d("CONNECTED", "Another player has joined "+mba.getName());
            }
            return true;
        }
        else {
            synchronized (this) {
                bConnected = new BluetoothConnectedThread(mmSocket);
                bConnected.start();
				Log.d("CONNECTED", "Joining server!");
            }
            return true;
        }
	}

	public void disconnect(BluetoothConnectedThread connection) {
		connection.cancel();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		// Don't forget to unregister the ACTION_FOUND receiver.
		mba.setName(origName);
		unregisterReceiver(mReceiver);
	}
	
}
