package org.libsdl.app;

import android.Manifest;
import android.app.Activity;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;
import androidx.core.content.ContextCompat;

/**
 * HSNotification - Android notification implementation for HopStar
 */
public class HSNotification {
    private static final String TAG = "HSNotification";
    private static final int PERMISSION_REQUEST_CODE = 2001;

    // Notification channels
    private static final String CHANNEL_GENERAL = "general";
    private static final String CHANNEL_MESSAGES = "messages";
    private static final String CHANNEL_CALLS = "calls";
    private static final String CHANNEL_FILESHARE = "fileshare";

    private static boolean initialized = false;
    private static int notificationIdCounter = 1000;

    // Native callbacks
    private static native void nativeOnNotificationTapped(String id, String title, String body, String deepLink);
    private static native void nativeOnPushReceived(String eventId, String type);
    private static native void nativeOnPushToken(String token);
    private static native void nativeOnPermissionResult(boolean granted);

    public static void initialize(Context context) {
        if (initialized) return;

        createNotificationChannels(context);
        initialized = true;
        Log.d(TAG, "Initialized");
    }

    private static void createNotificationChannels(Context context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationManager nm = context.getSystemService(NotificationManager.class);

            // General channel
            NotificationChannel general = new NotificationChannel(
                    CHANNEL_GENERAL, "General", NotificationManager.IMPORTANCE_DEFAULT);
            general.setDescription("General notifications");
            nm.createNotificationChannel(general);

            // Messages channel
            NotificationChannel messages = new NotificationChannel(
                    CHANNEL_MESSAGES, "Messages", NotificationManager.IMPORTANCE_HIGH);
            messages.setDescription("Message notifications");
            nm.createNotificationChannel(messages);

            // Calls channel (high priority)
            NotificationChannel calls = new NotificationChannel(
                    CHANNEL_CALLS, "Calls", NotificationManager.IMPORTANCE_HIGH);
            calls.setDescription("Incoming call notifications");
            calls.enableVibration(true);
            nm.createNotificationChannel(calls);

            // File share channel
            NotificationChannel fileshare = new NotificationChannel(
                    CHANNEL_FILESHARE, "File Sharing", NotificationManager.IMPORTANCE_DEFAULT);
            fileshare.setDescription("File sharing notifications");
            nm.createNotificationChannel(fileshare);

            Log.d(TAG, "Notification channels created");
        }
    }

    public static boolean hasPermission() {
        Context context = SDLActivity.getContext();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            return ContextCompat.checkSelfPermission(context,
                    Manifest.permission.POST_NOTIFICATIONS) == PackageManager.PERMISSION_GRANTED;
        }
        return true; // Permission not required before Android 13
    }

    public static void requestPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            Activity activity = (Activity) SDLActivity.getContext();
            if (!hasPermission()) {
                ActivityCompat.requestPermissions(activity,
                        new String[]{Manifest.permission.POST_NOTIFICATIONS},
                        PERMISSION_REQUEST_CODE);
            } else {
                nativeOnPermissionResult(true);
            }
        } else {
            nativeOnPermissionResult(true);
        }
    }

    public static void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode == PERMISSION_REQUEST_CODE) {
            boolean granted = grantResults.length > 0 &&
                    grantResults[0] == PackageManager.PERMISSION_GRANTED;
            nativeOnPermissionResult(granted);
        }
    }

    public static void postLocal(String id, String title, String body,
                                  String deepLink, String channel, int priority) {
        Context context = SDLActivity.getContext();
        if (context == null) return;

        initialize(context);

        if (!hasPermission()) {
            Log.w(TAG, "No notification permission");
            return;
        }

        String channelId = CHANNEL_GENERAL;
        switch (channel) {
            case "messages": channelId = CHANNEL_MESSAGES; break;
            case "calls": channelId = CHANNEL_CALLS; break;
            case "fileshare": channelId = CHANNEL_FILESHARE; break;
        }

        // Create intent for tap action
        Intent intent = new Intent(context, SDLActivity.class);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
        intent.putExtra("notification_id", id);
        intent.putExtra("notification_title", title);
        intent.putExtra("notification_body", body);
        intent.putExtra("notification_deepLink", deepLink);

        int notificationId = id.hashCode();
        PendingIntent pendingIntent = PendingIntent.getActivity(context, notificationId,
                intent, PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE);

        NotificationCompat.Builder builder = new NotificationCompat.Builder(context, channelId)
                .setSmallIcon(android.R.drawable.ic_dialog_info) // TODO: Use app icon
                .setContentTitle(title)
                .setContentText(body)
                .setAutoCancel(true)
                .setContentIntent(pendingIntent);

        // Set priority
        switch (priority) {
            case 0: builder.setPriority(NotificationCompat.PRIORITY_LOW); break;
            case 1: builder.setPriority(NotificationCompat.PRIORITY_DEFAULT); break;
            case 2: builder.setPriority(NotificationCompat.PRIORITY_HIGH); break;
            case 3: builder.setPriority(NotificationCompat.PRIORITY_MAX); break;
        }

        NotificationManagerCompat nm = NotificationManagerCompat.from(context);
        try {
            nm.notify(notificationId, builder.build());
            Log.d(TAG, "Posted notification: " + title);
        } catch (SecurityException e) {
            Log.e(TAG, "SecurityException posting notification", e);
        }
    }

    public static void cancelLocal(String id) {
        Context context = SDLActivity.getContext();
        if (context == null) return;

        NotificationManagerCompat nm = NotificationManagerCompat.from(context);
        nm.cancel(id.hashCode());
    }

    public static void cancelAllLocal() {
        Context context = SDLActivity.getContext();
        if (context == null) return;

        NotificationManagerCompat nm = NotificationManagerCompat.from(context);
        nm.cancelAll();
    }

    // Called when activity receives intent from notification tap
    public static void handleIntent(Intent intent) {
        if (intent == null) return;

        String id = intent.getStringExtra("notification_id");
        if (id != null) {
            String title = intent.getStringExtra("notification_title");
            String body = intent.getStringExtra("notification_body");
            String deepLink = intent.getStringExtra("notification_deepLink");

            Log.d(TAG, "Notification tapped: " + id);
            nativeOnNotificationTapped(id, title, body, deepLink);
        }
    }

    // Called from FCM service
    public static void onMessageReceived(String eventId, String type) {
        Log.d(TAG, "Push received: " + eventId + " (" + type + ")");
        nativeOnPushReceived(eventId, type);
    }

    // Called from FCM service
    public static void onNewToken(String token) {
        Log.d(TAG, "New FCM token");
        nativeOnPushToken(token);
    }
}
