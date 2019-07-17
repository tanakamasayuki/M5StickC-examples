#include <M5StickC.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
uint64_t chipid;
char chipname[256];

void setup() {
  chipid = ESP.getEfuseMac();
  sprintf( chipname, "M5StickC_%04X", (uint16_t)(chipid >> 32));
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.printf("Bluetooth: %s\n", chipname);
  M5.Lcd.printf("Ver: %s %s\n", __DATE__, __TIME__);
  M5.Lcd.println();
  M5.Lcd.printf("Val:");

  SerialBT.begin(chipname);
}

void loop() {
  int val = analogRead(33);

  Serial.println(val);
  SerialBT.println(val);
  M5.Lcd.setCursor(8*4, 8*3);
  M5.Lcd.printf("%4d", val);
  delay(500);
}
