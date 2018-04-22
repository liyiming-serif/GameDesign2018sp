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

	BluetoothServerThread bServer = null;
	BluetoothClientThread bClient = null;
	BluetoothAdapter mba = null;
	BluetoothConnectedThread bConnected = null;
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

	/** Call consumeState from C++ to send one COMPLETE game state
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
		BluetoothAdapter mba = BluetoothAdapter.getDefaultAdapter();
		if (mba == null) {
			System.out.print("Bluetooth Unavailable");
		}

		if (!mba.isEnabled()) {
			Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
		}
	}

	public String[] getPairedDevicesString(BluetoothAdapter mba) {
		Set<BluetoothDevice> pairedDevices = mba.getBondedDevices();
		int pairedPhoneSize = 0;

		if (pairedDevices.size() > 0) {
			// There are paired devices. Get the name and address of each paired device.
			for (BluetoothDevice device : pairedDevices) {
				int deviceClass = device.getBluetoothClass().getDeviceClass();
				if (deviceClass == BluetoothClass.Device.PHONE_SMART) {
					pairedPhoneSize++;
				}
			}
			String[] pairedPhoneList = new String[pairedPhoneSize];
			int i = 0;
			for (BluetoothDevice device : pairedDevices) {
				String deviceName = device.getName();
				int deviceClass = device.getBluetoothClass().getDeviceClass();
				if (deviceClass == BluetoothClass.Device.PHONE_SMART) {
					pairedPhoneList[i] = deviceName;
					i++;
				}
			}
			return pairedPhoneList;
		}
		else {
			String[] pairedPhoneList = new String[0];
			return pairedPhoneList;
		}
	}

	public Set<BluetoothDevice> getPairedDevices(BluetoothAdapter mba) {
		Set<BluetoothDevice> pairedDevices = mba.getBondedDevices();
		Set<BluetoothDevice> pairedPhones = new HashSet<>();

		if (pairedDevices.size() > 0) {
			// There are paired devices. Get the name and address of each paired device.
			for (BluetoothDevice device : pairedDevices) {
				int deviceClass = device.getBluetoothClass().getDeviceClass();
				if (deviceClass == BluetoothClass.Device.PHONE_SMART) {
					pairedPhones.add(device);
				}
			}
		}
		return pairedPhones;
	}

	public void findNewDevices(BluetoothAdapter mba) {
		if (!mba.startDiscovery()) {
			System.out.print("Discovery Failed");
		}
	}

	public void makeDiscoverable(BluetoothAdapter mba) {
		Intent discoverableIntent =
				new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
		//discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
		startActivity(discoverableIntent);
	}

	public void setupBluetoothServer() {
	    isServer = true;
	    bConnectedRing = new ArrayList<BluetoothConnectedThread>(5);
		bServer = new BluetoothServerThread(this);
		bServer.start();
	}

	public void setupBluetoothClient() {
		Set<BluetoothDevice> newDevices = getPairedDevices(mba);
		BluetoothDevice[] newDevicesArray = newDevices.toArray(new BluetoothDevice[1]);
		Log.d("CLIENT", "Bluetooth Device Paired " +newDevicesArray[0].getName());
		bClient = new BluetoothClientThread(newDevicesArray[0], this);
		bClient.start();
	}

	public boolean connected(BluetoothSocket mmSocket) {
		Log.d("CONNECTED", "connected: Starting.");

		// Start the thread to manage the connection and perform transmissions
        if(isServer){
            if(bConnectedRing.size()>=MAX_PLAYERS){
                return false;
            }
            synchronized (this) {
                BluetoothConnectedThread b = new BluetoothConnectedThread(mmSocket);
                bConnectedRing.add(b);
                b.start();
                Log.d("CONNECTED", "Connected Thread Starting");
            }
            return true;
        }
        else {
            synchronized (this) {
                bConnected = new BluetoothConnectedThread(mmSocket);
                bConnected.start();
				Log.d("CONNECTED", "Connected Thread Starting");
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
		unregisterReceiver(mReceiver);
	}
	
}
