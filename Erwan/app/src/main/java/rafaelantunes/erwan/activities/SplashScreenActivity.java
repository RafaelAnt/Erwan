package rafaelantunes.erwan.activities;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import rafaelantunes.erwan.R;

public class SplashScreenActivity extends Activity {

    private final String TAG = "ERWAN KATTEL";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_spalsh_screen);

        Log.d(TAG,"Sending Intent.");
        Intent intent = new Intent(this, LoginActivity.class );
        startActivity(intent);
    }
}
