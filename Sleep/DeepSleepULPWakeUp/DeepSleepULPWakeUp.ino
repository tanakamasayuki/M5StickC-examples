#include "esp32/ulp.h"

// スローメモリー変数割当
enum {
  SLOW_BOOT_COUNT,      // 起動回数

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

void setup(){
  // シリアル初期化
  Serial.begin(115200);

  // 初回起動の特殊処理
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  if( wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED ){
    // シリアル初期化待ち
    delay(1000);

    // ULPを1秒間隔で起動
    ULP_PROG(1000000);

    // 起動回数初期化
    RTC_SLOW_MEM[SLOW_BOOT_COUNT] = 0;
  }

  // 起動回数カウントアップ
  RTC_SLOW_MEM[SLOW_BOOT_COUNT]++;
  Serial.printf("起動回数: %d ", RTC_SLOW_MEM[SLOW_BOOT_COUNT]);

  // 起動方法取得
  switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0      : Serial.printf("外部割り込み(RTC_IO)で起動\n"); break;
    case ESP_SLEEP_WAKEUP_EXT1      : Serial.printf("外部割り込み(RTC_CNTL)で起動 IO=%08LX\n", esp_sleep_get_ext1_wakeup_status()); break;
    case ESP_SLEEP_WAKEUP_TIMER     : Serial.printf("タイマー割り込みで起動\n"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.printf("タッチ割り込みで起動 PAD=%d\n", esp_sleep_get_touchpad_wakeup_status()); break;
    case ESP_SLEEP_WAKEUP_ULP       : Serial.printf("ULPプログラムで起動\n"); break;
    case ESP_SLEEP_WAKEUP_GPIO      : Serial.printf("ライトスリープからGPIO割り込みで起動\n"); break;
    case ESP_SLEEP_WAKEUP_UART      : Serial.printf("ライトスリープからUART割り込みで起動\n"); break;
    default                         : Serial.printf("スリープ以外からの起動\n"); break;
  }

  // ULPをウェイクアップソースとして有効にする
  esp_sleep_enable_ulp_wakeup();

  // ディープスリープ
  esp_deep_sleep_start();
}

void loop(){
}
