package org.libsdl.app;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

/**
 * HSBackground - Android background task implementation for HopStar
 * Uses Handler-based tasks for short operations
 * For long-running tasks, WorkManager would be used
 */
public class HSBackground {
    private static final String TAG = "HSBackground";

    private static Handler mainHandler;
    private static Map<String, Runnable> activeTasks = new HashMap<>();
    private static boolean initialized = false;

    // Native callbacks
    private static native void nativeOnTaskExpiring(String taskId);
    private static native void nativeOnWorkResult(String taskId, boolean success);

    public static void initialize() {
        if (initialized) return;

        mainHandler = new Handler(Looper.getMainLooper());
        initialized = true;
        Log.d(TAG, "Initialized");
    }

    public static String beginTask(String taskId, String reason, int maxSeconds) {
        if (!initialized) initialize();

        final String finalTaskId = (taskId == null || taskId.isEmpty())
                ? "task_" + UUID.randomUUID().toString().substring(0, 8)
                : taskId;

        // Schedule expiration
        Runnable expirationRunnable = new Runnable() {
            @Override
            public void run() {
                Log.d(TAG, "Task expiring: " + finalTaskId);
                nativeOnTaskExpiring(finalTaskId);
                activeTasks.remove(finalTaskId);
            }
        };

        activeTasks.put(finalTaskId, expirationRunnable);
        mainHandler.postDelayed(expirationRunnable, maxSeconds * 1000L);

        Log.d(TAG, "Begin task: " + finalTaskId + " (" + reason + "), max " + maxSeconds + "s");
        return finalTaskId;
    }

    public static void endTask(String taskId) {
        if (taskId == null) return;

        Runnable runnable = activeTasks.remove(taskId);
        if (runnable != null) {
            mainHandler.removeCallbacks(runnable);
            Log.d(TAG, "End task: " + taskId);
        }
    }

    public static void endAllTasks() {
        for (Runnable runnable : activeTasks.values()) {
            mainHandler.removeCallbacks(runnable);
        }
        activeTasks.clear();
        Log.d(TAG, "End all tasks");
    }

    public static void scheduleWork(String workId, int delayMinutes) {
        // For production, use WorkManager:
        // OneTimeWorkRequest workRequest = new OneTimeWorkRequest.Builder(HSSyncWorker.class)
        //     .setInitialDelay(delayMinutes, TimeUnit.MINUTES)
        //     .addTag(workId)
        //     .build();
        // WorkManager.getInstance(context).enqueue(workRequest);

        Log.d(TAG, "Schedule work: " + workId + " in " + delayMinutes + " minutes");

        // Simple implementation using Handler
        mainHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                Log.d(TAG, "Executing scheduled work: " + workId);
                nativeOnWorkResult(workId, true);
            }
        }, delayMinutes * 60 * 1000L);
    }

    public static void cancelWork(String workId) {
        // WorkManager.getInstance(context).cancelAllWorkByTag(workId);
        Log.d(TAG, "Cancel work: " + workId);
    }
}
