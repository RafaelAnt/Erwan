package rafaelantunes.erwan.activities;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.Window;

import rafaelantunes.erwan.R;


public class SplashScreenActivity extends Activity {

    private final String TAG = "ERWAN KATTEL";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_splash_screen);

        /*ProgressBar progressBar = (ProgressBar)findViewById(R.id.spin_kit);
        FadingCircle doubleBounce = new FadingCircle();
        progressBar.setIndeterminateDrawable(doubleBounce);*/

        Intent intent = getIntent();
        String action = intent.getAction();
        Uri data = intent.getData();

        start();

    }

    @Override
    protected void onResume() {
        super.onResume();
        //start();
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        //start();
    }

    private void start() {

        // TODO: 25/01/2017 check login here;

        //isLogedIn();
        //vamos passar o login a frente;
        boolean login = true;

        if (login) {
            Intent intent = new Intent(this, LoginActivity.class);
            startActivity(intent);
        } else {
            Intent intent = new Intent(this, MainActivity.class);
            startActivity(intent);
        }

    }



    @Override
    public void onStart() {
        super.onStart();
    }

    @Override
    public void onStop() {
        super.onStop();
    }

}
