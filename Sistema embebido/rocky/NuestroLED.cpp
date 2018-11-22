#include <Arduino.h>
#include "NuestroLED.h"

/**
 * Instancia un LED. Recibe el pin por parametro.
 */
NuestroLED::NuestroLED(const int pin){
    this->pin=pin;
    pinMode(this->pin, OUTPUT);
    
    this->modo=SOFT_PWM;
    reiniciarAtributos();
}

/**
 * Cambia el modo del LED
 */
void NuestroLED::setModo(const int modo){
    this->modo=modo;
    reiniciarAtributos();
}

/**
 * Pone los atributos isON, enSubida, start e intensidad
 * en sus valores por default.
 */
void NuestroLED::reiniciarAtributos(){
    this->isON=true;
    this->enSubida=true;
    this->start=millis();
    this->intensidad=0;
}

/**
 * El LED es notificado de una nueva vuelta en el loop principal y 
 * responde segun el modo en que se encuentra.
 */
void NuestroLED::activar(const int intensidadPWMManual){
    switch ( this->modo )  
    {  
     case PRENDE_APAGA:
        onOff();
        break;  
     case INTENSIDAD_VARIABLE:
        manual(intensidadPWMManual);
        break;
     case SOFT_PWM:
        soft();
        break;
     case SIEMPRE_PRENDIDO:
        this->isON = true;
        setIntensidadBoolON();
        break;
     case SIEMPRE_APAGADO:
        this->isON=false;
        setIntensidadBoolOFF();
        break;
    }  
}

/**
 * Escribe un nuevo valor PWM en el pin del LED
 */
void NuestroLED::setIntensidadPWM(){
  analogWrite(this->pin,this->intensidad);
}

/**
 * Escribe el valor logico HIGH alta en el pin del LED
 */
void NuestroLED::setIntensidadBoolON(){
  digitalWrite(this->pin,HIGH);
}

/**
 * Escribe el valor logico LOW en el pin del LED
 */
void NuestroLED::setIntensidadBoolOFF(){
  digitalWrite(this->pin,LOW);
}

/**
 * Se fija si ya paso un segundo y cambia entre prendido y apagado.
 */
void NuestroLED::onOff(){
  if(this->isON && millis() - this->start > 1000){ //Cuando pase un segundo, si está prendido, se apaga
    this->isON = false;
    setIntensidadBoolOFF();
  }
  if(!(this->isON) && millis() - this->start > 1000){ //Cuando pase otro segundo, si está apagado, se prende
    this->isON = true;
    setIntensidadBoolON();
  }
}

/**
 * Recibe y escribe una nueva intensidad PWM
 */
void NuestroLED::manual(const int intensidadPWMManual){
  this->intensidad = map(intensidadPWMManual,0,1023,0,255);
  setIntensidadPWM();
}

/**
 * Sube y baja la intensidad levemente
 */
void NuestroLED::soft(){
    if(this->enSubida){
        if(this->intensidad<LIM_SUP){
            this->intensidad++;
            setIntensidadPWM();
        }else
            this->enSubida = false;
    } else{ //No estoy en subida
        if(this->intensidad>LIM_INF){
            this->intensidad--;
            setIntensidadPWM();
        }else
            this->enSubida = true;
    }
}
