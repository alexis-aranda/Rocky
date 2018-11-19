#include <SoftwareSerial.h>
#include "nuestroServo.h"
#include "NuestroPotenciometro.h"
#include "NuestroLED.h"
#include "ColorRocklet.h"
#include "NuestraBarreraLaser.h"
#include "NuestroPulsador.h"

#define CANT_COLORES 6

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
#define PIN_0_LECTOR_COLOR 4
#define PIN_1_LECTOR_COLOR 5
#define PIN_2_LECTOR_COLOR 6
#define PIN_3_LECTOR_COLOR 7
#define PIN_SALIDA_LECTOR_COLOR 8
#define PIN_LASER 2
#define PIN_FLAME_DETECTOR 3
#define PIN_LED 9
#define PIN_POTENCIOMETRO 0
#define PIN_PULSADOR 12
#define PIN_SERVO_CINTA 11
#define PIN_SERVO_TOBOGAN 10
#define PIN_BT_RX 0
#define PIN_BT_TX 1

/*Tiempos de espera*/
#define TBUSCAR 1000
#define TLLEVAR 1000
#define TDESPACHO 1000
#define TACOMODAR 1000

/* Definicion de limites para servos*/
#define MAX_NEGRO 190
#define MIN_NEGRO 0
#define MAX_AZUL 254
#define MIN_AZUL 50

/*Seteo de variables y pines*/
ColorRocklet lectorColor = ColorRocklet(
    PIN_0_LECTOR_COLOR, PIN_1_LECTOR_COLOR, 
    PIN_2_LECTOR_COLOR, PIN_3_LECTOR_COLOR, 
    PIN_SALIDA_LECTOR_COLOR); //Sensor de color
NuestraBarreraLaser barreraLaser = NuestraBarreraLaser(
    PIN_LASER, 
    PIN_FLAME_DETECTOR); //Barrera con laser y detector de flama
NuestroLED led = NuestroLED(PIN_LED);
NuestroPotenciometro potenciometro = NuestroPotenciometro(PIN_POTENCIOMETRO);
NuestroPulsador pulsador = NuestroPulsador(PIN_PULSADOR);
NuestroServo servoCinta = NuestroServo(
    PIN_SERVO_CINTA, 
    MIN_NEGRO, MAX_NEGRO); //Servo de estaciones RECEPCION, COLOR y CAIDA
NuestroServo servoTobogan = NuestroServo(
    PIN_SERVO_TOBOGAN, 
    MIN_AZUL, MAX_AZUL); //Servo de estaciones de colores
SoftwareSerial bluetooth(PIN_BT_RX, PIN_BT_TX); //Modulo bluetooth

int estadoActual;
int modo; //Automatico o manual
unsigned int inicioEsperaServo; //Tiempo en que el servo comenzo a moverse, para saber hasta que tiempo esperar
bool llendo; //El servo está llendo a algún lado
bool sensado; //El sensorColor fue sensado
int color; //Color del rocklet leído
int cantColores[CANT_COLORES] = {0}; //Contador de cantidad de cada color
const int estaciones[] = {
    NuestroServo::ST_1, NuestroServo::ST_2, 
    NuestroServo::ST_3, NuestroServo::ST_4, 
    NuestroServo::ST_5, NuestroServo::ST_6 }; //Estaciones segun el color
int posPotenciometro; // Posicion leida del potenciometro

void setup() {
  //Serial.begin(9600);
  bluetooth.begin(9600);
  estadoActual = EN_ESPERA;
  modo = AUTO;
  llendo = false;
}

