#include <M5StickC.h>

// 最後に電圧確認した時間
int lastVinTime = 0;

// AXPを電源オフする
void axp_halt(){
    Wire1.beginTransmission(0x34);
    Wire1.write(0x32);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();

    Wire1.beginTransmission(0x34);
    Wire1.write(0x32);
    Wire1.write(buf | 0x80); // halt bit
    Wire1.endTransmission();
}

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(WHITE);
}

void loop() {
  if( (M5.Axp.GetVinData()*1.7) < 3.0 ){
    if( lastVinTime + 5000 < millis() ){
      // 3.0V以下が5秒以上で電源オフ
      axp_halt();
    }
  } else {
    // 最終電圧確認時間更新
    lastVinTime = millis();
  }

  delay(500);
}
