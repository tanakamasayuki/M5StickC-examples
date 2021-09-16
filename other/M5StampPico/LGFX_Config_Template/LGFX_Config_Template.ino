#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include "lgfx_user/M5UnitOLED.hpp"
#include "lgfx_user/M5UnitLCD.hpp"

#include "LGFX_ESP32_SSD1306_I2C.hpp"

#include "LGFX_ESP32_ST7735S_SPI.hpp"
#include "LGFX_ESP32_ST7789_SPI.hpp"

//M5UnitOLED display(/*SDA*/32, /*SCL*/33);
//M5UnitLCD display(/*SDA*/32, /*SCL*/33);
//LGFX_ESP32_SSD1306_I2C display(/*GND*/22, /*VCC*/21, /*scl*/19, /*sda*/18, /*port Wire=0, Wire1=1*/0, /*addr*/0x3C);
//LGFX_ESP32_ST7735S_SPI/*0.96*/ display(/*width*/ 80, /*height*/160, /*sclk(SCL)*/25, /*mosi(SDA)*/22, /*rst*/21, /*dc*/19, /*cs*/ 18, /*bl*/ -1, /*miso*/ -1, /*busy*/ -1);
//LGFX_ESP32_ST7789_SPI/*1.14*/  display(/*width*/135, /*height*/240, /*sclk(SCL)*/25, /*mosi(SDA)*/22, /*rst*/21, /*dc*/19, /*cs*/ 18, /*bl*/ -1, /*miso*/ -1, /*busy*/ -1);
//LGFX_ESP32_ST7789_SPI/*1.3*/   display(/*width*/240, /*height*/240, /*sclk(SCL)*/25, /*mosi(SDA)*/22, /*rst*/21, /*dc*/19, /*cs*/ -1, /*bl*/ 18, /*miso*/ -1, /*busy*/ -1);
//LGFX_ESP32_ST7789_SPI/*1.54*/  display(/*width*/240, /*height*/240, /*sclk(SCL)*/25, /*mosi(SDA)*/22, /*rst*/21, /*dc*/19, /*cs*/ 18, /*bl*/ -1, /*miso*/ -1, /*busy*/ -1);
//LGFX_ESP32_ST7789_SPI/*2.0*/   display(/*width*/240, /*height*/320, /*sclk(SCL)*/21, /*mosi(SDA)*/19, /*rst*/18, /*dc*/25, /*cs*/ 22, /*bl*/ -1, /*miso*/ -1, /*busy*/ -1);

void setup() {
  // Init
  display.init();
}

uint32_t count = ~0;
void loop(void) {
  display.startWrite();
  display.fillScreen(TFT_WHITE);

  display.setRotation(++count & 7);
  display.setColorDepth((count & 8) ? 16 : 24);

  display.setTextColor(random(65536));
  display.drawNumber(display.getRotation(), 16, 0);

  display.setTextColor(0xFF0000U);
  display.drawString("R", 30, 16);
  display.setTextColor(0x00FF00U);
  display.drawString("G", 40, 16);
  display.setTextColor(0x0000FFU);
  display.drawString("B", 50, 16);

  display.setTextColor(TFT_BLACK);
  display.drawString("BLACK", 60, 16);

  display.drawRect(10, 10, 20, 20, random(65536));

  display.endWrite();

  delay(1000);
}
