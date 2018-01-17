package rafaelantunes.erwan.activities;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.Address;
import android.location.Geocoder;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Chronometer;
import android.widget.Toast;

import org.json.JSONException;

import java.io.IOException;
import java.util.List;
import java.util.Locale;

import rafaelantunes.erwan.R;
import rafaelantunes.erwan.applications.GlobalVariables;
import rafaelantunes.erwan.classes.JSONWeatherParser;
import rafaelantunes.erwan.classes.Weather;
import rafaelantunes.erwan.classes.WeatherHttpClient;
import rafaelantunes.erwan.fragments.LightFragment;
import rafaelantunes.erwan.fragments.SectionsPagerAdapter;
import rafaelantunes.erwan.fragments.WeatherFragment;


public class WeatherActivity extends AppCompatActivity implements SensorEventListener {

    private String TAG = "RafaDebugWeather";
    private GlobalVariables globalVariables;
    private final int MY_PERMISSIONS_REQUEST_MULTIPLE_LOCATION = 600;

    // Sensors
    private SensorManager mSensorManager;
    private Sensor mLight, mAccelerometer;
    private LocationManager locationManager;
    private LocationListener locationListenerGPS, locationListenerNet;
    private Location locationGPS;
    private Location locationNet;

    //time
    private long startTimeLight;
    private long endTimeLight;
    private long startTimeAccelerometer;
    private long endTimeAccelerometer;
    private long elapsedTimeLight;
    private long elapsedTimeAccelerometer;

    private SectionsPagerAdapter mSectionsPagerAdapter;
    private ViewPager mViewPager;

