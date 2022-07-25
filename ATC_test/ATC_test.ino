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
movingAvg avgTemp(10);                  // define the moving average object
movingAvg avgbatt(10);                  // define the moving average object

BluetoothSerial SerialBT;
int maxV = 2840;
int minV = 1710;
#include "Adafruit_SHT4x.h"

//Adafruit_SHT4x sht4 = Adafruit_SHT4x();

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
  avgbatt.begin();
//  Serial.println("Adafruit SHT4x test");
//  if (! sht4.begin()) {
//    Serial.println("Couldn't find SHT4x");
//    while (1) delay(1);
//  }
//  Serial.println("Found SHT4x sensor");
//  Serial.print("Serial number 0x");
//  Serial.println(sht4.readSerial(), HEX);
//
//  // You can have 3 different precisions, higher precision takes longer
//  sht4.setPrecision(SHT4X_HIGH_PRECISION);
//  Serial.println("Found SHT4x sensor");
//  Serial.print("Serial number 0x");
//  Serial.println(sht4.readSerial(), HEX);
//  sht4.setPrecision(SHT4X_HIGH_PRECISION);


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
//  sensors_event_t humidity, temp;

//  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data



  display.clearDisplay();
//  tem = temp.temperature;
//  hum = humidity.relative_humidity;
  int temp_a = analogRead(2);
  float T_temp = (1023 / (temp_a*3.3/4.095)) - 1;
  T_temp = 100000 / T_temp;
  float avg = avgTemp.reading(T_temp) / 10.0;
  int batt_a = avgbatt.reading(analogRead(4));
  int batt_b = batt_a * 3300 / 4095;
  float batt = (batt_b - minV) / ((maxV - minV) / 100);
  if (batt > 100) {
    batt = 100;
  }
  if (batt < 0) {
    batt = 0;
  }
  display.setTextSize(0.5);      // Normal 1:1 pixel scale

  //Serial Monitor
//  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
//  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
  Serial.print("ATC: "); Serial.print(avg); Serial.println(" C.");
  Serial.print(" ATC_V: "); Serial.print(temp_a * 3.3 / 4.095); Serial.println(" mV");
  Serial.print("ATC_AnalogRead: "); Serial.print(temp_a); Serial.println(" count");
  Serial.print("Battery: "); Serial.print(batt); Serial.print("%");Serial.print(" V_Batt: "); Serial.print(batt_b*1.333); Serial.println(" mV");
  Serial.println(" ");
  //Display
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(5 , 5);    // Start at top-left corner
  display.write(("Bat: " + String(batt, 0) + " %").c_str());
  display.setCursor(70 , 5);    // Start at top-left corner
  display.write((String(batt_b*1.333) + " mV").c_str());
  display.setCursor(5, 20);     // Start at top-left corner
  display.write(("ATC: " + String(avg) + " C").c_str());
  display.setCursor(5, 40);     // Start at top-left corner
  display.write(("V_ATC: " + String(temp_a * 3.3 / 4.095) + " mV").c_str());

  //display.cp437(true);         // Use full 256 char 'Code Page 437' font
//  display.write(("T: " + String(tem, 1) + " C").c_str());
//  display.setCursor(5, 50);     // Start at top-left corner
//  display.write(("RH:" + String(hum, 1) + " %").c_str());
  display.display();

  //BT
  SerialBT.print(("T: " + String(avg, 1) + " C, ").c_str());
//  SerialBT.print(("RH:" + String(hum, 1) + " %, \n").c_str());

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
