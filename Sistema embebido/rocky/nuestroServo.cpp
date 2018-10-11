/*
 * nuestroServo.h - Biblioteca para inicializar y mover los servos
 */
#include <Arduino.h>
#include "nuestroServo.h"

NuestroServo::NuestroServo(int pin, int limiteInf, int limiteSup)
{
  this->pin = pin;
  this->limInf = limiteInf;
  this->limSup = limiteSup;
  pinMode(this->pin, OUTPUT);
}

void NuestroServo::irA(int pos)
{
  if(pos >= this->limInf && pos <= this->limSup)
     analogWrite(this->pin, pos);
}
