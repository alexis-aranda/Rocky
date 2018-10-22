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

void setup() {
  /*Seteo de variables y pines*/
  ColorRocklet lectorColor = ColorRocklet(PIN_0_LECTOR_COLOR, PIN_1_LECTOR_COLOR, PIN_2_LECTOR_COLOR, PIN_3_LECTOR_COLOR, PIN_SALIDA_LECTOR_COLOR);
  NuestraBarreraLaser barreraLaser = NuestraBarreraLaser(PIN_LASER, PIN_FLAME_DETECTOR);
  NuestroLED led = NuestroLED(PIN_LED);
  NuestroPotenciometro potenciometro = NuestroPotenciometro(PIN_POTENCIOMETRO);
  NuestroPulsador pulsador = NuestroPulsador(PIN_PULSADOR);
  NuestroServo servoCinta = NuestroServo(PIN_SERVO_CINTA, NuestroServo::RECEPCION_ST, NuestroServo::CAIDA_ST);
  NuestroServo servoTobogan = NuestroServo(PIN_SERVO_TOBOGAN, NuestroServo::ST_1, NuestroServo::ST_6);
  int estadoActual = EN_ESPERA;
  int modo = AUTO;
}

void loop() {
  /*if(estadoActual == EN_ESPERA){
    
  }*/
}
