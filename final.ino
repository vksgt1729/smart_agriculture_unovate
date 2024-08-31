// Blynk Template ID and Template Name
#define BLYNK_TEMPLATE_ID "TMPL-W7Q_wLw"
#define BLYNK_TEMPLATE_NAME "ENVI"

// Include the library files
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Initialize the LCD display
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Blynk authentication and Wi-Fi credentials
char auth[] = "6TeXxU1hAt6PyaymQ7aRtURKlC4SfTzD";
char ssid[] = "Galaxy F42 5G 7448";
char pass[] = "";

DHT dht(4, DHT11); // (DHT sensor pin, sensor type)
BlynkTimer timer;

// Define component pins
#define soilPin A0     // Soil Moisture Sensor pin for ESP8266

int relay1State = LOW;
int pushButton1State = HIGH;
#define RELAY_PIN_1   15
#define PUSH_BUTTON_1 13
#define VPIN_BUTTON_1 V12

void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("  Initializing  ");
  for (int a = 5; a <= 10; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();
  lcd.setCursor(11, 1);
  lcd.print("W:OFF");

  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(500L, checkPhysicalButton);
}

// Get the DHT11 sensor values
void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);
}

// Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(soilPin);
  value = map(value, 0, 1023, 0, 100); // Adjust for ESP8266's 10-bit ADC
  value = (value - 100) * -1;

  Blynk.virtualWrite(V3, value);
  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(value);
  lcd.print(" ");
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON_1);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}

void checkPhysicalButton() {
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
    if (pushButton1State != LOW) {
      relay1State = !relay1State;
      digitalWrite(RELAY_PIN_1, relay1State);
      Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
    }
    pushButton1State = LOW;
  } else {
    pushButton1State = HIGH;
  }
}

void loop() {
  if (relay1State == HIGH) {
    lcd.setCursor(11, 1);
    lcd.print("W:ON ");
  } else if (relay1State == LOW) {
    lcd.setCursor(11, 1);
    lcd.print("W:OFF");
  }
  
  Blynk.run();
  timer.run();
}
