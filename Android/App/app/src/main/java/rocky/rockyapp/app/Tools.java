package rocky.rockyapp.app;

import android.graphics.Color;

class Tools {

    public static final int SALTO_GRIS = 1118481;

    static public int luxToColor(float luxValue){
        if(luxValue > 100)
            luxValue = 100;
        int colorFinal = (int)((float)16777216 / 100 * luxValue );
        if(colorFinal == 0) return -1;
        return colorFinal * -1;
    }

    static public int luxToGreyColor(float luxValue){
        if(luxValue >= 128)
            return Color.WHITE;
        int lightPer = (int)(luxValue / 128 * 16);
        return Color.BLACK + SALTO_GRIS * lightPer;
    }

    static public int gyroToServo(float gyroValue){
        if(gyroValue >= 0.5)
            return 1023;
        if(gyroValue <= -0.5)
            return 0;
        return (int)((gyroValue + 0.5) * 1024 );
    }

    static public int gyroToServoInverse(float gyroValue){ //Inversa, por si el servo gira al reves, solo dejar la que se valla a usar
        if(gyroValue >= 0.5)
            return 0;
        if(gyroValue <= -0.5)
            return 1023;
        return (int)((gyroValue - 0.5) * -1024 );
    }

    static public String codToStringColor(String value){
        switch(value.charAt(0)){
            case '0':
                return "Marron";
            case '1':
                return "Verde";
            case '2':
                return "Azul";
            case '3':
                return "Rojo";
            case '4':
                return "Naranja";
            case '5':
                return "Amarillo";
            default:
                return "Error";
        }
    }

    static public String codToStringModo(String value){
        switch(value.charAt(0)){
            case 'a':
                return "Automatico";
            case 'm':
                return "Manual";
            default:
                return "Error";
        }
    }
}
