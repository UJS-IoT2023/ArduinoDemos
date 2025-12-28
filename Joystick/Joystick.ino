#include <Joystick.h>

// 明确声明为 JOYSTICK 类型
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK, 0, 0, true, false, false, false, false, false, false, false, false, false, false);

void setup() {
  Joystick.begin();
  // 建议范围设大一点，比如 0-1023
  Joystick.setXAxisRange(0, 1023);
}

void loop() {
  // 极慢速摆动，观察游戏内车轮是否能转到极限
  for (int i = 0; i <= 1023; i++) {
    Joystick.setXAxis(i);
    delay(20); // 调慢一点，方便观察
  }
  for (int i = 1023; i >= 0; i--) {
    Joystick.setXAxis(i);
    delay(20);
  }
}