#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; 
byte colPins[COLS] = {6, 7, 8, 9}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int buzzer = 1;
const int ledPin = A0;
const String PASSWORD = "7355608";

String inputCode = "";
bool isPlanted = false;
unsigned long startTime;
int countdown = 40;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  showMsg("READY TO PLANT", 1);
}

void loop() {
  if (!isPlanted) {
    char key = keypad.getKey();
    if (key) {
      beep(100); 
      if (key == '*') {
        inputCode = "";
      } else {
        inputCode += key;
      }
      
      updateEntryDisplay();

      if (inputCode == PASSWORD) {
        plantBomb();
      }
    }
  } else {
    handleCountdown();
  }
}

void updateEntryDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("ENTER CODE:");
  display.setTextSize(2);
  display.println(inputCode);
  display.display();
}

void plantBomb() {
  isPlanted = true;
  startTime = millis();
  showMsg("BOMB PLANTED", 2);
  delay(1000);
}

void handleCountdown() {
  unsigned long elapsed = (millis() - startTime) / 1000;
  int timeLeft = countdown - elapsed;

  int beepInterval = 1000;
  if (timeLeft < 5) beepInterval = 150;
  else if (timeLeft < 10) beepInterval = 300;
  else if (timeLeft < 20) beepInterval = 500;

  static unsigned long lastBeep = 0;
  if (millis() - lastBeep > beepInterval) {
    lastBeep = millis();
    digitalWrite(ledPin, HIGH);
    beep(100);
    digitalWrite(ledPin, LOW);
  }

  display.clearDisplay();
  display.setCursor(20, 20);
  display.setTextSize(4);
  display.print(timeLeft);
  display.display();

  if (timeLeft <= 0) {
    explode();
  }
}

void explode() {
  display.clearDisplay();
  display.setCursor(10, 20);
  display.setTextSize(3);
  display.println("BOOM!!!");
  display.display();
  
  for(int i=0; i<50; i++) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzer, LOW);
    delay(50);
  }
  
  isPlanted = false;
  inputCode = "";
  showMsg("TERRORISTS WIN", 1);
  delay(3000);
  showMsg("READY TO PLANT", 1);
}

void beep(int ms) {
  digitalWrite(buzzer, HIGH);
  delay(ms);
  digitalWrite(buzzer, LOW);
}

void showMsg(String msg, int sz) {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(sz);
  display.println(msg);
  display.display();
}