package rocky.rockyapp.app;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

public class GiroActivity extends AppCompatActivity implements SensorEventListener {
    private SensorManager sensorManager;
    private Sensor sensorProx;
    private Sensor sensorGyro;
    private TextView textGyro;
    private TextView textEstado;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_giro);
    //Probablemente esa clase deba tener un indicador de que en tal momento o en tal otro se puede girar y tirar
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        sensorProx = sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
        sensorGyro = sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
        //Valido el sensor de luz, y preparo su Listener
        if(sensorGyro == null){ //Esta comprobación debería hacerse en la activity llamadora seguro
            Toast.makeText(getApplicationContext(), "No hay Sensor de Rotación", Toast.LENGTH_SHORT).show();
        }
        else{
            Toast.makeText(getApplicationContext(), "Hay Sensor de Rotación", Toast.LENGTH_SHORT).show();
            sensorManager.registerListener(this, sensorGyro, SensorManager.SENSOR_DELAY_NORMAL);
        }
        if(sensorProx == null){
            Toast.makeText(getApplicationContext(), "No hay Sensor de Proximidad", Toast.LENGTH_SHORT).show();
        }
        else{
            Toast.makeText(getApplicationContext(), "Hay Sensor de Proximidad", Toast.LENGTH_SHORT).show();
            sensorManager.registerListener(this,sensorProx, SensorManager.SENSOR_DELAY_NORMAL);
        }

        textGyro = (TextView) findViewById(R.id.textGyro);
        textEstado = (TextView) findViewById(R.id.textEstado);
    }

    @Override
    protected void onResume() {
        sensorManager.registerListener(this,sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY),SensorManager.SENSOR_DELAY_NORMAL);
        sensorManager.registerListener(this,sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR),SensorManager.SENSOR_DELAY_NORMAL);
        SingletonColorPantalla.pantallaActiva(this);
        super.onResume();
    }

    @Override
    //Cuando se llama al metodo OnPausa se cancela la busqueda de dispositivos bluethoot
    public void onPause()
    {
        SingletonColorPantalla.pantallaInactiva();
        sensorManager.unregisterListener(this);
        super.onPause();
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        synchronized (this){
            float[] masData;
            switch(event.sensor.getType()){
                case Sensor.TYPE_PROXIMITY:
                    masData = event.values;
                    if(masData[0] == 0){
                        Toast.makeText(getApplicationContext(), "Rocklet lanzado!", Toast.LENGTH_SHORT).show(); //Provisorio
                        //TODO llamado a método para lanzar
                    }
                    break;
                case Sensor.TYPE_ROTATION_VECTOR:
                    masData = event.values;
                    textGyro.setText(String.valueOf(Tools.gyroToServo(masData[2]))); //Provisorio
                    //TODO llamado a metodo para mover servo
                default:
                    break;
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
}
