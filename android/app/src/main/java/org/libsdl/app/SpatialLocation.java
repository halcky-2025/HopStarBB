package org.libsdl.app;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Looper;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

/**
 * SpatialLocation - Android location services for HopStar
 */
public class SpatialLocation implements LocationListener {
    private static final String TAG = "SpatialLocation";
    private static final int PERMISSION_REQUEST_CODE = 1001;

    private static SpatialLocation instance;
    private Context context;
    private LocationManager locationManager;
    private boolean isRunning = false;

    // Native callback pointer
    private long nativeHubPtr = 0;

    // Location settings
    private long minTimeMs = 1000;      // 1 second
    private float minDistanceM = 1.0f;  // 1 meter

    private SpatialLocation(Context ctx) {
        this.context = ctx.getApplicationContext();
        this.locationManager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);
    }

    public static synchronized SpatialLocation getInstance(Context ctx) {
        if (instance == null) {
            instance = new SpatialLocation(ctx);
        }
        return instance;
    }

    public void setNativeHub(long ptr) {
        this.nativeHubPtr = ptr;
    }

    public boolean hasLocationPermission() {
        return ContextCompat.checkSelfPermission(context, Manifest.permission.ACCESS_FINE_LOCATION)
                == PackageManager.PERMISSION_GRANTED;
    }

    public void requestPermission(Activity activity) {
        if (!hasLocationPermission()) {
            ActivityCompat.requestPermissions(activity,
                    new String[]{
                            Manifest.permission.ACCESS_FINE_LOCATION,
                            Manifest.permission.ACCESS_COARSE_LOCATION
                    },
                    PERMISSION_REQUEST_CODE);
        }
    }

    public boolean isLocationEnabled() {
        return locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER) ||
               locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
    }

    public void start() {
        if (isRunning) {
            Log.d(TAG, "Already running");
            return;
        }

        if (!hasLocationPermission()) {
            Log.e(TAG, "Location permission not granted");
            return;
        }

        try {
            // Try GPS first, fall back to network
            if (locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
                locationManager.requestLocationUpdates(
                        LocationManager.GPS_PROVIDER,
                        minTimeMs,
                        minDistanceM,
                        this,
                        Looper.getMainLooper()
                );
                Log.d(TAG, "Started GPS location updates");
            }

            if (locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER)) {
                locationManager.requestLocationUpdates(
                        LocationManager.NETWORK_PROVIDER,
                        minTimeMs,
                        minDistanceM,
                        this,
                        Looper.getMainLooper()
                );
                Log.d(TAG, "Started Network location updates");
            }

            isRunning = true;

            // Get last known location immediately
            Location lastGps = locationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
            Location lastNetwork = locationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);

            Location best = null;
            if (lastGps != null && lastNetwork != null) {
                best = (lastGps.getTime() > lastNetwork.getTime()) ? lastGps : lastNetwork;
            } else if (lastGps != null) {
                best = lastGps;
            } else if (lastNetwork != null) {
                best = lastNetwork;
            }

            if (best != null) {
                onLocationChanged(best);
            }

        } catch (SecurityException e) {
            Log.e(TAG, "SecurityException: " + e.getMessage());
        }
    }

    public void stop() {
        if (!isRunning) return;

        try {
            locationManager.removeUpdates(this);
            isRunning = false;
            Log.d(TAG, "Stopped location updates");
        } catch (Exception e) {
            Log.e(TAG, "Error stopping: " + e.getMessage());
        }
    }

    public void setUpdateInterval(long minTimeMs, float minDistanceM) {
        this.minTimeMs = minTimeMs;
        this.minDistanceM = minDistanceM;

        // Restart if running to apply new settings
        if (isRunning) {
            stop();
            start();
        }
    }

    // LocationListener implementation
    @Override
    public void onLocationChanged(Location location) {
        if (location == null) return;

        double lat = location.getLatitude();
        double lon = location.getLongitude();
        double alt = location.getAltitude();
        float hAcc = location.getAccuracy();
        float vAcc = location.hasVerticalAccuracy() ? location.getVerticalAccuracyMeters() : -1;
        float speed = location.hasSpeed() ? location.getSpeed() : 0;
        float course = location.hasBearing() ? location.getBearing() : 0;

        Log.v(TAG, String.format("Location: %.6f, %.6f, alt=%.1f, acc=%.1f",
                lat, lon, alt, hAcc));

        // Call native
        if (nativeHubPtr != 0) {
            nativeUpdateLocation(nativeHubPtr, lat, lon, alt, hAcc, vAcc, speed, course);
        }
    }

    @Override
    public void onStatusChanged(String provider, int status, Bundle extras) {
        Log.d(TAG, "Provider " + provider + " status changed: " + status);
    }

    @Override
    public void onProviderEnabled(String provider) {
        Log.d(TAG, "Provider enabled: " + provider);
    }

    @Override
    public void onProviderDisabled(String provider) {
        Log.d(TAG, "Provider disabled: " + provider);
    }

    // Native methods
    private native void nativeUpdateLocation(long hubPtr,
                                             double lat, double lon, double alt,
                                             float hAcc, float vAcc,
                                             float speed, float course);

    // Static methods called from native code
    public static void nativeStart(Context context, long hubPtr) {
        SpatialLocation loc = getInstance(context);
        loc.setNativeHub(hubPtr);
        loc.start();
    }

    public static void nativeStop(Context context) {
        SpatialLocation loc = getInstance(context);
        loc.stop();
    }

    public static boolean nativeHasPermission(Context context) {
        return getInstance(context).hasLocationPermission();
    }

    public static boolean nativeIsEnabled(Context context) {
        return getInstance(context).isLocationEnabled();
    }

    public static void nativeRequestPermission(Activity activity) {
        getInstance(activity).requestPermission(activity);
    }
}
