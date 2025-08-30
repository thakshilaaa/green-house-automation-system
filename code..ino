#define BLYNK_TEMPLATE_ID "TMPL6l0D6yKI-"
#define BLYNK_TEMPLATE_NAME "Green House Automation System"
#define BLYNK_AUTH_TOKEN "iEfOH5C2ZaQYSRPVpHDEmlKDGWESNWUf"

#include <WiFi.h>  // For ESP32 WiFi connection
#include <BlynkSimpleEsp32.h>  // Blynk library for ESP32
#include <DHT.h>   // DHT sensor

#define DHTPIN 4            // GPIO 4 for DHT sensor (D2 on NodeMCU)
#define LDR_PIN A0          // Analog pin (A0)
#define SOIL_MOIST_PIN 13   // GPIO 13 for soil moisture sensor (D7 on NodeMCU)
#define RELAY_FAN 14        // GPIO 14 for fan relay (D5 on NodeMCU)
#define RELAY_PUMP 5        // GPIO 5 for pump relay (D1 on NodeMCU)

DHT dht(DHTPIN, DHT11);  // Initialize DHT sensor (DHT11 model)

char auth[] = "iEfOH5C2ZaQYSRPVpHDEmlKDGWESNWUf";  // Blynk Auth token
char ssid[] = "Project";   // WiFi SSID
char pass[] = "12345678";  // WiFi Password

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);  // Initialize Blynk connection

  pinMode(RELAY_FAN, OUTPUT);  // Set relay for fan as output
  pinMode(RELAY_PUMP, OUTPUT);  // Set relay for pump as output
  pinMode(SOIL_MOIST_PIN, INPUT);  // Set soil moisture sensor pin as input
  
  dht.begin();  // Start the DHT sensor

  // Ensure fan and pump are off initially
  digitalWrite(RELAY_FAN, HIGH);  
  digitalWrite(RELAY_PUMP, HIGH);  
}

void loop() {
  Blynk.run();  // Run Blynk

  // Read sensor values
  int ldrValue = analogRead(LDR_PIN);
  float ldr_map_val = map(ldrValue, 0, 4095, 100, 0);  // Adjust range for ESP32 ADC (0-4095)
  float temp = dht.readTemperature();  // Get temperature from DHT
  float humidity = dht.readHumidity();  // Get humidity from DHT
  int soilMoisture = digitalRead(SOIL_MOIST_PIN);  // Read soil moisture level

  // Print sensor values
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("°C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("LDR Value: ");
  Serial.println(ldr_map_val);

  Serial.print("Soil Moisture: ");
  if (soilMoisture == HIGH) {
    Serial.println("Dry");
  } else {
    Serial.println("Moist");
  }

  // Determine if it's daytime based on LDR value
  bool isDay = ldrValue < 2000;  // Adjust threshold for ESP32 (0-4095 range)

  // Fan control logic based on temperature and light levels
  if (isDay && temp > 35) {
    digitalWrite(RELAY_FAN, LOW);  // Turn fan on during daytime if temp > 35°C
    Serial.println("Fan: ON (Daytime, Temp > 35°C)");
    Blynk.virtualWrite(V1, 255);  // Update Blynk LED for fan
  } else if (!isDay && temp > 30) {
    digitalWrite(RELAY_FAN, LOW);  // Turn fan on during nighttime if temp > 30°C
    Serial.println("Fan: ON (Nighttime, Temp > 30°C)");
    Blynk.virtualWrite(V1, 255);  // Update Blynk LED for fan
  } else {
    digitalWrite(RELAY_FAN, HIGH);  // Turn fan off
    Serial.println("Fan: OFF");
    Blynk.virtualWrite(V1, 0);  // Update Blynk LED for fan
  }

  // Pump control logic based on soil moisture
  if (soilMoisture == LOW) {
    digitalWrite(RELAY_PUMP, LOW);  // Turn pump on if soil is dry
    Serial.println("Pump: ON (Soil Dry)");
    Blynk.virtualWrite(V3, 255);  // Update Blynk LED for pump
  } else {
    digitalWrite(RELAY_PUMP, HIGH);  // Turn pump off if soil is moist
    Serial.println("Pump: OFF (Soil Moist)");
    Blynk.virtualWrite(V3, 0);  // Update Blynk LED for pump
  }

  // Send sensor data to Blynk
  Blynk.virtualWrite(V4, temp);         // Send temperature to Blynk
  Blynk.virtualWrite(V2, ldr_map_val);  // Send LDR value to Blynk
  Blynk.virtualWrite(V0, humidity);     // Send humidity to Blynk

  delay(1000);  // Delay before next reading
}