void loop() {
    loDeSiempre(); //Chequea y hace todo lo que tiene que hacer en cada loop

    //Decide qué más ejecutar según el estado actual
  switch (estadoActual) {
    case EN_ESPERA:/* Condicion de la barrera laser */
      barreraLaser.activarBarrera();
      /* Condicion cambio de estado de EN_ESPERA a BUSCANDO */
      if (barreraLaser.detecta()) {
        estadoActual = BUSCANDO;
        //Serial.println("BUSCANDO");
        setearLED();
        barreraLaser.desactivarBarrera();
      }
      break;
    case BUSCANDO:/* Condicion de el servo cinta  para BUSCANDO*/
      if (!llendo) {
        servoCinta.irA(NuestroServo::RECEPCION_ST);
        llendo = true;
        inicioEsperaServo = millis();
      } else if (millis() - inicioEsperaServo >= TBUSCAR) {
        llendo = false;
        estadoActual = LLEVANDO;
        Serial.println("LLEVANDO");
        setearLED();
      }
      break;
    case LLEVANDO:/* Condicion del el servo cinta para LLEVANDO */
      if (!llendo) {
        servoCinta.irA(NuestroServo::COLOR_ST);
        llendo = true;
        inicioEsperaServo = millis();
      } else if (millis() - inicioEsperaServo >= TLLEVAR) {
        llendo = false;
        estadoActual = SENSANDO;
        Serial.println("SENSANDO");
        setearLED();
      }
      break;
    case SENSANDO:/* Condicion del sensor color */

      if (!sensado) { //Si todavia no lo identifique, hago otra lectura
        lectorColor.prenderSensor();
        sensado = lectorColor.hacerLectura();

      } else { //Si ya lo identifique
        sensado = false;
        lectorColor.apagarSensor();

        //Obtengo el color
        color = lectorColor.getColor();
        if (color != ColorRocklet::NO_IDENTIFICADO)
            cantColores[color]++;
        reportarColores();
        
        for(int i = 0; i < 6; i++){
            Serial.print(cantColores[i]);
            Serial.print(" ");
        }
        Serial.println();
        Serial.println(modo);

        //Paso al tobogan segun el estado
        if (modo == AUTO) {
          estadoActual = TOBOGAN_A;
          Serial.println("TOBOGAN_A");
          setearLED();
        } else {
          estadoActual = TOBOGAN_M;
          Serial.println("TOBOGAN_M");
          setearLED();
        }
      }
      break;
    case TOBOGAN_A:/* Tobogan en modo auto*/
      if (modo == AUTO) {
        if (!llendo) {
            //Los colores están del 0 al 5, para servir de indices en los vectores
            if(color != ColorRocklet::NO_IDENTIFICADO) //Si reconoci un color
                servoTobogan.irA( estaciones[color] );
            else { //Sino lo mando a alguna estacion definida para los no reconocidos
                servoTobogan.irA(NuestroServo::ST_3);
                Serial.println("Sin color");
            }
          llendo = true;
          inicioEsperaServo = millis();
        } else if (millis() - inicioEsperaServo >= TACOMODAR) {
          llendo = false;
          estadoActual = DESPACHANDO;
          Serial.println("DESPACHANDO");
          setearLED();
        }
      } else {
        estadoActual = TOBOGAN_M;
        Serial.println("TOBOGAN_M1");
        setearLED();
      }
      break;
    case TOBOGAN_M:/* Tobogan en modo manual (verificar)*/
      if (modo == MANUAL) {
        if (!pulsador.detectaCorto()) {
          posPotenciometro = potenciometro.getPosicion();
          servoTobogan.irAAnalogico(posPotenciometro);
        } else {
          estadoActual = DESPACHANDO;
          Serial.println("DESPACHANDO");
          setearLED();
        }
      } else {
        estadoActual = TOBOGAN_A;
        Serial.println("TOBOGAN_A1");
        setearLED();
      }
      break;
    case DESPACHANDO:/* Despacho */
      if (!llendo) {
        servoCinta.irA(NuestroServo::CAIDA_ST);
        llendo = true;
        inicioEsperaServo = millis();
      } else if (millis() - inicioEsperaServo >= TDESPACHO) {
        llendo = false;
        estadoActual = EN_ESPERA;
        Serial.println("EN_ESPERA");
        setearLED();
      }
  }
}

//creo una función para setear el LED que es llamada en cada cambio de estado
void setearLED() {
  /* Seteo de LED */
  if ( modo == MANUAL ) {
    if ( estadoActual == TOBOGAN_M ) {
      led.setModo(NuestroLED::INTENSIDAD_VARIABLE);
    } else
      led.setModo(NuestroLED::PRENDE_APAGA);
  } else {
    if ( estadoActual == EN_ESPERA )
      led.setModo(NuestroLED::SOFT_PWM);
    else {
      if (estadoActual == TOBOGAN_A || estadoActual == DESPACHANDO)
        led.setModo(NuestroLED::SIEMPRE_PRENDIDO);
      else //BUSCANDO || LLEVANDO || SENSANDO
        led.setModo(NuestroLED::SIEMPRE_APAGADO);
    }
  }
}

void loDeSiempre(){
    //Chequeo el bluetooth
    if(bluetooth.available()){
        //TODO recibir datos
    }
    
  /* Checkeo el pulsador */
  pulsador.chequear();
  //Si hace falta, cambio de modo
  if (pulsador.detectaLargo()) {
    if (modo == AUTO){
      modo = MANUAL;
      //Serial.println("M");
    }
    else{
      modo = AUTO;
      //Serial.println("A");
    }
  }

  //Mando un tick al LED
  led.activar(posPotenciometro);
}

void reportarColores(){
    bluetooth.print("#");
    bluetooth.print(cantColores[0]);
    for(int i = 1; i < CANT_COLORES; i++){
        bluetooth.print("-");
        bluetooth.print(cantColores[i]);
    }
    bluetooth.println();
}
