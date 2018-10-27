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

/**
 * Recibe un valor entre 0 y 255 y mueve el servo proporcionalmente
 * entre sus valores minimo y maximo. Si el valor no esta entre 0 o 1023
 * lo ignora.
*/
void NuestroServo::irAAnalogico(int posicion)
{
  int pos = map(posicion, 0, 1023, this->limInf, this->limSup);
  if(pos >= this->limInf && pos <= this->limSup)
     analogWrite(this->pin, pos);
}
