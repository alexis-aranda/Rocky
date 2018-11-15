package rocky.rockyapp.app;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;

/*

Aca irian los listeners de los botones cantRocklets, mover tobogan y pausar
 */
public class Funciones extends AppCompatActivity {

    TextView tvCont1;
    TextView tvCont2;
    TextView tvCont3;
    TextView tvCont4;
    TextView tvCont5;
    TextView tvCont6;
    Button tobogan;
    Button pausar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_funciones);

        tvCont1 = (TextView)findViewById(R.id.textViewCont1);
        tvCont2 = (TextView)findViewById(R.id.textViewCont2);
        tvCont3 = (TextView)findViewById(R.id.textViewCont3);
        tvCont4 = (TextView)findViewById(R.id.textViewCont4);
        tvCont5 = (TextView)findViewById(R.id.textViewCont5);
        tvCont6 = (TextView)findViewById(R.id.textViewCont6);

        tobogan = (Button)findViewById(R.id.btnTobogan);
        pausar = (Button)findViewById(R.id.btnPausar);
    }
}
