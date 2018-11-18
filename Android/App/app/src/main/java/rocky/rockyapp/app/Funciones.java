package rocky.rockyapp.app;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

/*

Aca irian los listeners de los botones cantRocklets, mover tobogan y pausar
 */
public class Funciones extends AppCompatActivity {

    TextView tvContverde;
    TextView tvContazul;
    TextView tvControjo;
    TextView tvContnaranja;
    TextView tvContamarillo;
    TextView tvContmarron;
    Button tobogan;
    Button pausar;

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

        tvContverde = (TextView)findViewById(R.id.textViewCont1); //contador verde
        tvContazul = (TextView)findViewById(R.id.textViewCont2); //contador azul
        tvControjo = (TextView)findViewById(R.id.textViewCont3); //contador rojo
        tvContnaranja = (TextView)findViewById(R.id.textViewCont4); //contador naranja
        tvContamarillo = (TextView)findViewById(R.id.textViewCont5); //contador amarillo
        tvContmarron = (TextView)findViewById(R.id.textViewCont6); //contador marron

        tobogan = (Button)findViewById(R.id.btnTobogan);
        pausar = (Button)findViewById(R.id.btnPausar);

        btAdapter = BluetoothAdapter.getDefaultAdapter();

        btIn = Handler_Msg_Hilo_Principal();

        //ver handlers para los botones



    }

    @Override
    protected void onResume() {
        super.onResume();

        Intent i = getIntent();
        Bundle extras = i.getExtras();

        adress = extras.getString("Direccion_Bluetooth");

        BluetoothDevice device = btAdapter.getRemoteDevice(adress); //obtengo el adress del device


        //conexion del bt a traves del socket
        try{
            btSocket = createBluetoothSocket(device);
        }catch (IOException e) {
            Toast.makeText(getApplicationContext(), "Error socket", Toast.LENGTH_SHORT).show();
        }

        // conexion del socket
        try
        {
            btSocket.connect(); //no pasa de aca--- segun vale, esto podria funcionar solo con arduino
        }
        catch (IOException e)
        {
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
        thread.write("x"); //no muestra la interfaz de la activity porque no termina este metodo


    }

    //RECIBIR DATOS DEL ARDUINO
    //Handler que sirve que permite mostrar datos en el Layout al hilo secundario
    //CAMBIAR POR LOS DATOS DE CANTIDAD DE ROCKLETS
    @SuppressLint("HandlerLeak")
    private Handler Handler_Msg_Hilo_Principal ()
    {
        return new Handler() {
            public void handleMessage(android.os.Message msg)
            {
                //si se recibio un msj del hilo secundario
                if (msg.what == estadoHandler)
                {
                    //voy concatenando el msj
                    String readMessage = (String) msg.obj;
                    recDataString.append(readMessage);
                    int endOfLineIndex = recDataString.indexOf("\r\n");//definir caracter de fin de datos enviados


                    //cuando recibo toda una linea la muestro en el layout
                    if (endOfLineIndex > 0)
                    {
                        String dataInPrint = recDataString.substring(0, endOfLineIndex);//obtengo toda la linea
                        if(dataInPrint.charAt(0) == '#') { //arduino deberia mandar un # para que android reconozca estos datos
                           String [] array = dataInPrint.split("-");//los datos deberian estar separados con un '-' en arduino
                            tvContverde.setText(array[0].substring(1,array.length-1)); // el primer caracter de este contador es un #
                            tvContazul.setText(array[1]);
                            tvControjo.setText(array[2]);
                            tvContnaranja.setText(array[3]);
                            tvContamarillo.setText(array[4]);
                            tvContmarron.setText(array[5]);
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
Clase para manejar el hilo secundario
 */
    private class ConnectedThread extends Thread
    {
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

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
            while (true)
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


