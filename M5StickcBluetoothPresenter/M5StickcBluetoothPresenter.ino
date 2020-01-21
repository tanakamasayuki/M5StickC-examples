#include <M5StickC.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("M5StickC BLE Presenter");

// バッテリー更新用
unsigned long nextVbatCheck = 0;

// タイマー用
unsigned long startTimer = 0;

// バッテリー残量取得
int getVlevel() {
  float vbat = M5.Axp.GetBatVoltage();
  int vlevel = ( vbat - 3.2 ) / 0.8 * 100;
  if ( vlevel < 0 ) {
    vlevel = 0;
  }
  if ( 100 < vlevel ) {
    vlevel = 100;
  }

  return vlevel;
}

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(9);
  setCpuFrequencyMhz(80);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 16);
  M5.Lcd.println("BLE Presenter");
  M5.Lcd.println(" BtnA :Next");
  M5.Lcd.println(" BtnB :Prev");
  M5.Lcd.println(" BtnP :Reset");

  // 起動時にしかバッテリー残量は設定できない
  bleKeyboard.setBatteryLevel(getVlevel());
  bleKeyboard.begin();
}

void loop() {
  // ボタン状態更新
  M5.update();

  // キー操作
  if (bleKeyboard.isConnected()) {
    if ( M5.BtnA.wasPressed() ) {
      // ホームボタンでスライドを進める
      bleKeyboard.write(KEY_RIGHT_ARROW);
    }
    if ( M5.BtnB.wasPressed() ) {
      // 右ボタンでスライドを戻す
      bleKeyboard.write(KEY_LEFT_ARROW);
    }
    if ( M5.Axp.GetBtnPress() != 0 ) {
      // 電源ボタンでタイマーリセット
      startTimer = millis();
    }
  } else {
    // 接続していない場合にはタイマーを進めない
    startTimer = millis();
  }

  // バッテリー残量更新(1分毎)
  if (nextVbatCheck < millis()) {
    M5.Lcd.setCursor(112, 0);
    M5.Lcd.printf("%3d%%", getVlevel());

    nextVbatCheck = millis() + 60000;
  }

  // タイマー表示
  int min = ( millis() - startTimer ) / 1000 / 60;
  int sec = ( millis() - startTimer ) / 1000 % 60;
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("%5d:%02d", min, sec);

  // Wait
  delay(100);
}
