
const int max = 254; //Servo azul
const int min = 50; //Servo azul
const int delta = 5;

int pos1;
int pos2;
int pos3;
int pos4;
int pos5;
int pos6;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pos1 = 50; //map(0, 0, 180, min, max);
  pos2 = 84; //map(30, 0, 180, min, max);
  pos3 = 118; //map(60, 0, 180, min, max);
  pos4 = 163; //map(100, 0, 180, min, max);
  pos5 = 214; //map(145, 0, 180, min, max);
  pos6 = 248; //map(175, 0, 180, min, max);
}

void loop() {
  analogWrite(9, pos1);
  delay(1500);
  analogWrite(9, pos2);
  delay(1500);
  analogWrite(9, pos3);
  delay(1500);
  analogWrite(9, pos4);
  delay(1500);
  analogWrite(9, pos5);
  delay(1500);
  analogWrite(9, pos6);
  delay(1500);
}
