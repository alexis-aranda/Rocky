#include <Arduino.h>
#include "NuestroPulsador.h"

NuestroPulsador::NuestroPulsador(int const pin) {
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
}

bool NuestroPulsador::detectaCorto() {
  return this->estado == CORTO;
}

bool NuestroPulsador::detectaLargo() {
  return this->estado == LARGO;
}

/**
   Chequea el estado del pulsador y define si se trata de un pulso
   corto, largo o nada.
*/
void NuestroPulsador::chequear() {
  if (digitalRead(this->pin) == HIGH) { //Si esta pulsado
    if (!counting) { //Si no estaba contando, no estaba pulsado antes
      //Arranco la cuenta ahora
      this->counting = true;
      this->tmillis = millis();
      //Return porque todavia no se que es
    }
    //Si ya venia contando o recien empiezo a contar, espero a que lo suelte para definir que es
  } else {
    if (counting) {
      //Si no esta pulsado pero venia contando, es que recien lo suelta
      this->counting = false;
      //Defino que es
      if (millis() - this->tmillis > MAXSHORT) {
        Serial.println("L");
        this->estado = LARGO;
        return ;
      }
      else if (millis() - this->tmillis > MINSHORT) {
        Serial.println("C");
        this->estado = CORTO;
        return;
      }
      Serial.println(millis() - this->tmillis);
      //Si llego aca es que fue un pulso muy corto para ser algo
    }
  }
  this->estado = NADA;
}
