#include "nuestroServo.h"
#include "NuestroPotenciometro.h"
#include "NuestroLED.h"
#include "ColorRocklet.h"
#include "NuestraBarreraLaser.h"
#include "NuestroPulsador.h"

/*Definicion de estados*/
#define EN_ESPERA 0
#define BUSCANDO 1
#define LLEVANDO 2
#define SENSANDO 3
#define TOBOGAN_A 4
#define TOBOGAN_M 5
#define DESPACHANDO 6

/*Defino los modos de operación*/
#define AUTO 0
#define MANUAL 1

/*Definicion de pines*/
#define PIN_0_LECTOR_COLOR 0
#define PIN_1_LECTOR_COLOR 0
#define PIN_2_LECTOR_COLOR 0
#define PIN_3_LECTOR_COLOR 0
#define PIN_SALIDA_LECTOR_COLOR 0
#define PIN_LASER 0
#define PIN_FLAME_DETECTOR 0
#define PIN_LED 0
#define PIN_POTENCIOMETRO 0
#define PIN_PULSADOR 0
#define PIN_SERVO_CINTA 0
#define PIN_SERVO_TOBOGAN 0

/*Tiempos de espera*/
#define TBUSCAR 1000
#define TLLEVAR 1000
#define TDESPACHO 1000

/*Seteo de variables y pines*/
ColorRocklet lectorColor = ColorRocklet(PIN_0_LECTOR_COLOR, PIN_1_LECTOR_COLOR, PIN_2_LECTOR_COLOR, PIN_3_LECTOR_COLOR, PIN_SALIDA_LECTOR_COLOR);
NuestraBarreraLaser barreraLaser = NuestraBarreraLaser(PIN_LASER, PIN_FLAME_DETECTOR);
NuestroLED led = NuestroLED(PIN_LED);
NuestroPotenciometro potenciometro = NuestroPotenciometro(PIN_POTENCIOMETRO);
NuestroPulsador pulsador = NuestroPulsador(PIN_PULSADOR);
NuestroServo servoCinta = NuestroServo(PIN_SERVO_CINTA, NuestroServo::RECEPCION_ST, NuestroServo::CAIDA_ST);
NuestroServo servoTobogan = NuestroServo(PIN_SERVO_TOBOGAN, NuestroServo::ST_1, NuestroServo::ST_6);
int estadoActual;
int modo;
bool barreraDetecta;
unsigned int inicioEsperaServo; //El programa salta loops hasta que el servo se mueva
bool llendo; //El servo está llendo a algún lado
bool sensado; //El sensorColor fue sensado
int color; //Color del rocklet leído

void setup() {
  estadoActual = EN_ESPERA;
  modo = AUTO;
  barreraDetecta = false;
  llendo = false;
}

void loop() {
  /* Condicion de la barrera laser */
  if(estadoActual == EN_ESPERA){
    if(barreraLaser.isOn())
        barreraDetecta = barreraLaser.detecta();
    else{
      barreraLaser.activarBarrera();
      barreraLaser.detecta();
    }
  }

  /* Condicion cambio de estado de EN_ESPERA a BUSCANDO */
  if(barreraDetecta){
    barreraDetecta = false;
    estadoActual = BUSCANDO;
  }

  /* Condicion de el servo cinta  para BUSCANDO*/
  if(estadoActual == BUSCANDO){
    if(!llendo){
      servoCinta.irA(NuestroServo::RECEPCION_ST);
      llendo=true;
      inicioEsperaServo = millis();
    }else if(millis() - inicioEsperaServo >= TBUSCAR){
      llendo=false;
      estadoActual = LLEVANDO;
    }
  }

  /* Condicion del el servo cinta para LLEVANDO */
   if(estadoActual == LLEVANDO){
    if(!llendo){
      servoCinta.irA(NuestroServo::COLOR_ST);
      llendo=true;
      inicioEsperaServo = millis();
    }else if(millis() - inicioEsperaServo >= TLLEVAR){
      llendo=false;
      estadoActual = SENSANDO;
    }
  }

  /* Condicion del sensor color */
  if(estadoActual == SENSANDO){
    if(!sensado){
      color = lectorColor.identificarColor();
      sensado=true;
    }else{
      sensado=false;
      if(modo == AUTO)
        estadoActual = TOBOGAN_A;
      else
        estadoActual = TOBOGAN_M;
    }
  }

  /* Tobogan en modo auto*/
  if(estadoActual == TOBOGAN_A){
    if(modo == AUTO){
      //TODO
    }else
      estadoActual == TOBOGAN_M;
  }

  /* Tobogan en modo manual */
  if(estadoActual == TOBOGAN_M){
    if(modo == MANUAL){
      //TODO
    }else
      estadoActual == TOBOGAN_A;
  }

  /* Despacho */
  if(estadoActual == DESPACHANDO){
    if(!llendo){
      servoCinta.irA(NuestroServo::CAIDA_ST);
      llendo=true;
      inicioEsperaServo = millis();
    }else if(millis() - inicioEsperaServo >= TDESPACHO){
      llendo=false;
      estadoActual = EN_ESPERA;
    }
  }
}
