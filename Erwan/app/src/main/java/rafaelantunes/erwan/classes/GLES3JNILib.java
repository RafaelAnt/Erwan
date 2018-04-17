package rafaelantunes.erwan.classes;

/**
 * Created by Rafael on 30/09/2017.
 * Erwan.
 * rafaelantunes.erwan.classes.
 */

import android.content.res.AssetManager;

public class GLES3JNILib {

    static {
        System.loadLibrary("main");
    }

    public static native void init(AssetManager c);
    /*public static native void resize(int width, int height);
    public static native void step();

    public static native void startRotating(float x, float y);
    public static native void updateRotation(float x, float y);
    public static native void stopRotating();

    public static native void startScaling(float dist);
    public static native void updateScale(float dist);
    public static native void stopScaling();*/

    public static native void sendPath(String path);

}
