package org.pytorch.helloworld;

import android.util.Log;

public class YOLOv5 {

    public static final String TAG = "YOLOv5";

    static {
        try {
            System.loadLibrary("torchvision_ops");
            System.loadLibrary("torch_test");
        } catch (Exception e) {
            Log.e(TAG, e.getMessage());
        }
    }

    public native void loadAndForward(final String filePath);
}
