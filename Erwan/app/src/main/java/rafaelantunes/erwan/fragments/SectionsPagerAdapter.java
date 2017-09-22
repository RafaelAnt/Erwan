package rafaelantunes.erwan.fragments;

/**
 * Created by Rafael on 17/09/2017.
 * Erwan.
 * rafaelantunes.erwan.fragments.
 */

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.util.Log;
import android.util.SparseArray;
import android.view.ViewGroup;

import rafaelantunes.erwan.applications.GlobalVariables;

/**
 * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
 * one of the sections/tabs/pages.
 */
public class SectionsPagerAdapter extends FragmentPagerAdapter {

    private final static String TAG = "RafaDebugSectionPageApt";

    private SparseArray<Fragment> registeredFragments = new SparseArray<>();

    public SectionsPagerAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int position) {
        // getItem is called to instantiate the fragment for the given page.
        // Return a PlaceholderFragment (defined as a static inner class below).
        switch(position) {
            case 0:
                Log.d(TAG,"new Instance of Weather...");
                return WeatherFragment.newInstance(position+1);
            case 1:
                Log.d(TAG,"new Instance of Light...");
                LightFragment lightFragment = LightFragment.newInstance(position+1);

                return lightFragment;
            default:

                // Other fragments
        }
        return null;
    }

    @Override
    public int getCount() {
        return 2;
    }

    @Override
    public Object instantiateItem(ViewGroup container, int position) {
        /*switch (position){
            case (0): // Weather
                break;
            case(1): //Light
                super.instantiateItem(container, position);
                break;
        }*/
        Fragment fragment = (Fragment) super.instantiateItem(container, position);
        registeredFragments.put(position, fragment);
        return fragment;
    }

    @Override
    public void destroyItem(ViewGroup container, int position, Object object) {
        registeredFragments.remove(position);
        super.destroyItem(container, position, object);
    }

    public Fragment getRegisteredFragment(int position) {
        return registeredFragments.get(position);
    }

    @Override
    public CharSequence getPageTitle(int position) {
        switch (position) {
            case 0:
                return "Weather";
            case 1:
                return "Light";
        }
        return null;
    }
}
