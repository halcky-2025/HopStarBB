package org.libsdl.app;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;

import androidx.core.app.NotificationCompat;

/**
 * HSBackgroundService - Foreground service for long-running background tasks
 * Used by BackgroundNetHub for file sync, uploads, etc.
 */
public class HSBackgroundService extends Service {
    private static final String TAG = "HSBackgroundService";
    private static final String CHANNEL_ID = "hopstar_background";
    private static final int NOTIFICATION_ID = 9001;

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "Service created");
        createNotificationChannel();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG, "Service started");

        String title = "HopStar";
        String message = "Processing in background...";

        if (intent != null) {
            title = intent.getStringExtra("title");
            message = intent.getStringExtra("message");
            if (title == null) title = "HopStar";
            if (message == null) message = "Processing in background...";
        }

        Notification notification = createNotification(title, message);
        startForeground(NOTIFICATION_ID, notification);

        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "Service destroyed");
        super.onDestroy();
    }

    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationChannel channel = new NotificationChannel(
                    CHANNEL_ID,
                    "Background Tasks",
                    NotificationManager.IMPORTANCE_LOW
            );
            channel.setDescription("Notifications for background processing tasks");
            channel.setShowBadge(false);

            NotificationManager manager = getSystemService(NotificationManager.class);
            if (manager != null) {
                manager.createNotificationChannel(channel);
            }
        }
    }

    private Notification createNotification(String title, String message) {
        Intent notificationIntent = new Intent(this, SDLActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(
                this, 0, notificationIntent,
                PendingIntent.FLAG_IMMUTABLE
        );

        return new NotificationCompat.Builder(this, CHANNEL_ID)
                .setContentTitle(title)
                .setContentText(message)
                .setSmallIcon(android.R.drawable.ic_popup_sync)
                .setContentIntent(pendingIntent)
                .setOngoing(true)
                .setPriority(NotificationCompat.PRIORITY_LOW)
                .build();
    }

    // Static helper methods for C++ to start/stop service
    public static void startService(Context context, String title, String message) {
        Intent intent = new Intent(context, HSBackgroundService.class);
        intent.putExtra("title", title);
        intent.putExtra("message", message);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            context.startForegroundService(intent);
        } else {
            context.startService(intent);
        }
        Log.d(TAG, "Service start requested");
    }

    public static void stopService(Context context) {
        Intent intent = new Intent(context, HSBackgroundService.class);
        context.stopService(intent);
        Log.d(TAG, "Service stop requested");
    }

    public static void updateNotification(Context context, String title, String message) {
        NotificationManager manager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        if (manager != null) {
            NotificationCompat.Builder builder = new NotificationCompat.Builder(context, CHANNEL_ID)
                    .setContentTitle(title)
                    .setContentText(message)
                    .setSmallIcon(android.R.drawable.ic_popup_sync)
                    .setOngoing(true)
                    .setPriority(NotificationCompat.PRIORITY_LOW);

            manager.notify(NOTIFICATION_ID, builder.build());
        }
    }
}
