#include <M5StickC.h>               // M5StickCの読み込み

// フォント関連
#include <efontEnableJaMini.h>      // 第2水準相当の日本語フォントを読み込み 4千文字139KB
#include <efont.h>                  // 実際のフォントデータの読み込み
#include "efontM5StickC.h"          // efontテキスト描画関数

// イメージデータ
#include "data.h"                   // 画像データの読み込み

// FPS計算
static uint32_t sec;
static uint32_t psec;
static size_t fps = 0;
static size_t frame_count = 0;

// 描画座標
int x = 0;              // X座標
int y = 0;              // Y座標
int xd = 4;             // X座標移動量
int yd = 3;             // Y座標移動量

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(12);          // 7-12で明るさ設定
  M5.Lcd.setRotation(3);            // 0-3で画面の向き
  M5.Lcd.setSwapBytes(true);        // スワップON(色がおかしい場合には変更する)
}

void loop() {
  // 描画開始(明示的に宣言すると早くなる)
  M5.Lcd.startWrite();

  // 黒で塗りつぶし
  M5.Lcd.fillScreen(BLACK);

  // 画像を移動して描画
  x += xd;
  if ( x <= 0) {
    x = 0;
    xd = 4;
  } else if ( M5.Lcd.width() <= x ) {
    x = M5.Lcd.width();
    xd = -4;
  }
  y += yd;
  if ( y <= 0) {
    y = 0;
    yd = 3;
  } else if ( M5.Lcd.height() <= y ) {
    y = M5.Lcd.height();
    yd = -3;
  }
  M5.Lcd.pushImage(x, y, imgWidth, imgHeight, img, WHITE);

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
  sprintf(str, "M5ImageMove検証  %3d", fps);
  printEfont(str, 0, 0);

  // 描画終了
  M5.Lcd.endWrite();

  // Wait
  delay(100);
}
