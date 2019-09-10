#include <M5StickC.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>

const uint16_t kIrLed = 9;            // M5StickCはGPIO9にIRが内蔵

IRsend irsend(kIrLed);                // IR送信を宣言

const uint32_t CUSTOMER_ID = 0x00ff;  // カスタマーIDをセット

// リモコンコード保存用構造体
struct REMOTE {
  char name[9];
  uint8_t command;
};

// リモコンコード一覧
REMOTE remote[] = {
  { "POWER" , 0x45 },
  { "VOL+"  , 0x46 },
  { "VOL-"  , 0x15 },
  { "0"     , 0x16 },
  { "1"     , 0x0c },
  { "2"     , 0x18 },
  { "3"     , 0x5e },
  { "4"     , 0x08 },
  { "5"     , 0x1c },
  { "6"     , 0x5a },
  { "7"     , 0x42 },
  { "8"     , 0x52 },
  { "9"     , 0x4a },
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
    uint64_t send = (uint64_t)CUSTOMER_ID << 16;          // カスタマーコード
    send += reverseBits(remote[cursor].command,8) << 8;   // リモコンコードを順番入れ替えて送信
    send += reverseBits(remote[cursor].command,8) ^ 0xff; // リモコンコードのビット反転（パリティ）
    irsend.sendNEC(send);                                 // 送信

    // デバッグ表示
    Serial.printf("Send IR : 0x%08LX", send);
    Serial.printf("(customer=0x%04X, ", CUSTOMER_ID);
    Serial.printf("command=0x%02X)\n", remote[cursor].command);
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
