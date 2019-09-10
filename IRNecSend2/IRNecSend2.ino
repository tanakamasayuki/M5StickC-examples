#include <M5StickC.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 9;              // M5StickCはGPIO9にIRが内蔵

IRsend irsend(kIrLed);                  // IR送信を宣言

// リモコンコード保存用構造体
struct REMOTE {
  char name[9];
  uint64_t command;
};

// リモコンコード一覧
REMOTE remote[] = {
  { "POWER" , 0x00FFA25DUL },
  { "VOL+"  , 0x00FF629DUL },
  { "VOL-"  , 0x00FFA857UL },
  { "0"     , 0x00FF6897UL },
  { "1"     , 0x00FF30CFUL },
  { "2"     , 0x00FF18E7UL },
  { "3"     , 0x00FF7A85UL },
  { "4"     , 0x00FF10EFUL },
  { "5"     , 0x00FF38C7UL },
  { "6"     , 0x00FF5AA5UL },
  { "7"     , 0x00FF42BDUL },
  { "8"     , 0x00FF4AB5UL },
  { "9"     , 0x00FF52ADUL },
};

int cursor = 0; // カーソル位置

void setup() {
  M5.begin();     // M5StickC初期化
  irsend.begin(); // IR初期化

  // リモコン項目表示
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 8);
  for ( int i = 0 ; i < ( sizeof(remote) / sizeof(REMOTE) ) ; i++ ) {
    M5.Lcd.print((cursor == i) ? ">" : " ");
    M5.Lcd.println(remote[i].name);
  }
}

void loop() {
  M5.update();  // ボタン状態更新

  // M5ボタンで送信
  if ( M5.BtnA.wasPressed() ) {
    // 送信4Byte(カスタマーコード2Byte+リモコンコード+反転リモコンコード)
    irsend.sendNEC(remote[cursor].command);

    // デバッグ表示
    Serial.printf("Send IR : 0x%08LX", remote[cursor].command);
  }

  // 右ボタンでカーソル移動
  if ( M5.BtnB.wasPressed() ) {
    cursor++;
    cursor = cursor % ( sizeof(remote) / sizeof(REMOTE) );

    // カーソル描画
    M5.Lcd.setCursor(0, 8);
    for ( int i = 0 ; i < ( sizeof(remote) / sizeof(REMOTE) ) ; i++ ) {
      M5.Lcd.println((cursor == i) ? ">" : " ");
    }
  }

  delay(100);
}
