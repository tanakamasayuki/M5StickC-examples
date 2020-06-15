#include "FS.h"
#include "SPIFFS.h"
#include "file.h"

#define FILENAME "/trumpet2.mp3"

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  delay(50);

  Serial.println("SPIFFS FILE SAVE");

  // フォーマット
  Serial.println("SPIFFS FORMAT START");
  SPIFFS.format();
  Serial.println("SPIFFS FORMAT END");

  // ファイルを開く
  Serial.println("SPIFFS FILE OPEN");
  File fp = SPIFFS.open(FILENAME, FILE_WRITE);

  // 書き込み
  Serial.println("SPIFFS FILE WRITE START");
  fp.write(file, sizeof(file));
  fp.close();
  Serial.println("SPIFFS FILE WRITE END");

  // 保存できているか確認
  fp = SPIFFS.open(FILENAME, FILE_READ);
  size_t size = fp.size();
  Serial.printf("SPIFFS FILE SIZE = %d\n", size);
}

void loop() {
}
