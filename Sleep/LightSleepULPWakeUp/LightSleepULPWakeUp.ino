#include "esp32/ulp.h"

int bootCount = 0; // ライトスリープは通常の変数で良い

// スローメモリー変数割当
enum {
  SLOW_PROG_ADDR        // プログラムの先頭アドレス
};

void ULP_PROG(uint32_t us) {
  // ULPの起動間隔を設定
  ulp_set_wakeup_period(0, us);

  // ULPプログラム
  const ulp_insn_t  ulp_prog[] = {
    I_WAKE(),           // メインチップ起動
    I_HALT(),           // ULPプログラム停止
  };

  // 変数の分プログラムを後ろにずらして実行
  size_t size = sizeof(ulp_prog) / sizeof(ulp_insn_t);
  ulp_process_macros_and_load(SLOW_PROG_ADDR, ulp_prog, &size);
  ulp_run(SLOW_PROG_ADDR);
}

void setup() {
  // シリアル初期化
  Serial.begin(115200);

  // シリアル初期化待ち
  delay(1000);

  // ULPを1秒間隔で起動
  ULP_PROG(1000000);

  // ULPをウェイクアップソースとして有効にする
  esp_sleep_enable_ulp_wakeup();
}

void loop() {
  // 起動回数カウントアップ
  bootCount++;
  Serial.printf("起動回数: %d ", bootCount);

  // 起動方法取得
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0      : Serial.printf("外部割り込み(RTC_IO)で起動\n"); break;
    case ESP_SLEEP_WAKEUP_EXT1      : Serial.printf("外部割り込み(RTC_CNTL)で起動 IO=%llX\n", esp_sleep_get_ext1_wakeup_status()); break;
    case ESP_SLEEP_WAKEUP_TIMER     : Serial.printf("タイマー割り込みで起動\n"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.printf("タッチ割り込みで起動\n"); break;
    case ESP_SLEEP_WAKEUP_ULP       : Serial.printf("ULPプログラムで起動\n"); break;
    case ESP_SLEEP_WAKEUP_GPIO      : Serial.printf("ライトスリープからGPIO割り込みで起動\n"); break;
    case ESP_SLEEP_WAKEUP_UART      : Serial.printf("ライトスリープからUART割り込みで起動\n"); break;
    default                         : Serial.printf("スリープ以外からの起動\n"); break;
  }

  // シリアルをすべて出力する
  Serial.flush();

  // ライトスリープ
  esp_light_sleep_start();
}
