int bootCount = 0;  // ライトスリープは通常の変数で良い
int touchPad = 0;   // タッチされたPAD

// タッチコールバック関数
void callback32(){
  touchPad = 32;
}
void callback33(){
  touchPad = 33;
}

void setup() {
  // シリアル初期化
  Serial.begin(115200);

  // シリアル初期化待ち
  delay(1000);

  // GPIO32, 33のタッチ有効、しきい値は環境によって異なるので反応しない場合には増減が必要
  // ただし32と33のタッチだけはなぜか番号が入れ替わっているので、GPIO32を取得するときは33を指定する必要がある
  static int threshold = 16;
  pinMode(GPIO_NUM_32, INPUT);
  pinMode(GPIO_NUM_33, INPUT);
  touchAttachInterrupt(GPIO_NUM_32, callback32, threshold);
  touchAttachInterrupt(GPIO_NUM_33, callback33, threshold);

  // タッチパッドをウェイクアップソースとして有効にする
  esp_sleep_enable_touchpad_wakeup();
}

void loop() {
  // 起動回数カウントアップ
  bootCount++;
  Serial.printf("起動回数: %d ", bootCount);

  // ライトスリープでsp_sleep_get_touchpad_wakeup_status()を呼び出したらリブートする？ PADはコールバックで判定する
  //esp_sleep_get_touchpad_wakeup_status();
  Serial.printf("PAD: %d ", touchPad);

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
