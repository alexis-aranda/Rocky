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

/*Tiempos de espera*/
#define TBUSCAR 1000
#define TLLEVAR 1000
#define TDESPACHO 1000
#define TACOMODAR 1000

/* Definicion de limites para servos*/
#define MAX_NEGRO 220
#define MIN_NEGRO 60
#define MAX_AZUL 254
#define MIN_AZUL 50

/*Seteo de variables y pines*/
ColorRocklet lectorColor = ColorRocklet(PIN_0_LECTOR_COLOR, PIN_1_LECTOR_COLOR, PIN_2_LECTOR_COLOR, PIN_3_LECTOR_COLOR, PIN_SALIDA_LECTOR_COLOR);
NuestraBarreraLaser barreraLaser = NuestraBarreraLaser(PIN_LASER, PIN_FLAME_DETECTOR);
NuestroLED led = NuestroLED(PIN_LED);
NuestroPotenciometro potenciometro = NuestroPotenciometro(PIN_POTENCIOMETRO);
NuestroPulsador pulsador = NuestroPulsador(PIN_PULSADOR);
NuestroServo servoCinta = NuestroServo(PIN_SERVO_CINTA, MIN_NEGRO, MAX_NEGRO);
NuestroServo servoTobogan = NuestroServo(PIN_SERVO_TOBOGAN, MIN_AZUL, MAX_AZUL);
int estadoActual;
int modo;
bool barreraDetecta;
unsigned int inicioEsperaServo; //El programa salta loops hasta que el servo se mueva
bool llendo; //El servo está llendo a algún lado
bool sensado; //El sensorColor fue sensado
int color; //Color del rocklet leído
int posPotenciometro; // Posicion leida del potenciometro

void setup() {
  estadoActual = EN_ESPERA;
  modo = AUTO;
  barreraDetecta = false;
  llendo = false;
}

void loop() {
  switch(estadoActual){
    case EN_ESPERA:/* Condicion de la barrera laser */
        if(barreraLaser.isOn())
            barreraDetecta = barreraLaser.detecta();
        else{
          barreraLaser.activarBarrera(); //Cuando la desactivamos?
          barreraLaser.detecta();
        }
        /* Condicion cambio de estado de EN_ESPERA a BUSCANDO */
        if(barreraDetecta){
          barreraDetecta = false;
          estadoActual = BUSCANDO;
          setearLED();
          barreraLaser.desactivarBarrera();
        }
        break;
    case BUSCANDO:/* Condicion de el servo cinta  para BUSCANDO*/
        if(!llendo){
          servoCinta.irA(NuestroServo::RECEPCION_ST);
          llendo=true;
          inicioEsperaServo = millis();
        }else if(millis() - inicioEsperaServo >= TBUSCAR){
          llendo=false;
          estadoActual = LLEVANDO;
          setearLED();
        }
        break;
    case LLEVANDO:/* Condicion del el servo cinta para LLEVANDO */
        if(!llendo){
          servoCinta.irA(NuestroServo::COLOR_ST);
          llendo=true;
          inicioEsperaServo = millis();
        }else if(millis() - inicioEsperaServo >= TLLEVAR){
          llendo=false;
          estadoActual = SENSANDO;
          setearLED();
        }
    case SENSANDO:/* Condicion del sensor color */
        //Testear. Nuestra prueba tenia delays entre el sensado de un color y el otro por estabilidad.
        // Puede que tengamos que partirlo en varios loops
        if(!sensado){
          color = lectorColor.identificarColor();
          sensado=true;
          /*
           //planteo una alternativa usando un contador de lecturas
           color = lectorColor.identificarColor();
           if(!ColorRocklet::NO_IDENTIFICADO){
            sensado=true;
           }
          */
        }else{
          sensado=false;
          if(modo == AUTO){
            estadoActual = TOBOGAN_A;
            setearLED();
          }else{
            estadoActual = TOBOGAN_M;
            setearLED();
          }
        }
        break;
    case TOBOGAN_A:/* Tobogan en modo auto*/
        if(modo == AUTO){
          if(!llendo){
            switch( color )
            {
              case ColorRocklet::VERDE:
                servoTobogan.irA(NuestroServo::ST_1);
               break;
              case ColorRocklet::AZUL:
                servoTobogan.irA(NuestroServo::ST_2);
                break;
              case ColorRocklet::ROJO:
                servoTobogan.irA(NuestroServo::ST_3);
                break;
              case ColorRocklet::NARANJA:
                servoTobogan.irA(NuestroServo::ST_4);
                break;
              case ColorRocklet::AMARILLO:
                servoTobogan.irA(NuestroServo::ST_5);
               break;
             case ColorRocklet::MARRON:
                servoTobogan.irA(NuestroServo::ST_6);
                break;
              default:
                // qué hacemos cuando es no identificado??  
                break;
            }
            llendo = true;
            inicioEsperaServo = millis();
          }else if(millis() - inicioEsperaServo >= TACOMODAR){
            llendo=false;
            estadoActual = DESPACHANDO;
            setearLED();
          }
        }else{
          estadoActual == TOBOGAN_M;
          setearLED();
        }
    case TOBOGAN_M:/* Tobogan en modo manual (verificar)*/
        if(modo == MANUAL){
          if(!pulsador.detectaCorto()){
            posPotenciometro = potenciometro.getPosicion256();
            servoTobogan.irA(posPotenciometro);
          }else{ 
            estadoActual = DESPACHANDO;
            setearLED();
          }
        }else{
          estadoActual = TOBOGAN_A;
          setearLED();
        }
    case DESPACHANDO:/* Despacho */
        if(!llendo){
          servoCinta.irA(NuestroServo::CAIDA_ST);
          llendo=true;
          inicioEsperaServo = millis();
        }else if(millis() - inicioEsperaServo >= TDESPACHO){
          llendo=false;
          estadoActual = EN_ESPERA;
          setearLED();
        }
  }
  
  /* Checkeo si cambio de modo (verificar)*/ 
  if(pulsador.detectaLargo()){
    if(modo == AUTO)
      modo = MANUAL;
    else
      modo = AUTO;
  }
  
  led.activar(posPotenciometro);
  
}

