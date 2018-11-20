#include <Arduino.h>
#include "NuestroPotenciometro.h"

/**
 * Instancia un potenciometro. Recibe el pin como parametro.
 */
NuestroPotenciometro::NuestroPotenciometro(const int pin){
	this->pin=pin;
}

/**
 * Devuelve la posicion actual, un numero de 0 a 1023.
 */
int NuestroPotenciometro::getPosicion(){
	 int value = analogRead(this->pin);
	 return value;
}
