#include "./NuestraBarreraLaser.h"

NuestraBarreraLaser barrera(2, 0);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  barrera.activarBarrera();
  delay(500);
  barrera.desactivarBarrera();
  delay(500);
}
