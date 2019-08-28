RTC_DATA_ATTR int bootCount = 0; // RTCスローメモリに変数を確保

void callback(){
}

void setup(){
  // シリアル初期化
  Serial.begin(115200);

  // 初回起動だけはシリアル初期化待ち
  if( bootCount == 0 ){
    delay(1000);
  }

  // 起動回数カウントアップ
  bootCount++;
  Serial.printf("起動回数: %d ", bootCount);

  // 起動方法取得
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
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

  // GPIO32, 33のタッチ有効、しきい値は環境によって異なるので反応しない場合には増減が必要
  static int threshold = 16;
  pinMode(GPIO_NUM_32, INPUT);
  pinMode(GPIO_NUM_33, INPUT);
  touchAttachInterrupt(GPIO_NUM_32, callback, threshold);
  touchAttachInterrupt(GPIO_NUM_33, callback, threshold);

  // タッチパッドをウェイクアップソースとして有効にする
  esp_sleep_enable_touchpad_wakeup();

  // ディープスリープ
  esp_deep_sleep_start();
}

void loop(){
}
