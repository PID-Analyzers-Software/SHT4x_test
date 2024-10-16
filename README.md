# ESP32 SHT4x Sensor with OLED and Bluetooth

<img width="635" alt="image" src="https://github.com/user-attachments/assets/61356d3c-23c8-42e2-a117-b96ad7533eb7">

Test of SHT4x temperature and humidity sensor, display the readings on an SSD1306 OLED screen, and transmit the data via Bluetooth. It also reads the battery voltage from analog pin 4 and displays it on the OLED screen.

## Features

- Reads temperature and humidity data from the SHT4x sensor.
- Displays sensor data on the SSD1306 OLED screen.
- Transmits sensor data via Bluetooth.
- Monitors and displays battery voltage from analog pin 4.
- Displays a "BLE" label at the top-right corner of the OLED if a Bluetooth client is connected.


## Pin Configuration

| Component            | Pin          |
|----------------------|--------------|
| SHT4x Sensor (I2C)   | SDA: 23, SCL: 22 |
| SSD1306 Display (I2C) | SDA: 23, SCL: 22 |
| Battery Voltage       | Analog Pin 4 |
| Bluetooth             | Built-in on ESP32 |

## Libraries Required

- **Adafruit SHT4x Library**: For reading data from the SHT4x sensor.
- **SSD1306**: For handling the SSD1306 OLED display.
- **BluetoothSerial**: For enabling Bluetooth communication.
- **Wire**: For I2C communication.
