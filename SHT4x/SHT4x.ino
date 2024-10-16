#include <Wire.h>
#include <Adafruit_SHT4x.h>
#include <SSD1306.h>
#include <BluetoothSerial.h>  // Include BluetoothSerial library for Classic Bluetooth

// Pin allocation and settings
Adafruit_SHT4x sht4x = Adafruit_SHT4x();
SSD1306 display(0x3C, 21, 22); // SDA 21, SCL 22 for I2C

BluetoothSerial ESP_BT; // Create a BluetoothSerial object

const int batteryPin = 4; // Battery reading from analog pin 4

void setup() {
  Serial.begin(115200);
  Serial.println("SHT4x and SSD1306 with Bluetooth & Battery Reading Test");

  Wire.begin(21, 22); // Initialize I2C with SDA on 21, SCL on 22


  // Initialize Bluetooth
  ESP_BT.begin("ESP32_SHT4x"); // Bluetooth device name
  Serial.println("Bluetooth Device Started. You can pair now.");

  // Initialize display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  // Initialize SHT4x sensor
  if (!sht4x.begin()) {
    Serial.println("Couldn't find SHT4x sensor!");
    display.clear();
    display.drawString(0, 0, "SHT4x not found!");
    display.display();
    while (1) delay(1); // Stop execution if sensor is not found
  }

  Serial.println("SHT4x sensor found");
  display.clear();
  display.drawString(0, 0, "SHT4x Init OK");
  display.display();
  delay(2000);

  // Initialize battery pin
  pinMode(batteryPin, INPUT);
}

void loop() {
  // Read battery level
  int batteryValue = analogRead(batteryPin);
  float batteryVoltage = batteryValue * (3.3 / 4095.0) * 2; // Assuming voltage divider

  sensors_event_t humidity, temp;
  sht4x.getEvent(&humidity, &temp);  // Get sensor readings

  // Display readings on OLED
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "SHT4x Testing");

  // Show BLE in the top right corner if Bluetooth is active
  if (ESP_BT.hasClient()) {
    display.drawString(90, 0, "BLE");  // Display "BLE" at the top-right corner
  }

  display.drawString(0, 16, "Temp: " + String(temp.temperature) + " C");
  display.drawString(0, 32, "Humidity: " + String(humidity.relative_humidity) + " %");
  display.drawString(0, 48, "Battery: " + String(batteryVoltage, 2) + " V");
  display.display();

  // Print values to serial for debugging
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2);
  Serial.println(" V");

  // Send data over Bluetooth
  if (ESP_BT.hasClient()) {  // Check if Bluetooth is connected to a client
    ESP_BT.print("Temperature: ");
    ESP_BT.print(temp.temperature);
    ESP_BT.println(" C");

    ESP_BT.print("Humidity: ");
    ESP_BT.print(humidity.relative_humidity);
    ESP_BT.println(" %");

    ESP_BT.print("Battery Voltage: ");
    ESP_BT.print(batteryVoltage, 2);
    ESP_BT.println(" V");
  }

  delay(2000);  // Delay between updates
}