    private boolean weatherDone = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.weather_activity);
        globalVariables = (GlobalVariables) getApplicationContext();

        Log.d(TAG,"Loading Toolbar...");
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        try {
            getSupportActionBar().setTitle("Growth");
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        }catch(Exception e){
            Log.e(TAG,"ERROR IN ACTION BAR");
            e.printStackTrace();
        }
        // Create the adapter that will return a fragment for each of the three
        // primary sections of the activity.
        Log.d(TAG,"Loading PagerAdapter...");
        mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

        // Set up the ViewPager with the sections adapter.
        Log.d(TAG,"Setting up the ViewPager...");
        mViewPager = (ViewPager) findViewById(R.id.container);
        mViewPager.setAdapter(mSectionsPagerAdapter);


        /*FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });*/

        Log.d(TAG,"Setting up the Sensors...");
        // Get an instance of the sensor service, and use that to get an instance of
        // a particular sensor.
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mLight = mSensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);


        // Location
        locationGPS = null;
        locationManager = (LocationManager) this.getSystemService(Context.LOCATION_SERVICE);
        locationListenerGPS = new LocationListener() {
            @Override
            public void onLocationChanged(Location location) {
                if(location != null){
                    locationGPS = location;
                    globalVariables.setLatitude(location.getLatitude());
                    globalVariables.setLongitude(location.getLongitude());
                    globalVariables.setAltitude(location.getAltitude());
                    if(!weatherDone) getCity();
                }else{
                    Log.e(TAG, "Location GPS came NULL.");
                }
            }

            @Override
            public void onStatusChanged(String provider, int status, Bundle extras) {

            }

            @Override
            public void onProviderEnabled(String provider) {

            }

            @Override
            public void onProviderDisabled(String provider) {

            }
        };

        locationNet = null;
        locationListenerNet = new LocationListener() {
            @Override
            public void onLocationChanged(Location location) {
                if(location != null){
                    locationNet = location;
                    globalVariables.setLatitudeNet(location.getLatitude());
                    globalVariables.setLongitudeNet(location.getLongitude());
                    globalVariables.setAltitudeNet(location.getAltitude());
                    if(!weatherDone) getCity();
                }else{
                    Log.e(TAG, "Location NET came NULL.");
                }
            }

            @Override
            public void onStatusChanged(String provider, int status, Bundle extras) {

            }

            @Override
            public void onProviderEnabled(String provider) {

            }

            @Override
            public void onProviderDisabled(String provider) {

            }
        };

        startTimeLight = System.currentTimeMillis();
        startTimeAccelerometer = System.currentTimeMillis();

        ActivityCompat.requestPermissions(this, new String[]{
                Manifest.permission.ACCESS_FINE_LOCATION,
                Manifest.permission.ACCESS_COARSE_LOCATION},
                MY_PERMISSIONS_REQUEST_MULTIPLE_LOCATION);

        Log.d(TAG,"Finished \"OnCreate\"");
    }

    @Override
    public final void onAccuracyChanged(Sensor sensor, int accuracy) {
        // Do something here if sensor accuracy changes.
        switch (sensor.getType()) {
            case (Sensor.TYPE_LIGHT):
                globalVariables.setAccuracy_light(accuracy);
                break;
            case (Sensor.TYPE_ACCELEROMETER):
                globalVariables.setAccuracy_accelerometer(accuracy);
                break;
            default:
        }

    }

    @Override
    public final void onSensorChanged(SensorEvent event) {

        switch (event.sensor.getType()) {
            case (Sensor.TYPE_LIGHT):
                endTimeLight =  System.currentTimeMillis();
                elapsedTimeLight = endTimeLight - startTimeLight;
                if(elapsedTimeLight < 1000){
                    break;
                }
                startTimeLight = System.currentTimeMillis();

                //Log.d(TAG,"Light Sensor changed");

                float lux = event.values[0];
                LightFragment lfrag =
                        (LightFragment) mSectionsPagerAdapter.getRegisteredFragment(1);
                globalVariables.setLight_current(lux);
                if(lux>100){
                    globalVariables.increaseLightTotalPoints(lux);
                }

                if(globalVariables.getLight_total()>=globalVariables.getLight_objective()){
                    Toast.makeText(this,
                            "You have reached the objective for today!",
                            Toast.LENGTH_LONG).show();
                }


                lfrag.updateView();
                //Log.d(TAG,"Finished");
                break;
            case (Sensor.TYPE_ACCELEROMETER):
                endTimeAccelerometer =  System.currentTimeMillis();
                elapsedTimeAccelerometer = endTimeAccelerometer - startTimeAccelerometer;
                if(elapsedTimeAccelerometer < 1000){
                    break;
                }
                startTimeAccelerometer = System.currentTimeMillis();


                //Log.d(TAG,"Accelerometer Sensor changed");




                globalVariables.setAccelerometer(event.values[0], event.values[1], event.values[2]);
                //TODO: UPDATE THE VIEW
                //Log.d(TAG,"Finished");
                break;
            default:
        }

    }

    @Override
    public void onRequestPermissionsResult(
            int requestCode, String permissions[], int[] grantResults) {

        switch (requestCode) {
            case (MY_PERMISSIONS_REQUEST_MULTIPLE_LOCATION):
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0 &&
                        grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // permission was granted
                    Log.d(TAG, "Permission Granted!");
                    try {
                        locationManager.requestLocationUpdates(
                                LocationManager.GPS_PROVIDER, 0, 0, locationListenerGPS);
                        locationManager.requestLocationUpdates(
                                LocationManager.NETWORK_PROVIDER, 0, 0, locationListenerNet);

                    } catch (SecurityException e) {
                        Toast.makeText(getApplicationContext(),
                                "Location Access Denied", Toast.LENGTH_SHORT).show();
                        Log.d(TAG,"Permission Denied!");
                    }

                } else {// permission denied
                    Log.d(TAG,"Permission Denied!");
                }
            default:
        }
    }



    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.weather_toolbar, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        switch (id){
            case(R.id.action_settings):
                break;
            default:
                // If we got here, the user's action was not recognized.
                // Invoke the superclass to handle it.
                return super.onOptionsItemSelected(item);
        }

        return super.onOptionsItemSelected(item);
    }

    public void getCity (){

        //AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(MainActivity.this);
        //AlertDialog alertDialog;

        Log.d(TAG,"Getting City...");

        String city;
        String countryCode;

        if(locationGPS == null){
            if(locationNet == null){
                Log.e(TAG, "Location not available...");
                Toast.makeText(this, "Location not available. Turn on your GPS.",
                        Toast.LENGTH_LONG).show();
                return;
            }else{
                globalVariables.setUseLocationNet(true);
            }
        }else{
            globalVariables.setUseLocationNet(false);
        }

        Geocoder gcd = new Geocoder(WeatherActivity.this, Locale.ENGLISH);
        List<Address> addresses = null;
        try {
            if(!globalVariables.isUseLocationNet()){
                Log.d(TAG,locationGPS.toString());
                addresses = gcd.getFromLocation(locationGPS.getLatitude(),
                        locationGPS.getLongitude(), 1);
            }else{
                Log.d(TAG,locationNet.toString());
                addresses = gcd.getFromLocation(locationNet.getLatitude(),
                        locationNet.getLongitude(), 1);
            }

        } catch (IOException e) {
            Log.e(TAG,"error noGetFromLocation");
            e.printStackTrace();
        }
        if (addresses != null && addresses.size() > 0){
            city = addresses.get(0).getSubAdminArea();
            countryCode = addresses.get(0).getCountryCode();
            Log.d(TAG,"The address is: " + addresses.get(0));


            JSONWeatherTask task = new JSONWeatherTask();
            task.execute(city+","+countryCode);

            weatherDone = true;
        }else{
            Log.e(TAG,"Location not found...");
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener(this, mLight, SensorManager.SENSOR_DELAY_NORMAL);
        mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mSensorManager.unregisterListener(this);
    }


    private class JSONWeatherTask extends AsyncTask<String, Void, Weather> {
        public JSONWeatherTask asyncObject;   // as CountDownTimer has similar method -> to prevent shadowing

        @Override
        protected void onPreExecute() {
            asyncObject = this;
            Log.d(TAG, "Creating Timeout.");
            new CountDownTimer(7000, 7000) {
                public void onTick(long millisUntilFinished) {
                    // You can monitor the progress here as well by changing the onTick() time
                }

                public void onFinish() {
                    // stop async task if not in progress
                    if (asyncObject.getStatus() == AsyncTask.Status.RUNNING) {
                        Log.d(TAG, "Timeout on Weather information.");
                        asyncObject.cancel(false);
                        // Add any specific task you wish to do as your extended class variable works here as well.
                    }
                }
            }.start();
        }

        @Override
        protected Weather doInBackground(String... params) {

            Weather weather = new Weather();
            String data = ( (new WeatherHttpClient()).getWeatherData(params[0]));

            Log.d(TAG, "Weather Information Collected.");

            try {
                weather = JSONWeatherParser.getWeather(data);

                // Let's retrieve the icon
                weather.iconData = (new WeatherHttpClient()).getImage(weather.currentCondition.getIcon());

            } catch (JSONException e) {
                Log.e(TAG, "Error loading image.");
                e.printStackTrace();
            }
            return weather;

        }


        @Override
        protected void onPostExecute(Weather weather) {
            Log.d(TAG, "Finished async task.");
            super.onPostExecute(weather);


            if(weather == null){
                Log.e(TAG, "Weather is null");

                //Display error:
                AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(WeatherActivity.this);
                AlertDialog alertDialog;
                // set dialog message
                alertDialogBuilder.setMessage("Weather could not be loaded.\nVerify your internet connection and your location settings.");

                // create alert dialog
                alertDialog = alertDialogBuilder.create();
                // show it
                alertDialog.show();
            }

            globalVariables.setWeather(weather);

            WeatherFragment wfrag = (WeatherFragment) mSectionsPagerAdapter.getRegisteredFragment(0);
            wfrag.updateView();
        }
    }
}
