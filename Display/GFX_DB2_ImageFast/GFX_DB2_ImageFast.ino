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

// 画面ダブルバッファ用スプライト
static LGFX_Sprite canvas1(&lcd);
static LGFX_Sprite canvas2(&lcd);
static LGFX_Sprite *canvas[] = {&canvas1, &canvas2};
static uint8_t frameBuff = 0;

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

  // 画面ダブルバッファ用スプライト作成
  canvas1.createSprite(lcd.width(), lcd.height());
  canvas1.setSwapBytes(true);
  canvas1.setTextEFont();            // efontを有効化
  canvas2.createSprite(lcd.width(), lcd.height());
  canvas2.setSwapBytes(true);
  canvas2.setTextEFont();            // efontを有効化

  // 描画開始(SPIを専有)
  lcd.startWrite();
}

float x = 0;
float y = 0;
float xd = 1.2;
float yd = 1.5;

void loop() {
  // フレームバッファの切り替え
  frameBuff = (frameBuff + 1) & 0x01;

  // 黒で初期化
  canvas[frameBuff]->clear();

  // 画像を移動
  x += xd;
  if (x <= 0) {
    x = 0;
    xd = -xd;
  } else if (lcd.width() <= x) {
    x = lcd.width();
    xd = -xd;
  }
  y += yd;
  if (y <= 0) {
    y = 0;
    yd = -yd;
  } else if (lcd.height() <= y) {
    y = lcd.height();
    yd = -yd;
  }
  canvas[frameBuff]->pushImage((int)x, (int)y, imgWidth, imgHeight, img);

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
  sprintf(str, "GFXDB2Fast検証   %3d", fps);
  canvas[frameBuff]->setCursor(0, 0);
  canvas[frameBuff]->printf(str);

  // 描画
  canvas[frameBuff]->pushSprite(0, 0);
}
