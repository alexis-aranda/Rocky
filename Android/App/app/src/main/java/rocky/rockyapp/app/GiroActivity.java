package rocky.rockyapp.app;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;


public class GiroActivity extends AppCompatActivity implements SensorEventListener {
    private SensorManager sensorManager;
    private Sensor sensorProx;
    private Sensor sensorGyro;
    private TextView textGyro;
    private TextView textEstado;
    private boolean readyToLaunch = true;
    private Button pausar;
    private boolean corriendo=true;

    //comunicacion con arduino
    Handler btIn;
    final int estadoHandler=0;
    private BluetoothAdapter btAdapter=null;
    private BluetoothSocket btSocket=null;
    private StringBuilder recDataString = new StringBuilder();

    private ConnectedThreadGiro thread;

    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private static String adress = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_giro);
    //Probablemente esa clase deba tener un indicador de que en tal momento o en tal otro se puede girar y tirar
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        sensorProx = sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
        sensorGyro = sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
        pausar = (Button)findViewById(R.id.buttonPausa);
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        pausar.setOnClickListener(pausarListener);

        btIn = Handler_Msg_Giro();

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

        Intent i = getIntent();
        Bundle extras = i.getExtras();

        adress = extras.getString("Direccion_Bluetooth");

        BluetoothDevice device = btAdapter.getRemoteDevice(adress); //obtengo el adress del device


        //conexion del bt a traves del socket
        try{
            btSocket = device.createRfcommSocketToServiceRecord(BTMODULEUUID);
        }catch (IOException e) {
            Toast.makeText(getApplicationContext(), "Error socket", Toast.LENGTH_SHORT).show();
        }

        // conexion del socket
        try
        {
            btSocket.connect(); //Solo con Arduino: debe haber una aplicación esperando la conexion
        }
        catch (IOException e)
        {
            Toast.makeText(getApplicationContext(),"No se puede conectar!",Toast.LENGTH_SHORT).show();
            try
            {
                btSocket.close();
            }
            catch (IOException e2)
            {
                Toast.makeText(getApplicationContext(),"Error al cerrar socket",Toast.LENGTH_SHORT).show();
            }
        }

        //Una vez establecida la conexion con el Hc05 se crea el hilo secundario, el cual va a recibir
        // los datos de Arduino atraves del bluethoot
        thread = new ConnectedThreadGiro(btSocket);
        thread.start();

        //compruebo que esta conectado. sino se cierra
        thread.write("x"); //no muestra la interfaz de la activity porque no termina este metodo


        sensorManager.registerListener(this,sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY),SensorManager.SENSOR_DELAY_NORMAL);
        sensorManager.registerListener(this,sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR),SensorManager.SENSOR_DELAY_NORMAL);

        super.onResume();
    }

    @Override
    //Cuando se llama al metodo OnPausa se cancela la busqueda de dispositivos bluethoot
    public void onPause()
    {

        sensorManager.unregisterListener(this);
        super.onPause();
        thread.kill();
        try {
            btSocket.close();
        } catch (IOException e) {
            Toast.makeText(getApplicationContext(),"Error al cerrar socket",Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        synchronized (this){
            float[] masData;
            switch(event.sensor.getType()){
                case Sensor.TYPE_PROXIMITY:
                    masData = event.values;
                    if(readyToLaunch && masData[0] == 0){
                        Toast.makeText(getApplicationContext(), "Rocklet lanzado!", Toast.LENGTH_SHORT).show(); //Provisorio
                        textEstado.setText("Esperando nuevo rocklet\nAguarde porfavor");
                        thread.write(Funciones.SOLTAR_ROCKLET);
                        readyToLaunch=false;
                    }
                    break;
                case Sensor.TYPE_ROTATION_VECTOR:
                    masData = event.values;
                    textGyro.setText(String.valueOf(Tools.gyroToServo(masData[2]))); //Provisorio
                    if(readyToLaunch){
                        thread.write(Funciones.GYRO+String.valueOf(masData[2]));
                    }
                    //TODO llamado a metodo para mover servo
                default:
                    break;
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    private View.OnClickListener pausarListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            if(corriendo){
                thread.write(Funciones.PAUSA);
                pausar.setText("Reanudar");
                corriendo=false;
            }
            else{
                thread.write(Funciones.REANUDAR);
                pausar.setText("Pausar");
                corriendo=true;
            }
        }
    };

    //RECIBIR DATOS DEL ARDUINO
    //Handler que sirve que permite mostrar datos en el Layout al hilo secundario
    //CAMBIAR POR LOS DATOS DE CANTIDAD DE ROCKLETS
    @SuppressLint("HandlerLeak")
    private Handler Handler_Msg_Giro ()
    {
        return new Handler() {
            public void handleMessage(Message msg)
            {
                //si se recibio un msj del hilo secundario
                if (msg.what == estadoHandler)
                {
                    //voy concatenando el msj
                    String readMessage = (String) msg.obj;
                    recDataString.append(readMessage);
                    int endOfLineIndex = recDataString.indexOf("\r\n");//definir caracter de fin de datos enviados


                    //cuando recibo toda una linea, me fijo que estoy recibiendo
                    if (endOfLineIndex > 0)
                    {
                        String dataInPrint = recDataString.substring(0, endOfLineIndex);//obtengo toda la linea
                        //textmsg.setText(dataInPrint);
                        switch (dataInPrint.charAt(0)){

                            case '#':
                                String [] array = dataInPrint.split("-");//los datos deberian estar separados con un '-' en arduino
                                if(array.length >= 7) {
                                    //txtUltLanzado.setText(Tools.codToStringColor(array[0].charAt(1)));
                                    if(!readyToLaunch){
                                        getWindow().getDecorView().setBackgroundColor(Tools.codToBGColor(array[0].charAt(1)));
                                        readyToLaunch=true;
                                        textEstado.setText("Rotá la pantalla para girar el servo.\nAcercá la mano al celular para tirar el rocklet!");
                                    }else
                                        Toast.makeText(getApplicationContext(), "Mensaje inesperado!", Toast.LENGTH_SHORT).show();
                                    //tvContverde.setText(array[2]); // el primer caracter de este contador es un #
                                    //tvContazul.setText(array[3]);
                                    //tvControjo.setText(array[4]);
                                    //tvContnaranja.setText(array[5]);
                                    //tvContamarillo.setText(array[6]);
                                    //tvContmarron.setText(array[1]);
                                }
                                break;
                            default:
                                break;
                        }

                        recDataString.delete(0, recDataString.length());
                    }
                }
            }
        };

    }

    /*
Clase para manejar el hilo secundario
*/
    public class ConnectedThreadGiro extends Thread
    {
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;
        private boolean isAlive=true;

        //Constructor de la clase del hilo secundario
        public ConnectedThreadGiro(BluetoothSocket socket)
        {
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try
            {
                //Create I/O streams for connection
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        //metodo run del hilo, que va a entrar en una espera activa para recibir los msjs del HC05
        public void run()
        {
            byte[] buffer = new byte[256];
            int bytes;

            //el hilo secundario se queda esperando mensajes del HC05
            while (isAlive)
            {
                try
                {
                    //se leen los datos del Bluethoot
                    bytes = mmInStream.read(buffer);
                    String readMessage = new String(buffer, 0, bytes);

                    //se muestran en el layout de la activity, utilizando el handler del hilo
                    // principal antes mencionado
                    btIn.obtainMessage(estadoHandler, bytes, -1, readMessage).sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }
        }

        public void kill(){
            this.isAlive=false;
        }

        //write method
        public void write(String input) {
            byte[] msgBuffer = input.getBytes();           //converts entered String into bytes
            try {
                mmOutStream.write(msgBuffer);                //write bytes over BT connection via outstream
            } catch (IOException e) {
                //if you cannot write, close the application
                Toast.makeText(getApplicationContext(),"La conexion fallo",Toast.LENGTH_SHORT).show();
                finish();

            }
        }
    }

}
