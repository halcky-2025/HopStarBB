package org.libsdl.app;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;

/**
 * HSNearby - Android nearby discovery/sharing implementation for HopStar
 * Uses BLE for discovery and Nearby Connections API for transfer
 */
public class HSNearby {
    private static final String TAG = "HSNearby";
    private static final int PERMISSION_REQUEST_CODE = 4001;

    // Mode constants (match C++ enum)
    private static final int MODE_OFF = 0;
    private static final int MODE_PASSIVE = 1;
    private static final int MODE_DISCOVER = 2;
    private static final int MODE_PAIRING = 3;
    private static final int MODE_TRANSFER = 4;

    private static BluetoothManager bluetoothManager;
    private static BluetoothAdapter bluetoothAdapter;
    private static BluetoothLeScanner bleScanner;
    private static int currentMode = MODE_OFF;
    private static boolean isScanning = false;
    private static Handler handler;

    private static Map<String, NearbyPeer> discoveredPeers = new HashMap<>();

    // Native callbacks
    private static native void nativeOnPeerDiscovered(String peerId, String deviceName, int rssi);
    private static native void nativeOnPeerLost(String peerId);
    private static native void nativeOnTransferRequest(String transferId, String peerId, String fileName, long fileSize);
    private static native void nativeOnTransferProgress(String transferId, float progress, int state);
    private static native void nativeOnPermissionResult(boolean granted);

    private static class NearbyPeer {
        String peerId;
        String deviceName;
        int rssi;
        long lastSeen;
    }

    public static void initialize() {
        Context context = SDLActivity.getContext();
        bluetoothManager = (BluetoothManager) context.getSystemService(Context.BLUETOOTH_SERVICE);
        if (bluetoothManager != null) {
            bluetoothAdapter = bluetoothManager.getAdapter();
            if (bluetoothAdapter != null) {
                bleScanner = bluetoothAdapter.getBluetoothLeScanner();
            }
        }
        handler = new Handler(Looper.getMainLooper());
        Log.d(TAG, "Initialized");
    }

    public static void shutdown() {
        setMode(MODE_OFF);
        discoveredPeers.clear();
        Log.d(TAG, "Shutdown");
    }

    public static boolean hasPermission() {
        Context context = SDLActivity.getContext();

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            return ContextCompat.checkSelfPermission(context,
                    Manifest.permission.BLUETOOTH_SCAN) == PackageManager.PERMISSION_GRANTED &&
                   ContextCompat.checkSelfPermission(context,
                    Manifest.permission.BLUETOOTH_ADVERTISE) == PackageManager.PERMISSION_GRANTED &&
                   ContextCompat.checkSelfPermission(context,
                    Manifest.permission.BLUETOOTH_CONNECT) == PackageManager.PERMISSION_GRANTED;
        } else {
            return ContextCompat.checkSelfPermission(context,
                    Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED;
        }
    }

