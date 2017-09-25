package rafaelantunes.erwan.applications;

import android.app.Application;

import rafaelantunes.erwan.activities.WeatherActivity;
import rafaelantunes.erwan.classes.Weather;
import rafaelantunes.erwan.fragments.LightFragment;
import rafaelantunes.erwan.fragments.WeatherFragment;

/**
 * Created by Rafael on 17/09/2017.
 * Erwan.
 * ${PACKAGE_NAME}.
 */

public class GlobalVariables extends Application{

    private final float light_objective = 2000;
    private float light_current;
    private float light_total;

    private float accelerometerX;
    private float accelerometerY;
    private float accelerometerZ;

    private double latitude;
    private double longitude;
    private double altitude;
    private double latitudeNet;
    private double longitudeNet;
    private double altitudeNet;
    private Weather weather;

    private boolean useLocationNet;

    //sensors accuracy
    private int accuracy_light;
    private int accuracy_accelerometer;

    //fragments
    private LightFragment currentLightFragment;
    private WeatherFragment currentWeatherFragment;

    //game
    private boolean watered;

    public boolean isWatered() {
        return watered;
    }

    public void setWatered(boolean watered) {
        this.watered = watered;
    }

    public boolean isUseLocationNet() {
        return useLocationNet;
    }

    public void setUseLocationNet(boolean useLocationNet) {
        this.useLocationNet = useLocationNet;
    }

    public float getLight_objective() {
        return light_objective;
    }

    public float getLight_current() {
        return light_current;
    }

    public void setLight_current(float light_current) {
        this.light_current = light_current;
    }

    public float getLight_total() {
        return light_total;
    }

    public void setLight_total(float light_total) {
        this.light_total = light_total;
    }

    public float getAccelerometerX() {
        return accelerometerX;
    }

    public void setAccelerometerX(float accelerometerX) {
        this.accelerometerX = accelerometerX;
    }

    public float getAccelerometerY() {
        return accelerometerY;
    }

    public void setAccelerometerY(float accelerometerY) {
        this.accelerometerY = accelerometerY;
    }

    public float getAccelerometerZ() {
        return accelerometerZ;
    }

    public void setAccelerometerZ(float accelerometerZ) {
        this.accelerometerZ = accelerometerZ;
    }

    public void setAccelerometer(float x, float y, float z){
        this.accelerometerX = x;
        this.accelerometerY = y;
        this.accelerometerZ = z;
    }

    public double getLatitude() {
        return latitude;
    }

    public void setLatitude(double latitude) {
        this.latitude = latitude;
    }

    public double getLongitude() {
        return longitude;
    }

    public void setLongitude(double longitude) {
        this.longitude = longitude;
    }

    public double getAltitude() {
        return altitude;
    }

    public void setAltitude(double altitude) {
        this.altitude = altitude;
    }

    public void setLocation(float latitude, float longitude, float altitude){
        this.latitude = latitude;
        this.longitude = longitude;
        this.altitude = altitude;
    }

    public double getLatitudeNet() {
        return latitudeNet;
    }

    public void setLatitudeNet(double latitudeNet) {
        this.latitudeNet = latitudeNet;
    }

    public double getLongitudeNet() {
        return longitudeNet;
    }

    public void setLongitudeNet(double longitudeNet) {
        this.longitudeNet = longitudeNet;
    }

    public double getAltitudeNet() {
        return altitudeNet;
    }

    public void setAltitudeNet(double altitudeNet) {
        this.altitudeNet = altitudeNet;
    }

    public void setLocationNet(float latitudeNet, float longitudeNet, float altitudeNet){
        this.latitudeNet = latitudeNet;
        this.longitudeNet = longitudeNet;
        this.altitudeNet = altitudeNet;
    }

    public Weather getWeather() {
        return weather;
    }

    public void setWeather(Weather weather) {
        this.weather = weather;
    }

    public int getAccuracy_light() {
        return accuracy_light;
    }

    public void setAccuracy_light(int accuracy_light) {
        this.accuracy_light = accuracy_light;
    }

    public int getAccuracy_accelerometer() {
        return accuracy_accelerometer;
    }

    public void setAccuracy_accelerometer(int accuracy_accelerometer) {
        this.accuracy_accelerometer = accuracy_accelerometer;
    }

    public LightFragment getCurrentLightFragment() {
        return currentLightFragment;
    }

    public void setCurrentLightFragment(LightFragment currentLightFragment) {
        this.currentLightFragment = currentLightFragment;
    }

    public WeatherFragment getCurrentWeatherFragment() {
        return currentWeatherFragment;
    }

    public void setCurrentWeatherFragment(WeatherFragment currentWeatherFragment) {
        this.currentWeatherFragment = currentWeatherFragment;
    }

    public boolean increaseLightTotalPoints(float add){
        this.light_total += add;
        if(this.light_total>= this.light_objective){
            this.light_total = this.light_objective;
            return true;
        }else{
            return false;
        }
    }
}

