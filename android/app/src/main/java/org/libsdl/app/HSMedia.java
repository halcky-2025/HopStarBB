package org.libsdl.app;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.media.AudioAttributes;
import android.media.AudioFocusRequest;
import android.media.AudioManager;
import android.os.Build;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

/**
 * HSMedia - Android media implementation for HopStar
 */
public class HSMedia {
    private static final String TAG = "HSMedia";
    private static final int MIC_PERMISSION_REQUEST = 3001;
    private static final int CAMERA_PERMISSION_REQUEST = 3002;

    private static AudioManager audioManager;
    private static AudioFocusRequest focusRequest;
    private static AudioManager.OnAudioFocusChangeListener focusListener;
    private static boolean isActive = false;
    private static int currentMode = 0;
    private static boolean useFrontCamera = true;

    // Native callbacks
    private static native void nativeOnMicPermission(boolean granted);
    private static native void nativeOnCameraPermission(boolean granted);
    private static native void nativeOnAudioFocusChange(int focusChange);

    public static void initialize(Context context) {
        audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        Log.d(TAG, "Initialized");
    }

    public static boolean hasMicPermission() {
        Context context = SDLActivity.getContext();
        return ContextCompat.checkSelfPermission(context,
                Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED;
    }

    public static boolean hasCameraPermission() {
        Context context = SDLActivity.getContext();
        return ContextCompat.checkSelfPermission(context,
                Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED;
    }

    public static void requestMicPermission() {
        Activity activity = (Activity) SDLActivity.getContext();
        if (!hasMicPermission()) {
            ActivityCompat.requestPermissions(activity,
                    new String[]{Manifest.permission.RECORD_AUDIO},
                    MIC_PERMISSION_REQUEST);
        } else {
            nativeOnMicPermission(true);
        }
    }

    public static void requestCameraPermission() {
        Activity activity = (Activity) SDLActivity.getContext();
        if (!hasCameraPermission()) {
            ActivityCompat.requestPermissions(activity,
                    new String[]{Manifest.permission.CAMERA},
                    CAMERA_PERMISSION_REQUEST);
        } else {
            nativeOnCameraPermission(true);
        }
    }

    public static void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        boolean granted = grantResults.length > 0 &&
                grantResults[0] == PackageManager.PERMISSION_GRANTED;

        if (requestCode == MIC_PERMISSION_REQUEST) {
            nativeOnMicPermission(granted);
        } else if (requestCode == CAMERA_PERMISSION_REQUEST) {
            nativeOnCameraPermission(granted);
        }
    }

    public static boolean start(int mode, boolean frontCamera) {
        if (isActive) {
            stop();
        }

        Context context = SDLActivity.getContext();
        if (audioManager == null) {
            initialize(context);
        }

        currentMode = mode;
        useFrontCamera = frontCamera;

        // Request audio focus
        focusListener = new AudioManager.OnAudioFocusChangeListener() {
            @Override
            public void onAudioFocusChange(int focusChange) {
                nativeOnAudioFocusChange(focusChange);
            }
        };

        int result;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            AudioAttributes attrs = new AudioAttributes.Builder()
                    .setUsage(mode == 1 || mode == 3 ? // VoiceCall or VideoCall
                            AudioAttributes.USAGE_VOICE_COMMUNICATION :
                            AudioAttributes.USAGE_MEDIA)
                    .setContentType(AudioAttributes.CONTENT_TYPE_SPEECH)
                    .build();

            focusRequest = new AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN)
                    .setAudioAttributes(attrs)
                    .setOnAudioFocusChangeListener(focusListener)
                    .build();

            result = audioManager.requestAudioFocus(focusRequest);
        } else {
            // Deprecated but required for API < 26
            @SuppressWarnings("deprecation")
            int legacyResult = audioManager.requestAudioFocus(focusListener,
                    AudioManager.STREAM_VOICE_CALL,
                    AudioManager.AUDIOFOCUS_GAIN);
            result = legacyResult;
        }

        if (result != AudioManager.AUDIOFOCUS_REQUEST_GRANTED) {
            Log.w(TAG, "Audio focus not granted");
        }

        // Set audio mode
        switch (mode) {
            case 1: // VoiceCall
            case 3: // VideoCall
                audioManager.setMode(AudioManager.MODE_IN_COMMUNICATION);
                break;
            case 2: // VoiceMemo
                audioManager.setMode(AudioManager.MODE_NORMAL);
                break;
            default:
                break;
        }

        isActive = true;
        Log.d(TAG, "Started mode " + mode);
        return true;
    }

    @SuppressWarnings("deprecation")
    public static void stop() {
        if (!isActive) return;

        if (audioManager != null) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O && focusRequest != null) {
                audioManager.abandonAudioFocusRequest(focusRequest);
            } else if (focusListener != null) {
                // Deprecated but required for API < 26
                audioManager.abandonAudioFocus(focusListener);
            }
            audioManager.setMode(AudioManager.MODE_NORMAL);
        }

        focusRequest = null;
        focusListener = null;
        isActive = false;
        Log.d(TAG, "Stopped");
    }

    @SuppressWarnings("deprecation")
    public static void setMicMuted(boolean muted) {
        if (audioManager != null) {
            // Deprecated in API 28+ but still functional for voice calls
            audioManager.setMicrophoneMute(muted);
            Log.d(TAG, "Mic muted: " + muted);
        }
    }

    @SuppressWarnings("deprecation")
    public static void setSpeaker(boolean enabled) {
        if (audioManager != null) {
            // Deprecated in API 31+ but no direct replacement for all use cases
            audioManager.setSpeakerphoneOn(enabled);
            Log.d(TAG, "Speaker: " + enabled);
        }
    }

    public static void switchCamera() {
        useFrontCamera = !useFrontCamera;
        Log.d(TAG, "Switch camera to " + (useFrontCamera ? "front" : "back"));
        // Camera2 implementation would handle actual camera switch
    }

    public static boolean isActive() {
        return isActive;
    }
}
