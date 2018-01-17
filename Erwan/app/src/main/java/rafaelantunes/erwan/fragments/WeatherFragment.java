package rafaelantunes.erwan.fragments;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Locale;

import rafaelantunes.erwan.R;
import rafaelantunes.erwan.activities.MainActivity;
import rafaelantunes.erwan.activities.WeatherActivity;
import rafaelantunes.erwan.applications.GlobalVariables;
import rafaelantunes.erwan.classes.Weather;

import static android.os.SystemClock.sleep;


/**
 * Created by Rafael on 17/09/2017.
 * Erwan.
 * rafaelantunes.erwan.fragments.
 */

public class WeatherFragment extends Fragment {


    private static final String TAG = "RafaDebugWeatherFrag";
    private static final String ARG_SECTION_NUMBER = "section_number";
    private GlobalVariables globalVariables;
    private Weather weather;

    private TextView tv_condition;
    private TextView tv_description;
    private TextView tv_temp;
    private TextView tv_rain;

    private ImageView im_temp;
    private ImageView im_rain;
    private ImageView im_about;

    private Button b_water;


    public WeatherFragment() {
    }

    /**
     * Returns a new instance of this fragment for the given section
     * number.
     */
    public static WeatherFragment newInstance(int sectionNumber) {
        WeatherFragment fragment = new WeatherFragment();
        Bundle args = new Bundle();
        args.putInt(ARG_SECTION_NUMBER, sectionNumber);
        fragment.setArguments(args);
        return fragment;

    }

    public void updateView(){
        if(globalVariables == null) {
            Log.e(TAG, "ERROR GLOBAL VARIABLES NULL");
            return ;
        }

        weather = globalVariables.getWeather();
        if(weather == null){
            Log.e(TAG, "Weather is null");
            //return null;
        }

        if(weather.currentCondition.getDescr() == null || weather.currentCondition.getCondition() == null){
            Log.e(TAG, "Weather is empty...");
            try {
                Toast.makeText(
                        getActivity().getApplicationContext(),
                        "Unable to Load. Check again later.",
                        Toast.LENGTH_LONG).show();
            }catch (Exception e){
                Log.e(TAG, "Exception creating a toast: " + e.getMessage());
                e.printStackTrace();
            }
        }else{
            Log.d(TAG, "Weather is not empty...");
            tv_condition.setText(weather.currentCondition.getCondition());

            // Set the first letter to Uppercase.
            String output = weather.currentCondition.getDescr().substring(0, 1).toUpperCase() +
                    weather.currentCondition.getDescr().substring(1);
            tv_description.setText(output);

            tv_temp.setText(String.format(
                    Locale.ENGLISH,"%d ºC",(int) weather.temperature.getTemp()));
            
            if(weather.rain.getAmmount() > 0){
                tv_rain.setText(R.string.rain);
                globalVariables.setWatered(true);
                b_water.setEnabled(false);
            }else{
                tv_rain.setText(R.string.no_rain);
                b_water.setEnabled(true);
            }

            if (weather.iconData != null && weather.iconData.length > 0) {
                Bitmap img = BitmapFactory.decodeByteArray(
                        weather.iconData, 0, weather.iconData.length);

                im_about.setImageBitmap(img);
            }else{
                Log.d(TAG, "Image could not be loaded.");
            }
        }
        if(globalVariables.isWatered()) tv_rain.setText(R.string.watered);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        Log.d(TAG, "Creating view!");
        globalVariables = (GlobalVariables) getActivity().getApplicationContext();

        View rootView = inflater.inflate(R.layout.weather_fragment_weather, container, false);
        tv_condition = (TextView) rootView.findViewById(R.id.tv_condition);
        tv_description = (TextView) rootView.findViewById(R.id.tv_description);
        tv_temp = (TextView) rootView.findViewById(R.id.tv_temp);
        tv_rain = (TextView) rootView.findViewById(R.id.tv_rain);

        this.im_temp = (ImageView) rootView.findViewById(R.id.im_temp);
        this.im_rain = (ImageView) rootView.findViewById(R.id.im_rain);
        this.im_about = (ImageView) rootView.findViewById(R.id.im_weather_icon);

        this.b_water = (Button) rootView.findViewById(R.id.b_water_the_plant);

        if(globalVariables.isWatered()) tv_rain.setText(R.string.watered);

        b_water.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d(TAG, "Watering the plant!");
                globalVariables.setWatered(true);
                b_water.setEnabled(false);
                b_water.setText(R.string.watered);
            }
        });

        if(globalVariables == null) {
            Log.e(TAG, "ERROR GLOBAL VARIABLES NULL");
        }

        NoConnectionTask task = new NoConnectionTask();
        task.execute();

        return rootView;
    }

    private class NoConnectionTask extends AsyncTask<String, Void, Weather> {

        @Override
        protected Weather doInBackground(String... strings) {
            sleep(5000);
            return null;
        }

        @Override
        protected void onPostExecute(Weather weather) {
            super.onPostExecute(weather);

            globalVariables = (GlobalVariables) getActivity().getApplicationContext();

            if(globalVariables.getWeather() == null){
                Log.e(TAG, "Weather is null");

                //Display error:
                AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(getActivity());
                AlertDialog alertDialog;
                // set dialog message
                alertDialogBuilder.setMessage("Weather could not be loaded.\nVerify your internet connection and your location settings.");

                // create alert dialog
                alertDialog = alertDialogBuilder.create();
                // show it
                alertDialog.show();
            }
        }
    }
}
