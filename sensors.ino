#include <DHT.h>

#define DHTPIN D4       // Define the pin where the data pin of the DHT11 is connected
#define DHTTYPE DHT11   // Define the type of sensor (DHT11)

DHT dht(DHTPIN, DHTTYPE);  // Initialize the DHT sensor

void setup() {
  Serial.begin(115200);  // Start the serial communication at a baud rate of 115200
  dht.begin();           // Start the DHT sensor
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();    // Read the humidity
  float t = dht.readTemperature(); // Read the temperature in Celsius

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the results to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
}
