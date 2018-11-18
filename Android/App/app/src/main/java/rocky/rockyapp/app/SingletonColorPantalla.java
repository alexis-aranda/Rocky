package rocky.rockyapp.app;

import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.view.Window;
import android.widget.Toast;

import static android.content.Context.SENSOR_SERVICE;

class SingletonColorPantalla implements SensorEventListener{
    private static SingletonColorPantalla instancia;
    private static Activity actividad;
    private SensorManager sensorManager;
    private Sensor sensorDeLuz;

    //private constructor to avoid client applications to use constructor
    private SingletonColorPantalla(Activity actividad){
        setActivity(actividad);
        sensorManager = (SensorManager) actividad.getSystemService(SENSOR_SERVICE);
        sensorDeLuz = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        if(sensorDeLuz == null){
            Toast.makeText(actividad.getApplicationContext(), "No hay Sensor de Luz", Toast.LENGTH_SHORT).show();
        }
        else{
            Toast.makeText(actividad.getApplicationContext(), "Hay Sensor de Luz", Toast.LENGTH_SHORT).show();
            sensorManager.registerListener(this, sensorDeLuz, SensorManager.SENSOR_DELAY_NORMAL);
        }
    }

    public static void pantallaActiva(Activity actividad){
        if(instancia == null){
            instancia = new SingletonColorPantalla(actividad);
        }
        instancia.setActivity(actividad);
    }

    private void setActivity(Activity actividad){
        this.actividad = actividad;
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        synchronized (this){
            float[] masData;
            if(event.sensor.getType() == Sensor.TYPE_LIGHT){
                masData = event.values;
                actividad.getWindow().getDecorView().setBackgroundColor(Tools.luxToGreyColor(masData[0]));
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
}
