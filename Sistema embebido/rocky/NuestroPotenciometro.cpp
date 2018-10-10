#include "NuestroPotenciometro.h"

NuestroPotenciometro::NuestroPotenciometro(const int pin){
	this->pin=pin;
	pinMode(this->pin, INPUT); //No recuerdo si este era el formato que se usaba para una entrada analogica, sino corregime vale
}

int NuestroPotenciometro::getPosicion(){
	 int value = analogRead(this->pin);
	 return value;
}

int NuestroPotenciometro::getPosicion256(){
	int value = this->getPosicion * (float)256 / 1024;
	return value;
}