#include "NuestraBarreraLaser.h"

NuestraBarreraLaser barrera(2,3);
int pinLed = 2;

void setup(){
  barrera.activarBarrera();
  pinMode(pinLed, OUTPUT);
}

void loop(){
  if(barrera.detecta())
    digitalWrite(pinLed, HIGH);
   else
    digitalWrite(pinLed, LOW);
}
