#include <M5StickC.h>               // M5StickCの読み込み

// フォント関連
#include <efontEnableJaMini.h>      // 第2水準相当の日本語フォントを読み込み 4千文字139KB
#include <efont.h>                  // 実際のフォントデータの読み込み
#include "efontM5StickC.h"          // efontテキスト描画関数

// イメージデータ
#include "data.h"                   // 画像データの読み込み

// 画面ダブルバッファ用スプライト
TFT_eSprite canvas = TFT_eSprite(&M5.Lcd);

// FPS計算
static uint32_t sec;
static uint32_t psec;
static size_t fps = 0;
static size_t frame_count = 0;

// タイマー
hw_timer_t * timer = NULL;

// 画面描画タスクハンドル
TaskHandle_t taskHandle;

// 画面描画タスク
void dispTask(void *pvParameters) {
  int x = 0;              // X座標
  int y = 0;              // Y座標
  int xd = 4;             // X座標移動量
  int yd = 3;             // Y座標移動量

  while (1) {
    // タイマーが発生するまで待つ
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

    // まずは前フレームを描画してから次フレームを作る
    canvas.pushSprite(0, 0);

    // 黒で塗りつぶし
    canvas.fillSprite(0x0000);

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
    canvas.pushImage(x, y, imgWidth, imgHeight, img);

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
    sprintf(str, "M5 30フレーム検証%3d", fps);
    printEfont(&canvas, str, 0, 0);
  }
}

// タイマー割り込み
void IRAM_ATTR onTimer() {
  xTaskNotifyFromISR(taskHandle, 0, eIncrement, NULL);
}

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(12);          // 7-12で明るさ設定
  M5.Lcd.setRotation(3);            // 0-3で画面の向き
  M5.Lcd.setSwapBytes(true);        // スワップON(色がおかしい場合には変更する)

  // 画面ダブルバッファ用スプライト作成
  canvas.createSprite(M5.Lcd.width(), M5.Lcd.height());
  canvas.setSwapBytes(false);

  // タイマー作成(33.333ms)
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 33333, true);
  timerAlarmEnable(timer);

  // 描画用タスク作成
  xTaskCreateUniversal(
    dispTask,
    "dispTask",
    8192,
    NULL,
    1,
    &taskHandle,
    APP_CPU_NUM
  );
}

void loop() {
  // Wait
  delay(1);
}
