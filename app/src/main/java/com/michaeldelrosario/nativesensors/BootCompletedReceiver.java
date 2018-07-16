package com.michaeldelrosario.nativesensors;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class BootCompletedReceiver extends BroadcastReceiver {
    final static String TAG = "BootCompletedReceiver";
    //ReminderAlarmReceiver reminderAlarm = new ReminderAlarmReceiver();
//    HourlyUploadAlarmReceiver hourlyAlarm = new HourlyUploadAlarmReceiver();
    public BootCompletedReceiver() {
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)) {
            Log.d(TAG, "ACTION_BOOT_COMPLETED");
            context.startService(new Intent(context, SensorServiceManager.class)); // start sensorData Service
//            hourlyAlarm.setAlarm(context);
            //reminderAlarm.setAlarm(context);
        }
    }
}
