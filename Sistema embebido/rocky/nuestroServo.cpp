/*
 * nuestroServo.h - Biblioteca para inicializar y mover los servos
 */
#include <Arduino.h>
#include "nuestroServo.h"

/** 
* Instancia un servo en el pin indicado y setea entre que valores PWM puede trabajar 
*/
NuestroServo::NuestroServo(int pin, int limiteInf, int limiteSup)
{
  this->pin = pin;
  this->limInf = limiteInf;
  this->limSup = limiteSup;
  pinMode(this->pin, OUTPUT);
}

/* 
* Indica al servo que se mueva hasta la posicion recibida por parametro, 
* siempre que esta se encuentre entre los limites setados.
* La posicion debe pasarse en valores PWM.
*/
void NuestroServo::irA(int pos)
{
  if(pos >= this->limInf && pos <= this->limSup)
     analogWrite(this->pin, pos);
}

/**
 * Recibe un valor entre 0 y 1023 y mueve el servo proporcionalmente
 * entre sus valores minimo y maximo. Si el valor no esta entre 0 o 1023
 * lo ignora.
*/
void NuestroServo::irAAnalogico(int posicion)
{
  int pos = map(posicion, 0, 1023, this->limInf, this->limSup);
  if(pos >= this->limInf && pos <= this->limSup)
     analogWrite(this->pin, pos);
}

void NuestroServo::irA256(int posicion)
{
      int pos = map(posicion, 0, 255, this->limInf, this->limSup);
      if(pos >= this->limInf && pos <= this->limSup)
         analogWrite(this->pin, pos);
}
