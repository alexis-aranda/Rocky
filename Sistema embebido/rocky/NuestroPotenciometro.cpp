#include <Arduino.h>
#include "NuestroPotenciometro.h"

NuestroPotenciometro::NuestroPotenciometro(const int pin){
	this->pin=pin;
}

int NuestroPotenciometro::getPosicion(){
	 int value = analogRead(this->pin);
	 return value;
}

int NuestroPotenciometro::getPosicion256(){
	int value = this->getPosicion() * (float)256 / 1024;
	return value;
}
