#include <Joystick.h>

Joystick_ Joystick;

void setup() {
  Joystick.begin();
  Joystick.setXAxisRange(0, 1023);
}

void loop() {
  for (int i = 256; i <= 768; i++) {
    Joystick.setXAxis(i); 
    delay(5);
  }
  for (int i = 768; i >= 256; i--) {
    Joystick.setXAxis(i);
    delay(5);
  }
}