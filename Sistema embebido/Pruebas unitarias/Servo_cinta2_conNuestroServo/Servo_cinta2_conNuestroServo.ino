#include "nuestroServo.h"

const int max = 220; //Prueba servo negro antes del limite
const int min = 60; //Servo negro

NuestroServo serv(11, min, max);

void setup() {
}

void loop() {
  serv.irA(NuestroServo::RECEPCION_ST);
  delay(2000);
  serv.irA(NuestroServo::COLOR_ST);
  delay(1500);
  serv.irA(NuestroServo::CAIDA_ST);
  delay(1500);
}
