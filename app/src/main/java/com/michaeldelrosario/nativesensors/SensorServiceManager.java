package com.michaeldelrosario.nativesensors;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.os.PowerManager;

import static com.michaeldelrosario.nativesensors.NativeSensorsJNI.nativeFree;
import static com.michaeldelrosario.nativesensors.NativeSensorsJNI.nativeInit;
import static com.michaeldelrosario.nativesensors.NativeSensorsJNI.nativeStart;
import static com.michaeldelrosario.nativesensors.NativeSensorsJNI.nativeStop;

public class SensorServiceManager extends Service{
    public static final String TAG = "SensorServiceManager";

    public static final String IS_RUNNING = "SENSOR_SERVICE_RUNNING";
    public static final String STATUS     = "SENSOR_SERVICE_STATUS";
    protected PowerManager.WakeLock _wakeLock;
    protected PowerManager _powerManager;

    protected boolean _bForegroundService = false;

    /**
     * Constructors
     */
    public SensorServiceManager() {
    }



    //@SuppressWarnings("deprecation")
    @Override //onStart is called within onStartCommand
    public int onStartCommand(Intent intent, int flags, int startId) {
        if(!_bForegroundService) {
            _bForegroundService = true;
            int requestID = (int)System.currentTimeMillis();
            /** Add notification bundle and put as Extra into intent **/
            Bundle notificationBundle = new Bundle();
            notificationBundle.putBoolean(IS_RUNNING,true);

            Intent notificationIntent = new Intent(getApplicationContext(), NativeSensorsActivity.class);
            notificationIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
            notificationIntent.setAction(Intent.ACTION_MAIN);
            notificationIntent.putExtra(STATUS,notificationBundle);

            PendingIntent pendingIntent = PendingIntent.getActivity(this, requestID,
                    notificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);

            Notification note = new Notification.Builder(this)
                    .setSmallIcon(R.drawable.app_icon)
                    .setContentTitle(getResources().getString(R.string.app_name))
                    .setContentText("Running")
                    .setWhen(System.currentTimeMillis())
                    .setContentIntent(pendingIntent)
                    .getNotification();
            note.flags |= Notification.FLAG_NO_CLEAR;
            note.flags |= Notification.FLAG_FOREGROUND_SERVICE; // addition

            startForeground(startId, note );
        }
        return START_STICKY;
    }

    /**
     *
     */
    @Override
    public void onDestroy() {
        unregisterSensorListeners();
        releaseWakeLock();
        stopForeground(true);
        super.onDestroy();
    }

    /**
     * Unregister Sensor Listeners first to stop producing sensorData objects that need to be processed
     */
    public void unregisterSensorListeners() {
        nativeStop();
        nativeFree();
    }

    @Override
    public void onCreate() {
        super.onCreate();
        acquireWakeLock();
        registerSensorListeners();
    }

    /**
     * Initializes a partial wake lock so that sensors can continue to be polled when the screen is off
     */
    public void acquireWakeLock() {
        _powerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
        _wakeLock = _powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "Partial Wake Lock");
        _wakeLock.acquire();
    }

    public void releaseWakeLock(){
        _wakeLock.release();
    }

    /**
     * Request access to each sensor and specify the sampling rate
     */
    public void registerSensorListeners() {
        nativeInit();
        nativeStart();
    }

    /**
     * If your service is private to your own application and runs in the same process as the client (which is common)
     * you should create your interface by extending the Binder class and returning an instance of it from onBind().
     *
     */
    @Override
    public IBinder onBind(Intent intent) {
        return new mBinder();
    }

    public class mBinder extends Binder {
        SensorServiceManager getService() {
            return SensorServiceManager.this;
        }
    }
}
