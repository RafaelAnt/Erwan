package rafaelantunes.erwan.activities;

import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.net.Uri;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import rafaelantunes.erwan.R;
import rafaelantunes.erwan.applications.GlobalVariables;
import rafaelantunes.erwan.classes.GLES3JNILib;

public class MainActivity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener{

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("main");
        //System.loadLibrary("assimp");
    }

    private final String TAG = "RafaDebugMain";

    private FirebaseUser firebaseUser;

    private  GlobalVariables globalVariables;

    // UI
    private TextView mEmailView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);

        Log.d(TAG,"Loading Global Variables...");

        globalVariables = ((GlobalVariables)getApplicationContext());

        Log.d(TAG,"Loading Toolbar...");

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setTitle("Erwan");

        Log.d(TAG,"Loading Drawer...");

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);

        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawer, toolbar,  R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
        //View header = getLayoutInflater().inflate(R.layout.main_drawer_header, null);
        //navigationView.addHeaderView(header);

        // UI
        //mEmailView = (TextView) findViewById(R.id.tv_show_mail);

        firebaseUser = FirebaseAuth.getInstance().getCurrentUser();
        if (firebaseUser != null) {
            // Name, email address, and profile photo Url
            String name = firebaseUser.getDisplayName();
            String email = firebaseUser.getEmail();
            //mEmailView.setText(email);
            Uri photoUrl = firebaseUser.getPhotoUrl();

            // The user's ID, unique to the Firebase project. Do NOT use this value to
            // authenticate with your backend server, if you have one. Use
            // FirebaseUser.getToken() instead.
            String uid = firebaseUser.getUid();
           // firebaseUser.

        }

        setGrammar();

    }

    private boolean setGrammar(){

        Log.d(TAG, "Copying files to internal storage.");

        AssetManager assetManager = getAssets();
        String[] files = null;
        try {
            files = assetManager.list("");
        } catch (IOException e) {
            Log.e("tag", "Failed to get asset file list.", e);
        }

        for(String filename : files) {
            //Log.d(TAG, "Found file: \""+filename+"\"");

            //skip other files
            if(!filename.equals("grammar.txt")){
                continue;
            }

            InputStream in = null;
            OutputStream out = null;
            try {
                in = assetManager.open(filename);

                //String outDir = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Erwan" ;


                //Log.d(TAG, "Out Dir: \""+outDir+"\"");
                //File erwanDirectory = new File(Environment.getExternalStorageDirectory().getAbsolutePath());
                //creating folder if i does not exist.
                //erwanDirectory.mkdirs();

                //erwanDirectory = new File(outDir);
                //creating folder if i does not exist.
                //erwanDirectory.mkdirs();


                //File outFile = new File(outDir, filename);

                //out = new FileOutputStream(outFile);
                out = openFileOutput(filename, Context.MODE_PRIVATE);

                GLES3JNILib.sendPath(getFilesDir().getAbsolutePath());

                copyFile(in, out);
                in.close();
                in = null;
                out.flush();
                out.close();
                out = null;
            } catch(IOException e) {
                Log.e(TAG, "Failed to copy asset file: " + filename, e);
            }
        }


        return true;
    }

    private void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
        Log.d(TAG,"Selecting on Navigation");
        Intent intent;

        // Handle navigation view item clicks here.
        int id = item.getItemId();

        switch (id) {
            case (R.id.drawer_invite_friends):
                break;
            case (R.id.drawer_language):
                break;
            case (R.id.drawer_notifications):
                break;
            case (R.id.drawer_settings):
                intent = new Intent(this, SettingsActivity.class);
                startActivity(intent);
                break;
            case (R.id.drawer_help):
                break;
            case (R.id.drawer_about):
                break;

            default:

        }

        // TODO: 04/02/2017
        // User chose the "Settings" item, show the app settings UI...
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(MainActivity.this);
        AlertDialog alertDialog;
        // set dialog message
        alertDialogBuilder.setMessage("Feature is still in development...");

        // create alert dialog
        alertDialog = alertDialogBuilder.create();
        // show it
        alertDialog.show();

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return false;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        Intent intent;
        //noinspection SimplifiableIfStatement
        switch (id) {

            case R.id.toolbar_settings:
                Log.d(TAG,"Loading Settings Activity...");
                // User chose the "Settings" item, show the app settings UI...
                intent = new Intent(this, SettingsActivity.class);
                startActivity(intent);
                break;

            case R.id.toolbar_friends:
                Log.d(TAG,"Loading Friends Activity...");
                intent = new Intent(this, FriendsActivity.class);
                startActivity(intent);
                break;

            case R.id.toolbar_weather:
                Log.d(TAG,"Loading Weather Activity...");
                intent = new Intent(this, WeatherActivity.class);
                startActivity(intent);
                break;

            default:
                // If we got here, the user's action was not recognized.
                // Invoke the superclass to handle it.
                return super.onOptionsItemSelected(item);
        }


        return true;
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main_toolbar, menu);
        return true;
    }

/*private void sendEmailVerification() {
        // Disable button
        //findViewById(R.id.verify_email_button).setEnabled(false);

        // Send verification email
        // [START send_email_verification]
        final FirebaseUser user = mAuth.getCurrentUser();
        user.sendEmailVerification()
                .addOnCompleteListener(this, new OnCompleteListener<Void>() {
                    @Override
                    public void onComplete(@NonNull Task<Void> task) {
                        // [START_EXCLUDE]
                        // Re-enable button
                        //findViewById(R.id.verify_email_button).setEnabled(true);

                        if (task.isSuccessful()) {
                            Toast.makeText(LoginRegisterActivity.this,
                                    "Verification email sent to " + user.getEmail(),
                                    Toast.LENGTH_SHORT).show();
                        } else {
                            Log.e(TAG, "sendEmailVerification", task.getException());
                            Toast.makeText(LoginRegisterActivity.this,
                                    "Failed to send verification email.",
                                    Toast.LENGTH_SHORT).show();
                        }
                        // [END_EXCLUDE]
                    }
                });
        // [END send_email_verification]
    }*/
}
