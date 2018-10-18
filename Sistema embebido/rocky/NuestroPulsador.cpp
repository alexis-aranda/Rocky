#include <Arduino.h>
#include "NuestroPulsador.h"

NuestroPulsador::NuestroPulsador(int const pin){
  this->pin=pin;
  pinMode(this->pin, OUTPUT);
}

bool NuestroPulsador::detectaCorto(){
  if(digitalRead(this->pin) == HIGH){
    if(!countingC){
      this->countingC = true;
      this->tmillisC = millis();
      return false;
    }
    if(millis() - this->tmillisC > MINSHORT){
      this->countingC = false;
      this->posibleShort = true;
    }
  }
  else{
    if(posibleShort && millis() - this->tmillisC < MAXSHORT){
      this->countingC = false;
      this->posibleShort = false;
      return true;
    }
    this->countingC = false;
  }
  return false;
}

bool NuestroPulsador::detectaLargo(){
  if(digitalRead(this->pin) == HIGH){
    if(!countingL){
      this->countingL = true;
      this->tmillisL = millis();
      return false;
    }
    if(millis() - this->tmillisL > MAXSHORT){
      this->countingL = false;
      return true;
    }
  }
  else{
    this->countingL = false;
  }
  return false;
}