//creo una función para setear el LED que es llamada en cada cambio de estado
void setearLED(){
  /* Seteo de LED */
  if( modo == MANUAL ){
    if( estadoActual == TOBOGAN_M ){
      led.setModo(NuestroLED::INTENSIDAD_VARIABLE);
    }else
      led.setModo(NuestroLED::PRENDE_APAGA);
  }else{
    if( estadoActual == EN_ESPERA )
      led.setModo(NuestroLED::SOFT_PWM);
    else{ 
      if(estadoActual == TOBOGAN_A || estadoActual == DESPACHANDO)
         led.setModo(NuestroLED::SIEMPRE_PRENDIDO);
      else //BUSCANDO || LLEVANDO || SENSANDO
        led.setModo(NuestroLED::SIEMPRE_APAGADO);
    }
  }
}

/*
void loop() {
  if(estadoActual == EN_ESPERA){
    if(barreraLaser.isOn())
        barreraDetecta = barreraLaser.detecta();
    else{
      barreraLaser.activarBarrera(); //Cuando la desactivamos?
      barreraLaser.detecta();
    }
    // Condicion cambio de estado de EN_ESPERA a BUSCANDO 
    if(barreraDetecta){
      barreraDetecta = false;
      estadoActual = BUSCANDO;
      desactivarBarrera();
    }
  }

  // Condicion de el servo cinta  para BUSCANDO
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

  // Condicion del el servo cinta para LLEVANDO
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

  //Testear. Nuestra prueba tenia delays entre el sensado de un color y el otro por estabilidad.
  // Puede que tengamos que partirlo en varios loops
  // Condicion del sensor color
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
  
  // Checkeo si cambio de modo (verificar)
  if(pulsador.detectaLargo()){
    if(modo == AUTO)
      modo = MANUAL;
    else
      modo = AUTO;
  }
    
  // Tobogan en modo auto
  if(estadoActual == TOBOGAN_A){
    if(modo == AUTO){
      if(!llendo){
        switch( color )
        {
          case ColorRocklet::VERDE:
            servoTobogan.irA(NuestroServo::ST_1);
           break;
          case ColorRocklet::AZUL:
            servoTobogan.irA(NuestroServo::ST_2);
            break;
          case ColorRocklet::ROJO:
            servoTobogan.irA(NuestroServo::ST_3);
            break;
          case ColorRocklet::NARANJA:
            servoTobogan.irA(NuestroServo::ST_4);
            break;
          case ColorRocklet::AMARILLO:
            servoTobogan.irA(NuestroServo::ST_5);
           break;
         case ColorRocklet::MARRON:
            servoTobogan.irA(NuestroServo::ST_6);
            break;
          default:
            // qué hacemos cuando es no identificado??  
            break;
        }
        llendo = true;
        inicioEsperaServo = millis();
      }else if(millis() - inicioEsperaServo >= TACOMODAR){
        llendo=false;
        estadoActual = DESPACHANDO;
      }
    }else
      estadoActual == TOBOGAN_M;
  }

  // Tobogan en modo manual (verificar)
  if(estadoActual == TOBOGAN_M){
    if(modo == MANUAL){
      if(!pulsador.detectaCorto()){
        posPotenciometro = potenciometro.getPosicion256();
        servoTobogan.irA(posPotenciometro);
      }else{ 
        estadoActual = DESPACHANDO;
      }
    }else{
      estadoActual = TOBOGAN_A;
    }
  }

  // Despacho
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

  // Vamos a setear el modo del LED en cada loop?
  // Seteo de LED
  if( modo == MANUAL ){
    if( estadoActual == TOBOGAN_M ){
      led.setModo(NuestroLED::INTENSIDAD_VARIABLE);
      //intensidadPWMManual = 
    }else
      led.setModo(NuestroLED::PRENDE_APAGA);
  }else{
    // me rechazaba los or en los cases.
    // switch (estadoActual) {
    //  case EN_ESPERA:
    //   led.setModo(NuestroLED::SOFT_PWM);
    //   break;
    //  case (TOBOGAN_A || DESPACHANDO):
    //   led.setModo(NuestroLED::SIEMPRE_PRENDIDO); //deberia preguntar si ya esta en ese modo con el getModo antes de cambiarlo?
    //   break;
    //  case (BUSCANDO || LLEVANDO || SENSANDO):  
    //   led.setModo(NuestroLED::SIEMPRE_APAGADO);
    //   break;
    // }
    
    if( estadoActual == EN_ESPERA )
      led.setModo(NuestroLED::SOFT_PWM);
    else{ 
      if(estadoActual == TOBOGAN_A || estadoActual == DESPACHANDO)
         led.setModo(NuestroLED::SIEMPRE_PRENDIDO);
      else //BUSCANDO || LLEVANDO || SENSANDO
        led.setModo(NuestroLED::SIEMPRE_APAGADO);
    }
  }
  
  led.activar(posPotenciometro);
}
*/
