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
  {'7','8','9','+'},
  {'4','5','6','-'},
  {'1','2','3','*'},
  {'C','0','=','/'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; 
byte colPins[COLS] = {6, 7, 8, 9}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String inputString = "";
long firstNum = 0;
long secondNum = 0;
char op = ' ';
boolean resultShown = false;

void setup() {
  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED Initial failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.print("Calculator");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (resultShown) {
      inputString = "";
      resultShown = false;
    }

    if (key >= '0' && key <= '9') {
      inputString += key;
    } 
    else if (key == '+' || key == '-' || key == '*' || key == '/') {
      firstNum = inputString.toInt();
      op = key;
      inputString = "";
    } 
    else if (key == '=') {
      secondNum = inputString.toInt();
      float result = calculate();
      inputString = String(result);
      resultShown = true;
    } 
    else if (key == 'C') {
      inputString = "";
      firstNum = 0;
      secondNum = 0;
      op = ' ';
    }
    
    updateDisplay();
  }
}

float calculate() {
  switch(op) {
    case '+': return firstNum + secondNum;
    case '-': return firstNum - secondNum;
    case '*': return (float)firstNum * secondNum;
    case '/': return (secondNum != 0) ? (float)firstNum / secondNum : 0;
    default: return 0;
  }
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Input:");
  display.setCursor(0, 25);
  display.setTextSize(2);
  display.print(inputString);
  display.display();
}