package rafaelantunes.erwan.fragments;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.Locale;

import rafaelantunes.erwan.R;
import rafaelantunes.erwan.applications.GlobalVariables;

/**
 * Created by Rafael on 19/09/2017.
 * Erwan.
 * rafaelantunes.erwan.fragments.
 */

public class LightFragment extends Fragment {
    /**
     * The fragment argument representing the section number for this
     * fragment.
     */
    private static final String TAG = "RafaDebugLightFragment";

    private GlobalVariables globalVariables;
    private TextView tv_light_current;
    private TextView tv_light_total;
    private TextView tv_light_objective;
    private ImageView im_light_bulb;


    private static final String ARG_SECTION_NUMBER = "section_numberF";

    public LightFragment() {
    }

    public void updateView(){
        if(globalVariables == null) {
            Log.e(TAG, "ERROR GLOBAL VARIABLES NULL");
            return ;
        }

        tv_light_current.setText(String.format(Locale.ENGLISH,"%d",
                (int) globalVariables.getLight_current()));
        tv_light_total.setText(String.format(Locale.ENGLISH,"%d",
                (int)globalVariables.getLight_total()));
        tv_light_objective.setText(String.format(Locale.ENGLISH,"%d",
                (int)globalVariables.getLight_objective()));

        if(globalVariables.getLight_current()>100){
            im_light_bulb.setImageResource(R.drawable.light_on_black);
        }else{
            im_light_bulb.setImageResource(R.drawable.light_off_black);
        }


    }

    /**
     * Returns a new instance of this fragment for the given section
     * number.
     */
    public static LightFragment newInstance(int sectionNumber) {

        LightFragment fragment = new LightFragment();
        Bundle args = new Bundle();
        args.putInt(ARG_SECTION_NUMBER, sectionNumber);
        fragment.setArguments(args);
        Log.d(TAG,"LightInstance Finished...");
        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        Log.d(TAG,"\t\"OnCreateView\" Started...");
        globalVariables = (GlobalVariables) getActivity().getApplicationContext();

        View rootView = inflater.inflate(R.layout.weather_fragment_light, container, false);
        this.tv_light_current = (TextView) rootView.findViewById(R.id.tv_light_points_sec);
        this.tv_light_total = (TextView) rootView.findViewById(R.id.tv_light_points_total);
        this.tv_light_objective = (TextView) rootView.findViewById(R.id.tv_light_points_objective);
        this.im_light_bulb = (ImageView) rootView.findViewById(R.id.im_light_bulb);

        Log.d(TAG,"\tUpdating View...");
        updateView();
        Log.d(TAG,"\t\"OnCreateView\" Finished...");
        return rootView;
    }

}
