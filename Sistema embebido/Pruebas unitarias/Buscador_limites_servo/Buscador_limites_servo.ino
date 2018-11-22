/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

//Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int min = 0; //40
int max = 255; //245
int pin = 11;

void setup() {
  //myservo.attach(11);  // attaches the servo on pin 9 to the servo object
  pinMode(pin, OUTPUT);
}

void loop() {
  for (pos = min; pos <= max; pos += 5) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    //myservo.write(pos);              // tell servo to go to position in variable 'pos'
    analogWrite(pin, pos);
    delay(100);                       // waits 15ms for the servo to reach the position
  }
  for (pos = max; pos >= min; pos -= 5) { // goes from 180 degrees to 0 degrees
    analogWrite(pin, pos);              // tell servo to go to position in variable 'pos'
    delay(100);                       // waits 15ms for the servo to reach the position
  }
}
