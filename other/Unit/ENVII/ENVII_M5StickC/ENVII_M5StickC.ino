/*Please install the < Adafruit BMP280 Library > （https://github.com/adafruit/Adafruit_BMP280_Library）
                     < Adafruit SHT31 Library > （https://github.com/adafruit/Adafruit_SHT31）
   from the library manager before use.
  This code will display the temperature, humidity and air pressure information on the screen*/

#include <M5StickC.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SHT31.h>

Adafruit_SHT31 sht3x = Adafruit_SHT31(&Wire);
Adafruit_BMP280 bme = Adafruit_BMP280(&Wire);

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  Wire.begin(32, 33);
  Serial.println(F("ENV Unit(SHT30 and BMP280) test..."));

  while (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  while (!sht3x.begin(0x44)) {
    Serial.println("Could not find a valid SHT3X sensor, check wiring!");
    M5.Lcd.println("Could not find a valid SHT3X sensor, check wiring!");
  }
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.println("ENV Unit test...");
}

void loop() {
  pressure = bme.readPressure();
  tmp = sht3x.readTemperature();
  hum = sht3x.readHumidity();

  Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%  Pressure: %0.2fhPa\r\n", tmp, hum, pressure / 100);
  M5.Lcd.setCursor(0, 16);
  M5.Lcd.setTextColor(WHITE, BLACK);

  M5.Lcd.printf("Temp:%2.2f*C\n", tmp);
  M5.Lcd.printf("Humi:%2.2f%%\n", hum);
  M5.Lcd.printf("Prs :%2.0fhPa\n", pressure / 100);

  delay(1000);
}
