#include "nuestroServo.h"

const int max = 254; //Servo azul
const int min = 50; //Servo azul
NuestroServo serv(10, min, max);

void setup() {
}

void loop() {
  serv.irA(NuestroServo::ST_1);
  delay(1500);
  serv.irA(NuestroServo::ST_2);
  delay(1500);
  serv.irA(NuestroServo::ST_3);
  delay(1500);
  serv.irA(NuestroServo::ST_4);
  delay(1500);
  serv.irA(NuestroServo::ST_5);
  delay(1500);
  serv.irA(NuestroServo::ST_6);
  delay(1500);
}
