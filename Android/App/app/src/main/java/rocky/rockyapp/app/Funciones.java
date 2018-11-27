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
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

/*

Aca irian los listeners de los botones cantRocklets, mover tobogan y pausar
 */
public class Funciones extends AppCompatActivity  implements SensorEventListener {
    //Cosas graficas
    TextView tvContverde;
    TextView tvContazul;
    TextView tvControjo;
    TextView tvContnaranja;
    TextView tvContamarillo;
    TextView tvContmarron;
    TextView txtModo;
    TextView txtUltLanzado;
    TextView textmsg;
    Button tobogan;
    Button pausar;
    TableRow rowModo;
    TableRow rowColor;

    //Constantes
    private static final String MODO_CELULAR_ON = "0";
    private static final String MODO_CELULAR_OFF = "1";
    private static final String PAUSA = "2";
    private static final String REANUDAR = "3";
    private static final String SOLTAR_ROCKLET = "4";
    private static final String GYRO = "5";

    //Sensores
    private SensorManager sensorManager;
    private Sensor sensorProx;
    private Sensor sensorGyro;

    //Control de estado
    private boolean readyToLaunch = true;
    private boolean corriendo=true;
    private boolean isTobogan=false;

    //comunicacion con arduino
    Handler btIn;
    final int estadoHandler=0;
    private BluetoothAdapter btAdapter=null;
    private BluetoothSocket btSocket=null;
    private StringBuilder recDataString = new StringBuilder();
    private ConnectedThread thread;
    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private static String adress = null;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_funciones);
        //Sensores
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        sensorProx = sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
        sensorGyro = sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
        //Contadores
        tvContverde = (TextView)findViewById(R.id.textViewCont1); //contador verde
        tvContazul = (TextView)findViewById(R.id.textViewCont2); //contador azul
        tvControjo = (TextView)findViewById(R.id.textViewCont3); //contador rojo
        tvContnaranja = (TextView)findViewById(R.id.textViewCont4); //contador naranja
        tvContamarillo = (TextView)findViewById(R.id.textViewCont5); //contador amarillo
        tvContmarron = (TextView)findViewById(R.id.textViewCont6); //contador marron
        //Botones y labels
        textmsg = (TextView) findViewById(R.id.textmsg);
        tobogan = (Button)findViewById(R.id.btnTobogan);
        pausar = (Button)findViewById(R.id.btnPausar);
        rowModo = (TableRow) findViewById(R.id.row_modo);
        rowColor = (TableRow) findViewById(R.id.row_color);
        txtModo = (TextView)findViewById(R.id.txtModo);
        txtUltLanzado = (TextView)findViewById(R.id.txtUltLanzado);
        //Bluetooth
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        btIn = Handler_Msg_Hilo_Principal();
        //Listeners
        pausar.setOnClickListener(pausarListener);
        tobogan.setOnClickListener(toboganListener);
        //Control de sensores
        if(sensorGyro == null){ //Esta comprobación debería hacerse en la activity llamadora seguro
            Toast.makeText(getApplicationContext(), "No hay Sensor de Rotación", Toast.LENGTH_SHORT).show();
            Toast.makeText(getApplicationContext(), "El modo tobogan estará deshabilitado", Toast.LENGTH_SHORT).show();
            tobogan.setEnabled(false); //Se deshabilita el botón de tobogan
        }
        else{
            Toast.makeText(getApplicationContext(), "Hay Sensor de Rotación", Toast.LENGTH_SHORT).show();
            sensorManager.registerListener(this, sensorGyro, SensorManager.SENSOR_DELAY_NORMAL);
            if(sensorProx == null){
                Toast.makeText(getApplicationContext(), "No hay Sensor de Proximidad", Toast.LENGTH_SHORT).show();
                Toast.makeText(getApplicationContext(), "El modo tobogan estará deshabilitado", Toast.LENGTH_SHORT).show();
                tobogan.setEnabled(false); //Se deshabilita el botón de tobogan
            }
            else{
                Toast.makeText(getApplicationContext(), "Hay Sensor de Proximidad", Toast.LENGTH_SHORT).show();
                sensorManager.registerListener(this,sensorProx, SensorManager.SENSOR_DELAY_NORMAL);
            }
        }

    }

    @Override
    protected void onResume() {
        super.onResume();
        //Seteo de cambio de color de pantalla
        SingletonColorPantalla.pantallaActiva(Funciones.this);
        //Preparación del bluetooth
        Intent i = getIntent();
        Bundle extras = i.getExtras();
        adress = extras.getString("Direccion_Bluetooth");
        BluetoothDevice device = btAdapter.getRemoteDevice(adress); //obtengo el adress del device
        //Conexion del bt a traves del socket
        try{
            btSocket = createBluetoothSocket(device);
        }catch (IOException e) {
            Toast.makeText(getApplicationContext(), "Error socket", Toast.LENGTH_SHORT).show();
        }
        //Conexion del socket
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
        thread = new Funciones.ConnectedThread(btSocket);
        thread.start();
        //compruebo que esta conectado. sino se cierra
        thread.write("x");
        //Se registran los sensores
        sensorManager.registerListener(this,sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY),SensorManager.SENSOR_DELAY_NORMAL);
        sensorManager.registerListener(this,sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR),SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    protected void onPause(){
        super.onPause();
        //Se desactiva el uso de sensores
        SingletonColorPantalla.pantallaInactiva();
        sensorManager.unregisterListener(this);
        //Se deja de escuchar al bluetooth
        thread.kill();
        try {
            btSocket.close();
        } catch (IOException e) {
            Toast.makeText(getApplicationContext(),"Error al cerrar socket",Toast.LENGTH_SHORT).show();
        }
    }

    //RECIBIR DATOS DEL ARDUINO
    //Handler que sirve que permite mostrar datos en el Layout al hilo secundario
    //CAMBIAR POR LOS DATOS DE CANTIDAD DE ROCKLETS
    @SuppressLint("HandlerLeak")
    private Handler Handler_Msg_Hilo_Principal ()
    {
        return new Handler() {
            public void handleMessage(Message msg)
            {
                //si se recibio un msj del hilo secundario
                if (msg.what == estadoHandler)
                {
                    //Voy concatenando el msj
                    String readMessage = (String) msg.obj;
                    recDataString.append(readMessage);
                    int endOfLineIndex = recDataString.indexOf("\r\n");//Caracter de fin de datos
                    //Cuando recibo toda una linea, me fijo que estoy recibiendo
                    if (endOfLineIndex > 0)
                    {
                        String dataInPrint = recDataString.substring(0, endOfLineIndex);//Obtengo toda la linea
                        switch (dataInPrint.charAt(0)){
                            case '#':
                                String [] array = dataInPrint.split("-");//Los datos deberian estar separados con un '-' en Arduino
                                if(array.length >= 7) {
                                    //Ultimo color sensado
                                    char color = array[0].charAt(1);
                                    //Colores de fondo
                                    txtUltLanzado.setText(Tools.codToStringColor(color));
                                    rowColor.setBackgroundColor(Tools.codToBGColor(color));
                                    rowModo.setBackgroundColor(Tools.codToBGColor(color));
                                    //Contadores
                                    tvContverde.setText(array[2]);
                                    tvContazul.setText(array[3]);
                                    tvControjo.setText(array[4]);
                                    tvContnaranja.setText(array[5]);
                                    tvContamarillo.setText(array[6]);
                                    tvContmarron.setText(array[1]);
                                    //Control de estado
                                    readyToLaunch=true;
                                    if(isTobogan) //Si está en modo tobogan, se pone el mensaje
                                        textmsg.setText("Ahora probá girando el celu!.\nAcercá la mano para tirar el rocklet!");
                                }
                                break;
                            case '$':
                                txtModo.setText(Tools.codToStringModo(dataInPrint.charAt(1)));
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

    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {

        return  device.createRfcommSocketToServiceRecord(BTMODULEUUID);
    }

    private View.OnClickListener pausarListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            if(corriendo){
                thread.write(PAUSA);
                pausar.setText("Reanudar");
                corriendo=false;
            }
            else{
                thread.write(REANUDAR);
                pausar.setText("Pausar");
                corriendo=true;
            }
        }
    };

    private View.OnClickListener toboganListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            if(!isTobogan) {
                if (corriendo) {
                    thread.write(MODO_CELULAR_ON);
                    textmsg.setText("Ahora probá girando el celu!.\nAcercá la mano para tirar el rocklet!");
                    tobogan.setText("Volver a Automatico");
                    isTobogan=true;
                } else
                    Toast.makeText(getApplicationContext(), "No se puede pasar a modo tobogan en pausa!", Toast.LENGTH_SHORT).show();
            }else{
                if (corriendo) {
                    thread.write(MODO_CELULAR_OFF);
                    textmsg.setText("");
                    tobogan.setText("Mover Tobogan");
                    isTobogan=false;
                    txtModo.setText("Automatico");
                } else
                    Toast.makeText(getApplicationContext(), "No se puede salir del modo tobogan en pausa!", Toast.LENGTH_SHORT).show();
            }
        }
    };

    @Override
    public void onSensorChanged(SensorEvent event) {
        synchronized (this){
            float[] masData;
            switch(event.sensor.getType()){
                case Sensor.TYPE_PROXIMITY:
                    masData = event.values;
                    if(masData[0] == 0)
                        if(isTobogan)
                            if(readyToLaunch){
                                Toast.makeText(getApplicationContext(), "Rocklet lanzado!", Toast.LENGTH_SHORT).show();
                                textmsg.setText("Esperando nuevo rocklet\nAguarde porfavor");
                                thread.write(Funciones.SOLTAR_ROCKLET);
                                readyToLaunch=false;
                            }else{
                                Toast.makeText(getApplicationContext(), "Aguarde para lanzar!", Toast.LENGTH_SHORT).show();
                            }
                    break;
                case Sensor.TYPE_ROTATION_VECTOR:
                    masData = event.values;
                    if(isTobogan)
                        if(readyToLaunch){
                            thread.write(Funciones.GYRO+String.format( "%04d",Tools.gyroToServo(masData[2])));
                        }
                default:
                    break;
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
Clase para manejar el hilo secundario
 */
    public class ConnectedThread extends Thread
    {
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;
        private boolean isAlive=true;

        //Constructor de la clase del hilo secundario
        public ConnectedThread(BluetoothSocket socket)
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


