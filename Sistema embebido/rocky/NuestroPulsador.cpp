#include <Arduino.h>
#include "NuestroPulsador.h"

/**
 * Instancia un pulsador. Recibe el pin por parametro.
 */
NuestroPulsador::NuestroPulsador(int const pin) {
  this->pin = pin;
}

/**
 * Devuelve true si lo ultimo que detecto fue un pulso corto.
 * Si fue largo o nada, devuelve false.
 */
bool NuestroPulsador::detectaCorto() {
  return this->estado == CORTO;
}

/**
 * Devuelve true si lo ultimo que detecto fue un pulso largo.
 * Si fue corto o nada, devuelve false.
 */
bool NuestroPulsador::detectaLargo() {
  return this->estado == LARGO;
}

/**
 * Chequea el estado del pulsador y define si se trata de un pulso
 * corto, largo o nada.
 * Debe ser llamado en cada vuelta del loop principal.
*/
void NuestroPulsador::chequear() {
    if (digitalRead(this->pin) == HIGH) { //Si esta pulsado
        if (!counting) { //Si no estaba contando, no estaba pulsado antes
            //Arranco la cuenta ahora
            this->counting = true;
            this->tmillis = millis();
            //Return porque todavia no se que es
        }
        //Si ya venia contando, espero a que lo suelte para definir que es
        this->estado = NADA;
        return;
    }
    
    //Si no esta pulsado
    if (counting) {
        //Si no esta pulsado pero venia contando, es que recien lo suelta
        this->counting = false;
        //Defino que es segun el tiempo que estuvo pulsado
        if (millis() - this->tmillis > MAXSHORT) {
            this->estado = LARGO;
            return ;
        }
        else if (millis() - this->tmillis > MINSHORT) {
            this->estado = CORTO;
            return;
        }
        //Si llego aca es que fue un pulso muy corto para ser algo
    }
    
    this->estado = NADA;
}
