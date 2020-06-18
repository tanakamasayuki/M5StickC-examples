/*Please install the < Adafruit BMP280 Library > （https://github.com/adafruit/Adafruit_BMP280_Library）
                     < Adafruit SHT31 Library > （https://github.com/adafruit/Adafruit_SHT31）
   from the library manager before use.
  This code will display the temperature, humidity and air pressure information on the screen*/

#include <M5Atom.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SHT31.h>

Adafruit_SHT31 sht3x = Adafruit_SHT31(&Wire1);
Adafruit_BMP280 bme = Adafruit_BMP280(&Wire1);

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

void setup() {
  M5.begin();
  Wire1.begin(26, 32);
  Serial.println(F("ENV Unit(SHT30 and BMP280) test..."));

  while (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  while (!sht3x.begin(0x44)) {
    Serial.println("Could not find a valid SHT3X sensor, check wiring!");
  }
}

void loop() {
  pressure = bme.readPressure();
  tmp = sht3x.readTemperature();
  hum = sht3x.readHumidity();

  Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%  Pressure: %0.2fhPa\r\n", tmp, hum, pressure / 100);

  delay(100);
}
