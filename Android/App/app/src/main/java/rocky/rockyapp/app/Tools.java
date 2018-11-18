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
}
