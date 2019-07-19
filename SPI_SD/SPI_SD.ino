#include <M5StickC.h>
#include "SD.h"

// PIN配置
enum { spi_sck = 0, spi_miso = 26, spi_mosi = 32, spi_ss = 33 };

void setup()
{
  // M5StickC初期化
  M5.begin();
  M5.Lcd.setRotation(3);

  // SPI初期化
  SPI.begin(spi_sck, spi_miso, spi_mosi, spi_ss);

  // SDカード初期化
  if (!SD.begin(spi_ss)) {
    M5.Lcd.println("Card Mount Failed");
    return;
  }

  // SDカード種別取得
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    M5.Lcd.println("None SD Card");
    return;
  }
  M5.Lcd.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    M5.Lcd.println("MMC");
  } else if (cardType == CARD_SD) {
    M5.Lcd.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    M5.Lcd.println("SDHC");
  } else {
    M5.Lcd.println("UNKNOWN");
  }

  // SDカード容量取得
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  M5.Lcd.printf("SD Card Size: %lluMB\n", cardSize);
}

void loop() {
}
