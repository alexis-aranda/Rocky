#include <Arduino.h>
#include "NuestraBarreraLaser.h"

/**
 * Instancia una barrera laser. Recibe el pin del laser y del detector
 * de flama por parametro.
 */
NuestraBarreraLaser::NuestraBarreraLaser(const int pinLaser, const int pinFD){
  this->pinLaser=pinLaser;
  this->pinFD=pinFD;
  pinMode(this->pinLaser, OUTPUT);
}

/**
 * Prende el laser de la barrera.
 */
void NuestraBarreraLaser::activarBarrera(){
  if (isOn()) return; //Already on
  digitalWrite(this->pinLaser, HIGH);
  this->prendido = true;
}

/**
 * Apaga el laser de la barrera
 */
void NuestraBarreraLaser::desactivarBarrera(){
  if (!isOn()) return; //Already off
  digitalWrite(this->pinLaser, LOW);
  this->prendido = false;
}

/**
 * Devuelve true si la barrera esta activada,
 */
bool NuestraBarreraLaser::isOn(){
  return this->prendido;
}

/**
 * Devuelve true si hay un rocklet, false si no
 */
bool NuestraBarreraLaser::detecta(){
  //Leer HIGH es que el laser llega, por lo tanto no hay rocklet
  //Leer LOW es que el laser no llega, algo lo esta tapando (probablemente un rocklet)
  return digitalRead(this->pinFD) == LOW;
}
