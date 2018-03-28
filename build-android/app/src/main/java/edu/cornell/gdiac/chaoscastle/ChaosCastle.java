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
import java.util.HashSet;
import java.util.Set;


/**
 * Default application entry point
 */
public class ChaosCastle extends SDLActivity {

	private static final int REQUEST_ENABLE_BT = 1;

	BluetoothServerThread bServer = null;
	BluetoothClientThread bClient = null;
	BluetoothAdapter mba = null;
	BluetoothConnectedThread bConneted = null;
	BluetoothSocket mmSocket = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
	    // Make sure this is before calling super.onCreate
	    setTheme(R.style.CUGLTheme);
	    super.onCreate(savedInstanceState);
		IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
		registerReceiver(mReceiver, filter);
	}

	//TODO: Use this method to read from the reading-connected thread (pass bytes to C side)
	private static native void readNetwork(byte[] byte_buffer);

	//TODO: Use this method to write to the writing-connected thread (might need to be native function)
	public void writeNetwork(byte[] byte_buffer) {
		return;
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

	public BluetoothAdapter enableBluetooth() {
		BluetoothAdapter mba = BluetoothAdapter.getDefaultAdapter();
		if (mba == null) {
			System.out.print("Bluetooth Unavailable");
		}

		if (!mba.isEnabled()) {
			Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
		}
		return mba;
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
		bServer = new BluetoothServerThread();
		bServer.start();
		setmmSocket(bServer.mmSocket);
		connected(mmSocket);
	}

	public void setupBluetoothClient() {
		Set<BluetoothDevice> newDevices = getPairedDevices(mba);
		BluetoothDevice[] newDevicesArray = newDevices.toArray(new BluetoothDevice[1]);
		Log.d("CLIENT", "Bluetooth Device Paired " +newDevicesArray[0].getName());
		bClient = new BluetoothClientThread(newDevicesArray[0]);
		bClient.start();
		setmmSocket(bClient.mmSocketFinal);
		connected(mmSocket);
	}

	public void setmmSocket(BluetoothSocket mmSocket) {
		this.mmSocket = mmSocket;
	}

	public void connected(BluetoothSocket mmSocket) {
		Log.d("CONNECTED", "connected: Starting.");

		// Start the thread to manage the connection and perform transmissions
		bConneted = new BluetoothConnectedThread(this.mmSocket);
		bConneted.start();
	}

	public void disconnected(BluetoothConnectedThread bConnected) {
		bConnected.cancel();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		// Don't forget to unregister the ACTION_FOUND receiver.
		unregisterReceiver(mReceiver);
	}
	
}
