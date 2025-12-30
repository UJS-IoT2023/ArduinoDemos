#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- OLED Config ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- DHT11 Config ---
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  // Initialize DHT11
  dht.begin();

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED Initialize failed"));
    for(;;); 
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  display.clearDisplay(); 
  
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ENVIRONMENT MONITOR");

  display.setCursor(0, 15);
  display.setTextSize(2);
  display.print("Temp:");
  display.print(t, 1);
  display.print("C");

  display.setCursor(0, 40);
  display.setTextSize(2);
  display.print("Humi:");
  display.print(h, 0);
  display.print("%");

  display.display();

  // Set the frequence of DHT11 read
  delay(2000);
}