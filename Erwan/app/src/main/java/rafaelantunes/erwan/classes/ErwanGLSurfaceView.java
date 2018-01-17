package rafaelantunes.erwan.classes;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;

/**
 * Created by Rafael on 30/09/2017.
 * Erwan.
 * rafaelantunes.erwan.classes.
 */

public class ErwanGLSurfaceView extends GLSurfaceView {

    private final ErwanGLRenderer erwanGLRenderer;

    public ErwanGLSurfaceView(Context context, AttributeSet attributeSet) {
        super(context, attributeSet);

        // Create an OpenGL ES 3.0 context
        setEGLContextClientVersion(3);
        erwanGLRenderer = new ErwanGLRenderer(context);

        // Set the Renderer for drawing on the GLSurfaceView
        setRenderer(erwanGLRenderer);
        // setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }


    @Override
    public boolean onTouchEvent(MotionEvent e) {
        // MotionEvent reports input details from the touch screen
        // and other input controls. In this case, we are only
        // interested in events where the touch position changed.

        float x = e.getX();
        float y = e.getY();

        switch (e.getAction()) {
            case MotionEvent.ACTION_DOWN:
                //erwanGLRenderer.startRotating(x,y);
                break;
            case MotionEvent.ACTION_UP:
                //erwanGLRenderer.stopRotating();
                break;
            case MotionEvent.ACTION_MOVE:
                //erwanGLRenderer.updateRotation(x,y);
                break;
        }
        return true;
    }
}
