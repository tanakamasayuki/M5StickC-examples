#include <M5StickC.h>
#include "efontEnableAll.h"
//#include "efontEnableAscii.h"
//#include "efontEnableCJK.h"
//#include "efontEnableJa.h"
#include "efont.h"
#include "efontM5StickC.h"

void setup() {
  M5.begin();
  M5.Lcd.setRotation(0);
  M5.Lcd.setCursor(0, 0);

  printEfont("Hello", 0, 16*0);
  printEfont("こんにちは", 0, 16*1);
  printEfont("你好", 0, 16*2);
  printEfont("안녕하세요", 0, 16*3);
  printEfont("Доброе утро", 0, 16*4);
  printEfont("Päivää", 0, 16*6);
  printEfont("Здравствуйте", 0, 16*7);
}

void loop() {
}
