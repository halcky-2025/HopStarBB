package org.libsdl.app;

import android.content.ClipData;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Build;
import android.provider.OpenableColumns;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;

import java.util.ArrayList;
import java.util.List;

/**
 * SDLActivity subclass that opts the app's surface out of the Android system
 * back-gesture in the left/right edge columns. Android 10+ reserves those
 * edges for the back-swipe gesture, which introduces ~10-50ms ambiguity
 * latency on edge taps. Our UI has an explicit in-app back/close affordance,
 * so we'd rather take the taps directly.
 *
 * Android constraint: each edge gets at most 200dp of cumulative exclusion.
 * We claim roughly the centre-vertical strip on each edge so the user still
 * has corner space to perform the OS back gesture if they want it.
 */
public class HopStarBBActivity extends SDLActivity {

    private static final String TAG = "HopStarBBActivity";

    /**
     * SAF (Storage Access Framework) で選択した URI に **永続的な read permission** を
     * 取得する。デフォルトの onActivityResult が走る前にこれを呼んでおくと、アプリ再起動後も
     * 同じ URI でファイルを開ける (= sqlite に保存したパスを次回起動で復元できる)。
     *
     * Android はアプリあたり最大 128 件まで permission を保持できる。それを超えると
     * 古いものから drop されるので、必要なら releasePersistableUriPermission も検討。
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.i(TAG, "onActivityResult: requestCode=" + requestCode + " resultCode=" + resultCode
                + " data=" + (data != null ? data.toString() : "null"));
        if (data != null) {
            int flags = Intent.FLAG_GRANT_READ_URI_PERMISSION;
            // 単一 URI
            Uri single = data.getData();
            if (single != null) {
                try {
                    getContentResolver().takePersistableUriPermission(single, flags);
                    Log.i(TAG, "takePersistableUriPermission OK: " + single);
                } catch (Throwable t) {
                    Log.w(TAG, "takePersistableUriPermission(single) failed: " + single, t);
                }
            }
            // 複数選択 (ClipData)
            ClipData clip = data.getClipData();
            if (clip != null) {
                for (int i = 0; i < clip.getItemCount(); i++) {
                    Uri u = clip.getItemAt(i).getUri();
                    if (u == null) continue;
                    try {
                        getContentResolver().takePersistableUriPermission(u, flags);
                        Log.i(TAG, "takePersistableUriPermission OK (multi): " + u);
                    } catch (Throwable t) {
                        Log.w(TAG, "takePersistableUriPermission(multi) failed: " + u, t);
                    }
                }
            }
        }
        // 後段は SDLActivity 側のデフォルト処理 (= onNativeFileDialog で URI を native に渡す)
        super.onActivityResult(requestCode, resultCode, data);
        Log.i(TAG, "onActivityResult: super done");
    }

    /**
     * Resolves a content:// or file:// URI string to its display filename
     * (e.g. "tone.xml"). Returns null if the URI can't be resolved.
     * Called from native code via JNI to get a user-friendly tab title
     * after SAF (Storage Access Framework) returns an opaque content URI.
     */
    public static String getDisplayNameForUri(String uriStr) {
        if (uriStr == null) return null;
        Context ctx = (mSingleton != null) ? mSingleton : null;
        if (ctx == null) return null;
        try {
            Uri uri = Uri.parse(uriStr);
            String scheme = uri.getScheme();
            if (scheme == null) return null;
            if (scheme.equals("file")) {
                String path = uri.getLastPathSegment();
                return path;
            }
            if (scheme.equals("content")) {
                try (Cursor cur = ctx.getContentResolver().query(uri,
                        new String[] { OpenableColumns.DISPLAY_NAME }, null, null, null)) {
                    if (cur != null && cur.moveToFirst()) {
                        int idx = cur.getColumnIndex(OpenableColumns.DISPLAY_NAME);
                        if (idx >= 0) {
                            String name = cur.getString(idx);
                            if (name != null && !name.isEmpty()) return name;
                        }
                    }
                } catch (Throwable t) {
                    Log.w(TAG, "getDisplayNameForUri: query failed", t);
                }
                // fallback: last path segment
                return uri.getLastPathSegment();
            }
        } catch (Throwable t) {
            Log.w(TAG, "getDisplayNameForUri failed", t);
        }
        return null;
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            applyGestureExclusion();
        }
    }

    private void applyGestureExclusion() {
        // setSystemGestureExclusionRects requires Android 10 (API 29) or higher.
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.Q) {
            return;
        }
        final SDLSurface surf = mSurface;
        if (surf == null) {
            return;
        }
        // Defer until the SurfaceView has measured dimensions. post() runs on
        // the UI thread once the current layout pass completes.
        surf.post(new Runnable() {
            @Override
            public void run() {
                int w = surf.getWidth();
                int h = surf.getHeight();
                if (w <= 0 || h <= 0) {
                    return;
                }

                DisplayMetrics dm = surf.getResources().getDisplayMetrics();
                int edgeStripPx = (int) (24f * dm.density);        // 24dp wide column
                int edgeHeightPx = Math.min(h, (int) (200f * dm.density));  // OS cap = 200dp
                int top = (h - edgeHeightPx) / 2;
                int bottom = top + edgeHeightPx;

                List<Rect> rects = new ArrayList<>();
                rects.add(new Rect(0, top, Math.min(edgeStripPx, w / 2), bottom));
                rects.add(new Rect(Math.max(0, w - edgeStripPx), top, w, bottom));
                try {
                    surf.setSystemGestureExclusionRects(rects);
                    Log.i(TAG, "setSystemGestureExclusionRects: w=" + w + " h=" + h
                            + " edge=" + edgeStripPx + "px height=" + edgeHeightPx + "px");
                } catch (Throwable t) {
                    Log.w(TAG, "setSystemGestureExclusionRects failed", t);
                }
            }
        });
        // Re-apply on any future layout change (rotation / fold / dock).
        surf.addOnLayoutChangeListener(new View.OnLayoutChangeListener() {
            @Override
            public void onLayoutChange(View v, int l, int t, int r, int b,
                                      int ol, int ot, int or_, int ob) {
                if ((r - l) != (or_ - ol) || (b - t) != (ob - ot)) {
                    applyGestureExclusion();
                }
            }
        });
    }
}
