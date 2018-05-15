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
	}

	public int getPlayers(){
	    if(isServer){
			if(bConnectedRing!=null){
				return bConnectedRing.size()+1;
			}
			return 1;
		}
		else{
	    	return -1;
		}
    }

	/**
	 * Call this when the server is ready to receive gameState packets.
	 * @return success value (0=success, 1=failure)
	 */
	public int clearServerACKs() {
		if(isServer){
			if(bConnectedRing==null||bConnectedRing.size()==0){
				return 1;
			}
			currClientIndex = (currClientIndex+1)%bConnectedRing.size();
			synchronized (this){
				//throws away result and just clear the buffer
				Log.d("SERVER", "clearing ACK messages from client"+currClientIndex);
				return bConnectedRing.get(currClientIndex).popState()==null ? 1:0;
			}
		}
		return 1;
	}

    /** Similar to consumeState, but the client reads states in FIFO order
	 *  and doesn't erase previous states.
	 */
    public byte[] consumeACK(){
		if(isServer){
			if(bConnectedRing==null || bConnectedRing.size()==0){
				return null;
			}
			try {
				currClientIndex = (currClientIndex+1)%bConnectedRing.size();
				String s;
				synchronized (this) {
					s = bConnectedRing.get(currClientIndex).dequeueState();
				}
				if(s==null){
					Log.d("SERVER", "got nothing");
					return null;
				}
				Log.d("SERVER", s);
				return s.getBytes("UTF-8");
			} catch (UnsupportedEncodingException ex) {
				return null;
			}
		}
		else {
			if (bConnected == null) {
				return null;
			}
			try {
				String s;
				synchronized (this) {
					s = bConnected.dequeueState();
				}
				if (s == null){
					Log.d("CLIENT", "got nothing");
					return null;
				}
				Log.d("CLIENT", s);
				return s.getBytes("UTF-8");
			} catch (UnsupportedEncodingException ex) {
				return null;
			}
		}
	}

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
                String s;
                synchronized (this) {
                    s = bConnectedRing.get(currClientIndex).dequeueState();
                }
                if(s==null){
                	Log.d("SERVER", "got nothing");
                	return null;
				}
				Log.d("SERVER", s);
                return s.getBytes("UTF-8");
            } catch (UnsupportedEncodingException ex) {
                return null;
            }
        }
        else {
            if (bConnected == null) {
                return null;
            }
            try {
                String s;
                synchronized (this) {
                    s = bConnected.popState();
                }
                if (s == null){
                	Log.d("CLIENT", "got nothing");
                	return null;
				}
				Log.d("CLIENT", s);
				return s.getBytes("UTF-8");
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
		mba.startDiscovery();
		int discClock = 0;
		while (discClock < 10) {
			discClock++;
		}
		updatePairedDevices();
		mba.cancelDiscovery();

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
				if (deviceClass == BluetoothClass.Device.PHONE_SMART ) {
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
		discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 800);
		startActivity(discoverableIntent);
	}


	public void setupBluetoothServer() {
	    makeDiscoverable();
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
            	//room is full; close server thread
                return false;
            }
            synchronized (this) {
            	//change server name to reflect number of players
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

	public void serverStopAccepting(){
		if(isServer&&bServer!=null){
			bServer.cancel();
		}
	}

	public void disconnect(BluetoothConnectedThread connection) {
		connection.cancel();
	}

	@Override
	protected void onDestroy() {
		// Don't forget to unregister the ACTION_FOUND receiver.
		unregisterReceiver(mReceiver);
		super.onDestroy();
	}
	
}
