#include <Servo.h>

//const int max = 240; //Servo negro
const int max = 220; //Prueba servo negro antes del limite
const int min = 50; //Servo negro
const int delta = 5;

int pos1;
int pos2;
int pos3;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pos1 = map(0, 0, 100, min, max);
  pos2 = map(50, 0, 100, min, max);
  pos3 = map(100, 0, 100, min, max);
}

void loop() {
  analogWrite(9, pos1);
  delay(1500);
  analogWrite(9, pos2);
  delay(1500);
  analogWrite(9, pos3);
  delay(1500);
}
