#include <Arduino.h>
#include "NuestroLED.h"

NuestroLED::NuestroLED(const int pin){
	this->pin=pin;
	pinMode(this->pin, OUTPUT);
}

NuestroLED::NuestroLED(const int pin, const int modo){
	this->pin=pin;
  pinMode(this->pin, OUTPUT);
	this->modo=modo;
}

void NuestroLED::setModo(const int modo){
    this->modo=modo;
    /* Probablemente necesite revisar si necesito resetear alguna variable acá */
}

int NuestroLED::getModo(){
    return this->modo;
}

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
            on();
            break;
         case SIEMPRE_APAGADO:
            off();
            break;  
         default:  
            break; //IDK
      }  
}

void NuestroLED::setIntensidadPWM(){
  analogWrite(this->pin,this->intensidad);
}

void NuestroLED::setIntensidadBoolON(){
  digitalWrite(this->pin,HIGH);
}

void NuestroLED::setIntensidadBoolOFF(){
  digitalWrite(this->pin,LOW);
}

void NuestroLED::onOff(){
  
}

void NuestroLED::manual(const int intensidadPWMManual){
  this->intensidad = intensidadPWMManual;
  
}

void NuestroLED::soft(){
  
}

void NuestroLED::on(){
  
}

void NuestroLED::off(){
  
}
