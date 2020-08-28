#include <M5StackAuto.h>

const int inputBeepPin    = 26;
const int inputAnalogPin  = 36;

hw_timer_t *timer;
QueueHandle_t xQueue;
TaskHandle_t taskHandle;

const int16_t listCount = 1000;
int16_t list[listCount];
int16_t listIndex = 0;

// タイマー割り込み
void IRAM_ATTR onTimer() {
  int16_t data;

  // データ取得
  data = analogRead(inputAnalogPin);

  // キューを送信
  xQueueSendFromISR(xQueue, &data, 0);
}

// 実際のタイマー処理用タスク
void task(void *pvParameters) {
  int16_t data;
  int drawX = 9999;

  // 画面初期化
  M5.Lcd.fillRect(0, 8 * 3, M5.Lcd.width(), M5.Lcd.height(), DARKGREY);

  // 初期値設定
  data = analogRead(inputAnalogPin);
  for (int i = 0; i < M5.Lcd.width(); i++) {
    list[i] = data;
  }

  while (1) {
    // タイマー割り込みがあるまで待機する
    xQueueReceive(xQueue, &data, portMAX_DELAY);

    // 過去履歴更新
    list[listIndex] = data;
    listIndex++;
    listIndex = listIndex % M5.Lcd.width();
    int16_t minVal = 4096;
    int16_t maxVal = 0;
    for (int i = 0; i < M5.Lcd.width(); i++) {
      minVal = min(minVal, list[i]);
      maxVal = max(maxVal, list[i]);
    }

    // X軸計算
    drawX++;
    if (M5.Lcd.width() <= drawX) {
      // 右端まで行ったら初期化
      drawX = 0;
    }

    // 実際の処理
    int drawY = map(4095 - data, 0, 4095, 8 * 3, M5.Lcd.height() - 1);
    M5.Lcd.fillRect(drawX, 8 * 3, 16, M5.Lcd.height(), DARKGREY);
    M5.Lcd.drawPixel(drawX, drawY, WHITE);
    Serial.println(data);
    M5.Lcd.setCursor(M5.Lcd.width() - (6 * 10), 8 * 0);
    M5.Lcd.printf("min %5.3fV\n", 3.3 * minVal / 4095);
    M5.Lcd.setCursor(M5.Lcd.width() - (6 * 10), 8 * 1);
    M5.Lcd.printf("max %5.3fV\n", 3.3 * maxVal / 4095);
    M5.Lcd.setCursor(M5.Lcd.width() - (6 * 10), 8 * 2);
    M5.Lcd.printf("now %5.3fV\n", 3.3 * data / 4095);
  }
}

void setup() {
  M5.begin();
  pinMode(inputBeepPin, INPUT_PULLUP);
  pinMode(inputAnalogPin, ANALOG);
  pinMode(M5_LED, OUTPUT_OPEN_DRAIN);
  digitalWrite(M5_LED, HIGH);
  M5.Beep.setVolume(1);

  // 画面
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("M5StickC Tester");
  M5.Lcd.println(" GPIO26 LowBeep");
  M5.Lcd.println(" GPIO36 AnalogIn");

  // キュー作成
  xQueue = xQueueCreate(1, sizeof(int16_t));

  // Core1の優先度5でタスク起動
  xTaskCreateUniversal(
    task,           // タスク関数
    "task",         // タスク名(あまり意味はない)
    8192,           // スタックサイズ
    NULL,           // 引数
    5,              // 優先度(大きい方が高い)
    &taskHandle,    // タスクハンドル
    APP_CPU_NUM     // 実行するCPU(PRO_CPU_NUM or APP_CPU_NUM)
  );

  // 4つあるタイマーの1つめを利用
  // 1マイクロ秒ごとにカウント(どの周波数でも)
  // true:カウントアップ
  timer = timerBegin(0, getApbFrequency() / 1000000, true);

  // タイマー割り込み設定
  timerAttachInterrupt(timer, &onTimer, true);

  // マイクロ秒単位でタイマーセット
  timerAlarmWrite(timer, 20 * 1000, true);

  // タイマー開始
  timerAlarmEnable(timer);
}

void loop() {
  static bool beep = false;
  M5.update();
  if (!digitalRead(inputBeepPin)) {
    if (!beep) {
      beep = true;
      M5.Beep.tone(1000, 1000000);
      digitalWrite(M5_LED, LOW);
    }
  } else {
    if (beep) {
      beep = false;
      M5.Beep.mute();
      digitalWrite(M5_LED, HIGH);
    }
  }

  delay(1);
}
