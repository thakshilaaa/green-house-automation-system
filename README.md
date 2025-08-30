# Greenhouse Automation System

This project is an **ESP32-based Greenhouse Automation System** that monitors and controls environmental conditions for plants.

## Features
- Reads **temperature** and **humidity** using DHT11.
- Monitors **soil moisture** and controls water pump automatically.
- Measures **light intensity** using LDR sensor.
- Controls **fan** based on temperature and light conditions.
- Remote monitoring and control using **Blynk app**.

## Hardware Used
- ESP32
- DHT11 Sensor
- Soil Moisture Sensor
- LDR Sensor
- Relay Module (for Fan and Pump)

## Software Used
- Arduino IDE
- Blynk Library

## Usage
1. Connect ESP32 to your sensors and relay modules.
2. Update WiFi credentials and Blynk auth token in the code.
3. Upload code via Arduino IDE.
4. Monitor and control your greenhouse via the Blynk app.