    public static void requestPermission() {
        Activity activity = (Activity) SDLActivity.getContext();

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            ActivityCompat.requestPermissions(activity,
                    new String[]{
                            Manifest.permission.BLUETOOTH_SCAN,
                            Manifest.permission.BLUETOOTH_ADVERTISE,
                            Manifest.permission.BLUETOOTH_CONNECT,
                            Manifest.permission.ACCESS_FINE_LOCATION
                    },
                    PERMISSION_REQUEST_CODE);
        } else {
            ActivityCompat.requestPermissions(activity,
                    new String[]{
                            Manifest.permission.ACCESS_FINE_LOCATION,
                            Manifest.permission.ACCESS_COARSE_LOCATION
                    },
                    PERMISSION_REQUEST_CODE);
        }
    }

    public static void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode == PERMISSION_REQUEST_CODE) {
            boolean allGranted = true;
            for (int result : grantResults) {
                if (result != PackageManager.PERMISSION_GRANTED) {
                    allGranted = false;
                    break;
                }
            }
            nativeOnPermissionResult(allGranted);
        }
    }

    public static void setMode(int mode) {
        if (currentMode == mode) return;

        Log.d(TAG, "Mode: " + currentMode + " -> " + mode);

        // Stop current mode
        stopCurrentMode();

        currentMode = mode;

        // Start new mode
        switch (mode) {
            case MODE_PASSIVE:
                // BLE advertising only
                startAdvertising();
                break;
            case MODE_DISCOVER:
            case MODE_PAIRING:
                startAdvertising();
                startScanning();
                break;
            case MODE_TRANSFER:
                // Keep connections, stop scanning
                break;
            case MODE_OFF:
            default:
                // Already stopped
                break;
        }
    }

    private static void stopCurrentMode() {
        stopScanning();
        stopAdvertising();
    }

    private static void startAdvertising() {
        // BLE advertising would go here
        // For production, use BluetoothLeAdvertiser
        Log.d(TAG, "Start advertising");
    }

    private static void stopAdvertising() {
        Log.d(TAG, "Stop advertising");
    }

    private static void startScanning() {
        if (isScanning || bleScanner == null) return;

        if (!hasPermission()) {
            Log.w(TAG, "Missing BLE permissions");
            return;
        }

        try {
            ScanSettings settings = new ScanSettings.Builder()
                    .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
                    .build();

            bleScanner.startScan(null, settings, scanCallback);
            isScanning = true;
            Log.d(TAG, "Started BLE scanning");

            // Auto-stop after 30 seconds
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    if (isScanning && currentMode == MODE_DISCOVER) {
                        stopScanning();
                    }
                }
            }, 30000);

        } catch (SecurityException e) {
            Log.e(TAG, "SecurityException starting scan", e);
        }
    }

    private static void stopScanning() {
        if (!isScanning || bleScanner == null) return;

        try {
            bleScanner.stopScan(scanCallback);
            isScanning = false;
            Log.d(TAG, "Stopped BLE scanning");
        } catch (SecurityException e) {
            Log.e(TAG, "SecurityException stopping scan", e);
        }
    }

    private static ScanCallback scanCallback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            BluetoothDevice device = result.getDevice();
            if (device == null) return;

            String address = device.getAddress();
            String name = null;
            try {
                name = device.getName();
            } catch (SecurityException e) {
                // Permission not granted
            }

            if (name == null || name.isEmpty()) {
                name = "Unknown Device";
            }

            int rssi = result.getRssi();

            // Update or add peer
            NearbyPeer peer = discoveredPeers.get(address);
            if (peer == null) {
                peer = new NearbyPeer();
                peer.peerId = address;
                peer.deviceName = name;
                discoveredPeers.put(address, peer);

                // Notify native
                nativeOnPeerDiscovered(address, name, rssi);
            }

            peer.rssi = rssi;
            peer.lastSeen = System.currentTimeMillis();
        }

        @Override
        public void onBatchScanResults(List<ScanResult> results) {
            for (ScanResult result : results) {
                onScanResult(0, result);
            }
        }

        @Override
        public void onScanFailed(int errorCode) {
            Log.e(TAG, "Scan failed: " + errorCode);
            isScanning = false;
        }
    };

    public static void sendFile(String peerId, String filePath) {
        Log.d(TAG, "Send file: " + filePath + " to " + peerId);
        // For production, use Nearby Connections API
        // ConnectionsClient.sendPayload()
    }

    public static void acceptTransfer(String transferId) {
        Log.d(TAG, "Accept transfer: " + transferId);
        // Accept incoming payload
    }

    public static void rejectTransfer(String transferId) {
        Log.d(TAG, "Reject transfer: " + transferId);
        // Reject incoming payload
    }

    // Cleanup stale peers
    public static void cleanupStalePeers() {
        long now = System.currentTimeMillis();
        long timeout = 30000; // 30 seconds

        for (Map.Entry<String, NearbyPeer> entry : discoveredPeers.entrySet()) {
            if (now - entry.getValue().lastSeen > timeout) {
                nativeOnPeerLost(entry.getKey());
                discoveredPeers.remove(entry.getKey());
            }
        }
    }
}
