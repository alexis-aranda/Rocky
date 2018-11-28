#include "NuestraBarreraLaser.h"

NuestraBarreraLaser barrera(2,3);
int pinLed = 13;

void setup(){
    Serial.begin(9600);
  barrera.activarBarrera();
  pinMode(pinLed, OUTPUT);
}

void loop(){
  if(!barrera.detecta()){
    digitalWrite(pinLed, HIGH);
    Serial.println("H");
  }
   else{
    Serial.println("L");
    digitalWrite(pinLed, LOW);
   }
}
