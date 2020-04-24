// フォント関連
#include <efontEnableJaMini.h>      // (どれか一つ有効化)第2水準相当の日本語フォントを読み込み 4千文字139KB
#include <efont.h>                  // 実際のフォントデータの読み込み

// LovyanGFX
#include <LovyanGFX.hpp>            // LovyanGFXの読み込み

// イメージデータ
#include "data.h"                   // 画像データの読み込み

// M5StickCライブラリとの共存設定
#define _M5DISPLAY_H_               // M5StickCとの共存用設定
class M5Display {};                 // 既存の描画関数は使えなくする
#include <M5StickC.h>               // M5StickCの読み込み
#include "utility/ST7735_Defines.h" // BLACKなどの定義を読み込む

static LGFX lcd;                    // 描画クラス

// FPS計算
static uint32_t sec;
static uint32_t psec;
static size_t fps = 0;
static size_t frame_count = 0;

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(12);          // 7-12で明るさ設定
  lcd.init();                       // 初期化
  lcd.setTextEFont();               // efontを有効化
  lcd.setRotation(3);               // 0-3で画面の向き
  lcd.setSwapBytes(true);           // スワップON(色がおかしい場合には変更する)
}

void loop() {
  // 描画開始(明示的に宣言すると早くなる)
  lcd.startWrite();

  // 画像をランダムに表示
  int x = random(lcd.width());
  int y = random(lcd.height());
  lcd.pushImage(x, y, imgWidth, imgHeight, img);

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
  sprintf(str, "GFXImgRGB565検証 %3d", fps);
  lcd.setCursor(0, 0);
  lcd.printf(str);

  // 描画終了
  lcd.endWrite();

  // Wait
  delay(1);
}
