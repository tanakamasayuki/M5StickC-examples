#include <M5StickC.h>

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3); // 画面に入らないので横向きにする
}

void loop() {
  // Buttonクラスを利用するときには必ずUpdateを呼んで状態を更新する
  M5.update();

  // カーソル初期化
  M5.Lcd.setCursor(0, 0);

  // ホームボタンが現在押されているか？
  M5.Lcd.print("BtnA.isPressed():");
  M5.Lcd.println( M5.BtnA.isPressed() );

  // ホームボタンが現在離しているか？
  M5.Lcd.print("BtnA.isReleased():");
  M5.Lcd.println( M5.BtnA.isReleased() );

  // ホームボタンを押したか？（1度だけ取得可能）
  if ( M5.BtnA.wasPressed() ) {
    Serial.println("BtnA.wasPressed() == TRUE");
  }

  // ホームボタンを離したか？（1度だけ取得可能）
  if ( M5.BtnA.wasReleased() ) {
    Serial.println("BtnA.wasReleased() == TRUE");
  }

  // ホームボタンを現在ms以上押しているか？
  if ( M5.BtnA.pressedFor(1000) ) {
    Serial.println("BtnA.pressedFor(1000) == TRUE");
  }

  // ホームボタンを離してからms以上経過しているか？
  M5.Lcd.print("BtnA.releasedFor(1000):");
  M5.Lcd.println( M5.BtnA.releasedFor(1000) );

  // ホームボタンをms以上押してから離したか？（1度だけ取得可能）
  if ( M5.BtnA.wasReleasefor(1000) ) {
    Serial.println("BtnA.wasReleasefor(1000) == TRUE");
  }

  // ホームボタンが最後に更新した起動経過時間 millis()
  M5.Lcd.print("BtnA.lastChange():");
  M5.Lcd.println( M5.BtnA.lastChange() );

  // 空行を追加
  M5.Lcd.println();

  // 右ボタンが現在押されているか？
  M5.Lcd.print("BtnB.isPressed():");
  M5.Lcd.println( M5.BtnB.isPressed() );

  // 右ボタンが現在離しているか？
  M5.Lcd.print("BtnB.isReleased():");
  M5.Lcd.println( M5.BtnB.isReleased() );

  // 右ボタンを押したか？（1度だけ取得可能）
  if ( M5.BtnB.wasPressed() ) {
    Serial.println("BtnB.wasPressed() == TRUE");
  }

  // 右ボタンを離したか？（1度だけ取得可能）
  if ( M5.BtnB.wasReleased() ) {
    Serial.println("BtnB.wasReleased() == TRUE");
  }

  // 右ボタンを現在ms以上押しているか？
  if ( M5.BtnB.pressedFor(1000) ) {
    Serial.println("BtnB.pressedFor(1000) == TRUE");
  }

  // 右ボタンを離してからms以上経過しているか？
  M5.Lcd.print("BtnB.releasedFor(1000):");
  M5.Lcd.println( M5.BtnB.releasedFor(1000) );

  // 右ボタンをms以上押してから離したか？（1度だけ取得可能）
  if ( M5.BtnB.wasReleasefor(1000) ) {
    Serial.println("BtnB.wasReleasefor(1000) == TRUE");
  }

  // 右ボタンが最後に更新した起動経過時間 millis()
  M5.Lcd.print("BtnB.lastChange():");
  M5.Lcd.println( M5.BtnB.lastChange() );

  // 電源ボタンの状態取得（一度しか0以外のステータスは取得できない）
  int axpButton = M5.Axp.GetBtnPress();
  if ( axpButton == 1 ) {
    // 1秒以上電源ボタンを押している
    Serial.println("M5.Axp.GetBtnPress() == 1");
  }
  if ( axpButton == 2 ) {
    // 1秒未満電源ボタンを押して離した
    Serial.println("M5.Axp.GetBtnPress() == 2");
  }
}
