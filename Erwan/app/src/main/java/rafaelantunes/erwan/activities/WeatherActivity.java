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
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
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

    //time
    private long startTimeLight;
    private long endTimeLight;
    private long startTimeAccelerometer;
    private long endTimeAccelerometer;
    private long elapsedTimeLight;
    private long elapsedTimeAccelerometer;

    /**
     * The {@link android.support.v4.view.PagerAdapter} that will provide
     * fragments for each of the sections. We use a
     * {@link FragmentPagerAdapter} derivative, which will keep every
     * loaded fragment in memory. If this becomes too memory intensive, it
     * may be best to switch to a
     * {@link android.support.v4.app.FragmentStatePagerAdapter}.
     */
    private SectionsPagerAdapter mSectionsPagerAdapter;

    /**
     * The {@link ViewPager} that will host the section contents.
     */
    private ViewPager mViewPager;

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
                if(locationGPS == null) locationGPS = location;
                globalVariables.setLatitude(location.getLatitude());
                globalVariables.setLongitude(location.getLongitude());
                globalVariables.setAltitude(location.getAltitude());
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

        locationListenerNet = new LocationListener() {
            @Override
            public void onLocationChanged(Location location) {
                if(locationGPS == null) locationGPS = location;
                globalVariables.setLatitudeNet(location.getLatitude());
                globalVariables.setLongitudeNet(location.getLongitude());
                globalVariables.setAltitudeNet(location.getAltitude());
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

        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION}, MY_PERMISSIONS_REQUEST_MULTIPLE_LOCATION);
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

                Log.d(TAG,"Light Sensor changed");

                float lux = event.values[0];
                LightFragment lfrag =
                        (LightFragment) mSectionsPagerAdapter.getRegisteredFragment(1);
                globalVariables.setLight_current(lux);
                if(lux>100){
                    globalVariables.increaseLightTotalPoints(lux);
                }

                if(globalVariables.getLight_total()>=globalVariables.getLight_objective()){
                    Toast.makeText(this, "You have reached the objective for today!",
                            Toast.LENGTH_LONG).show();
                }


                lfrag.updateView();
                Log.d(TAG,"Finished");
                break;
            case (Sensor.TYPE_ACCELEROMETER):
                endTimeAccelerometer =  System.currentTimeMillis();
                elapsedTimeAccelerometer = endTimeAccelerometer - startTimeAccelerometer;
                if(elapsedTimeAccelerometer < 1000){
                    break;
                }
                startTimeAccelerometer = System.currentTimeMillis();


                Log.d(TAG,"Accelerometer Sensor changed");
                globalVariables.setAccelerometer(event.values[0], event.values[1], event.values[2]);
                //TODO: UPDATE THE VIEW
                //Log.d(TAG,"Finished");
                break;
            default:
        }

    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch (requestCode) {
            case (MY_PERMISSIONS_REQUEST_MULTIPLE_LOCATION):
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) { // permission was granted
                    Log.d(TAG, "Permission Granted!");
                    try {
                        locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListenerGPS);
                        locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 0, 0, locationListenerNet);
                    } catch (SecurityException e) {
                        Toast.makeText(getApplicationContext(), "Location Access Denied", Toast.LENGTH_SHORT).show();
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

    public void getCity (View view){
        //AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(MainActivity.this);
        //AlertDialog alertDialog;

        String city;
        String countryCode;

        if(locationGPS == null){
            Log.e(TAG, "Location not available...");
            // set dialog message
            //alertDialogBuilder.setMessage();
            // create alert dialog
            //alertDialog = alertDialogBuilder.create();
            // show it
            //alertDialog.show();
            return;
        }

        Geocoder gcd = new Geocoder(WeatherActivity.this, Locale.ENGLISH);
        List<Address> addresses = null;
        try {
            Log.d("location",locationGPS.toString());
            addresses = gcd.getFromLocation(locationGPS.getLatitude(), locationGPS.getLongitude(), 1);
        } catch (IOException e) {
            Log.e(TAG,"error noGetFromLocation");
            e.printStackTrace();
        }
        if (addresses != null && addresses.size() > 0){
            //System.out.println();
            // set dialog message
            city = addresses.get(0).getSubAdminArea();
            countryCode=addresses.get(0).getCountryCode();
            //alertDialogBuilder.setMessage("Location: " + city + ", " + countryCode);
            Log.d(TAG,"The address is: " + addresses.get(0));

            // create alert dialog
            //alertDialog = alertDialogBuilder.create();
            // show it
            //alertDialog.show();

            JSONWeatherTask task = new JSONWeatherTask();
            task.execute(new String[]{city+","+countryCode});


        }else{
            Log.e(TAG,"Location not found...");
            // set dialog message
            //alertDialogBuilder.setMessage("Location not found...");
            // create alert dialog
            //alertDialog = alertDialogBuilder.create();
            // show it
            //alertDialog.show();
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

        @Override
        protected Weather doInBackground(String... params) {

            Weather weather = new Weather();
            String data = ( (new WeatherHttpClient()).getWeatherData(params[0]));

            try {
                weather = JSONWeatherParser.getWeather(data);

                // Let's retrieve the icon
                //weather.iconData = ( (new WeatherHttpClient()).getImage(weather.currentCondition.getIcon()));

            } catch (JSONException e) {
                e.printStackTrace();
            }
            return weather;

        }

        @Override
        protected void onPostExecute(Weather weather) {
            super.onPostExecute(weather);

            globalVariables.setWeather(weather);

            if (weather.iconData != null && weather.iconData.length > 0) {
                Bitmap img = BitmapFactory.decodeByteArray(weather.iconData, 0, weather.iconData.length);

                //imgView.setImageBitmap(img);
            }

            //Log.d("Result",""+weather.currentCondition.getCondition());
            //tv_cidade.setText(weather.location.getCity() + "," + weather.location.getCountry());
            //tv_weather.setText(weather.currentCondition.getCondition() + "(" + weather.currentCondition.getDescr() + ")");
            /*temp.setText("" + Math.round((weather.temperature.getTemp() - 273.15)) + "�C");
            hum.setText("" + weather.currentCondition.getHumidity() + "%");
            press.setText("" + weather.currentCondition.getPressure() + " hPa");
            windSpeed.setText("" + weather.wind.getSpeed() + " mps");
            windDeg.setText("" + weather.wind.getDeg() + "�");*/

        }
    }






}
