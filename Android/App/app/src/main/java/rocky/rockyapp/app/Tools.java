package rocky.rockyapp.app;

import android.graphics.Color;

class Tools {

    public static final int SALTO_GRIS = 1118481;
    public static final int MUCHA_LUX = 600;

    static public int luxToGreyColor(float luxValue){
        if(luxValue >= MUCHA_LUX)
            return Color.WHITE;
        int lightPer = (int)(luxValue / MUCHA_LUX * 16);
        return Color.BLACK + SALTO_GRIS * lightPer;
    }

    static public byte gyroToServo256(float gyroValue){
        if(gyroValue >= 0.5)
            return (byte)255;
        if(gyroValue <= -0.5)
            return 0;
        return (byte)(int)((gyroValue + 0.5) * 256 );
    }

    static public String codToStringColor(char value){
        switch(value){
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

    static public String codToStringModo(char value){
        switch(value){
            case 'a':
                return "Automatico";
            case 'm':
                return "Manual";
            default:
                return "Error";
        }
    }

    static public int codToBGColor(char value){
        switch(value){
            case '0':
                return 0xFFA0522D;
            case '1':
                return Color.GREEN;
            case '2':
                return Color.BLUE;
            case '3':
                return Color.RED;
            case '4':
                return 0xFFFF7700;
            case '5':
                return Color.YELLOW;
            default:
                return Color.DKGRAY;
        }
    }
}
