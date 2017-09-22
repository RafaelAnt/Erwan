package rafaelantunes.erwan.fragments;

import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import rafaelantunes.erwan.R;
import rafaelantunes.erwan.applications.GlobalVariables;

/**
 * Created by Rafael on 17/09/2017.
 * Erwan.
 * rafaelantunes.erwan.fragments.
 */

public class WeatherFragment extends Fragment {

    private static final String ARG_SECTION_NUMBER = "section_number";
    GlobalVariables globalVariables = (GlobalVariables) getContext();

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

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        View rootView = inflater.inflate(R.layout.weather_fragment_weather, container, false);


        return rootView;
    }
}
