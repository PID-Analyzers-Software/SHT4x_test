#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif
#include <movingAvg.h>                  // https://github.com/JChristensen/movingAvg
movingAvg avgTemp(15);                  // define the moving average object

BluetoothSerial SerialBT;
int maxV = 2930;
int minV = 1720;
#include "Adafruit_SHT4x.h"

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float tem, hum;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  avgTemp.begin();

  Serial.println("Adafruit SHT4x test");
  if (! sht4.begin()) {
    Serial.println("Couldn't find SHT4x");
    while (1) delay(1);
  }
  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);

  // You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  switch (sht4.getPrecision()) {
    case SHT4X_HIGH_PRECISION:
      Serial.println("High precision");
      break;
    case SHT4X_MED_PRECISION:
      Serial.println("Med precision");
      break;
    case SHT4X_LOW_PRECISION:
      Serial.println("Low precision");
      break;
  }

  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);

  // You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  switch (sht4.getPrecision()) {
    case SHT4X_HIGH_PRECISION:
      Serial.println("High precision");
      break;
    case SHT4X_MED_PRECISION:
      Serial.println("Med precision");
      break;
    case SHT4X_LOW_PRECISION:
      Serial.println("Low precision");
      break;
  }


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.

  // Clear the buffer
  display.clearDisplay();

}

void loop() {
  sensors_event_t humidity, temp;

  uint32_t timestamp = millis();
  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  timestamp = millis() - timestamp;



  Serial.print("Read duration (ms): ");
  Serial.println(timestamp);

  display.clearDisplay();
  tem = temp.temperature;
  hum = humidity.relative_humidity;
  int temp_a = analogRead(2);
  float T_temp = ((temp_a * (3.3 / 4095) * 1000)*0.1135-39.41)*10;
  float avg = avgTemp.reading(T_temp) / 10.0;
  int batt_a = analogRead(4);
  int batt_b = batt_a*3300/4095;
  float batt = (batt_b-minV)/((maxV-minV)/100);
  display.setTextSize(0.5);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
  Serial.print("TMP: "); Serial.print(avg); Serial.println(" C.");
  Serial.print("Battery: "); Serial.print(batt); Serial.println("%");

  display.setCursor(5 , 5);    // Start at top-left corner
  display.write(("Bat: " + String(batt, 0) + " %").c_str());
  display.setCursor(70 , 5);    // Start at top-left corner
  display.write((String(batt_b) + " mV").c_str());
  display.setCursor(5, 20);     // Start at top-left corner
  display.write(("TMP: " + String(temp_a) + " C").c_str());
  display.setCursor(5, 35);     // Start at top-left corner
  //display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write(("T: " + String(tem, 1) + " V").c_str());
  display.setCursor(5, 50);     // Start at top-left corner
  display.write(("RH:" + String(hum, 1) + " %").c_str());
  display.display();
  SerialBT.print(("T: " + String(avg, 1) + " C, ").c_str());
  SerialBT.print(("RH:" + String(hum, 1) + " %, \n").c_str());

  delay(1000);

}


void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("temeprature");
  display.display();
  delay(2000);
}



#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2
