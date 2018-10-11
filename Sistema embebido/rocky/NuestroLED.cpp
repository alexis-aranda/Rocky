#include <Arduino.h>
#include "NuestroLED.h"

NuestroLED::NuestroLED(const int pin){
	this->pin=pin;
	pinMode(this->pin, OUTPUT);
}

NuestroLED::NuestroLED(const int pin, const int modo){
	this->pin=pin;
  pinMode(this->pin, OUTPUT);
	this->modo=modo;
}
