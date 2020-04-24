#include <M5StickC.h>               // M5StickCの読み込み

// フォント関連
#include <efontEnableJaMini.h>      // 第2水準相当の日本語フォントを読み込み 4千文字139KB
#include <efont.h>                  // 実際のフォントデータの読み込み
#include "efontM5StickC.h"

// イメージデータ
#include "data.h"                   // 画像データの読み込み

// スプライト
TFT_eSprite sprite = TFT_eSprite(&M5.Lcd);

// FPS計算
static uint32_t sec;
static uint32_t psec;
static size_t fps = 0;
static size_t frame_count = 0;

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(12);          // 7-12で明るさ設定
  M5.Lcd.setRotation(3);            // 0-3で画面の向き

  // 画像用のスプライト作成
  sprite.createSprite(imgWidth, imgHeight);
  sprite.setSwapBytes(false);
  sprite.pushImage(0, 0, imgWidth, imgHeight, img);
}

void loop() {
  // 描画開始(明示的に宣言すると早くなる)
  M5.Lcd.startWrite();

  // 画像をランダムに表示
  int x = random(M5.Lcd.width());
  int y = random(M5.Lcd.height());
  sprite.pushSprite(x, y);

  // FPS更新
  ++frame_count;
  sec = millis() / 1000;
  if (psec != sec) {
    psec = sec;
    fps = frame_count;
    frame_count = 0;
  }

  // 文字表示
  char str[256];
  sprintf(str, "M5.Lcd検証    %3dfps", fps);
  printEfont(str, 0, 0);

  // 描画終了
  M5.Lcd.endWrite();

  // Wait
  delay(1);
}
