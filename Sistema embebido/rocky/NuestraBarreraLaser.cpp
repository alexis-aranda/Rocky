#include <Arduino.h>
#include "NuestraBarreraLaser.h"

NuestraBarreraLaser::NuestraBarreraLaser(const int pinLaser, const int pinFD){
  this->pinLaser=pinLaser;
  this->pinFD=pinFD;
  pinMode(this->pinLaser, OUTPUT);
  //digitalWrite(this->pinLaser, LOW); //Asumo que está así por defecto
  //pinMode(this->pinFD, INPUT);
}

void NuestraBarreraLaser::activarBarrera(){
  if (isOn()) return; //Already on
  digitalWrite(this->pinLaser, HIGH);
  this->prendido = true;
}

void NuestraBarreraLaser::desactivarBarrera(){
  if (!isOn()) return; //Already off
  digitalWrite(this->pinLaser, LOW);
  this->prendido = false;
}

bool NuestraBarreraLaser::isOn(){
  return this->prendido;
}

bool NuestraBarreraLaser::detecta(){
  if(digitalRead(this->pinFD) == HIGH){ //Por ahí por formato quede bueno preguntar si está prendido tambien
    if(!counting){
      this->counting = true;
      this->tmillis = millis();
      return false;
    }
    if(millis() - this->tmillis > TRESPUESTA){
      this->counting = false;
      return true;
    }
  }
  else{
    this->counting = false;
  }
  return false;
}
