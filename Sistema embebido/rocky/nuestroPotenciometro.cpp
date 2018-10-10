#include <Arduino.h>
#include "nuestroPotenciometro.h"

nuestroPotenciometro::nuestroPotenciometro(const int pin){
	this->pin=pin;
	//pinMode(this->pin, INPUT); //No hace falta esta linea, los pines son de entrada mientras no se especifique otra cosa
}

int nuestroPotenciometro::getPosicion(){
	 int value = analogRead(this->pin);
	 return value;
}

int nuestroPotenciometro::getPosicion256(){
	int value = this->getPosicion() * (float)256 / 1024;
	return value;
}
