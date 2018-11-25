#include "nuestroServo.h"

const int max = 200; //Prueba servo negro antes del limite
const int min = 20; //Servo negro
int pos;

NuestroServo serv(11, min, max);

void setup() {
  Serial.begin(9600);
}
/*
void loop() {
  serv.irA(50);
  Serial.print("En 50: ");
  delay(1000);
  serv.irA(100);
  Serial.print("En 100: ");
  delay(1000);
  serv.irA(180);
  Serial.print("En 180: ");
  delay(1000);
}
*/
void loop() {
  if(pos = serv.irA(NuestroServo::RECEPCION_ST)){
    Serial.print("ST recepción: ");
    Serial.println(pos);
    delay(2000);
  }
  if(pos = serv.irA(NuestroServo::COLOR_ST)){
    Serial.print("ST color: ");
    Serial.println(pos);
    delay(2000);
  }
  if(pos = serv.irA(NuestroServo::CAIDA_ST)){
    Serial.print("ST caída: ");
    Serial.println(pos);
    delay(2000);
  }
}
