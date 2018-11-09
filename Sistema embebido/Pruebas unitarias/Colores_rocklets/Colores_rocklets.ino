#include "ColorRocklet.h"

/*Definicion de pines*/
#define PIN_0_LECTOR_COLOR 4
#define PIN_1_LECTOR_COLOR 5
#define PIN_2_LECTOR_COLOR 6
#define PIN_3_LECTOR_COLOR 7
#define PIN_SALIDA_LECTOR_COLOR 8

/*Seteo de variables y pines*/
ColorRocklet lectorColor = ColorRocklet(PIN_0_LECTOR_COLOR, PIN_1_LECTOR_COLOR, PIN_2_LECTOR_COLOR, PIN_3_LECTOR_COLOR, PIN_SALIDA_LECTOR_COLOR);
int color; //Color del rocklet leído
bool sensado;

void setup() {
  Serial.begin(9600);
  sensado = false;
}

void loop() {
  if(!sensado){
    
          lectorColor.prenderSensor();
          sensado = lectorColor.hacerLectura();
          /*
           //planteo una alternativa usando un contador de lecturas
           color = lectorColor.identificarColor();
           if(color != ColorRocklet::NO_IDENTIFICADO){
            sensado = true;
           }
          */
        }else{
          sensado=false;
          lectorColor.apagarSensor();
          color = lectorColor.getColor();
          switch( color )
            {
              case ColorRocklet::VERDE:
                Serial.println("V");
               break;
              case ColorRocklet::AZUL:
                Serial.println("Az");
                break;
              case ColorRocklet::ROJO:
                Serial.println("R");
                break;
              case ColorRocklet::NARANJA:
                Serial.println("N");
                break;
              case ColorRocklet::AMARILLO:
                Serial.println("Am");
               break;
             case ColorRocklet::MARRON:
                Serial.println("M");
                break;
              default:
                Serial.println("Sin color");
            }
            delay(2000);
        }
}
