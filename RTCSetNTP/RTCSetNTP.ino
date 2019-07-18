#include <M5StickC.h>
#include <WiFi.h>
#include "time.h"
#include <Preferences.h>

Preferences preferences;
char wifi_ssid[33];
char wifi_key[65];

const char* ntpServer =  "ntp.nict.jp";

RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

void setup() {
  M5.begin();

  // Wi-Fiアクセスポイント情報取得
  preferences.begin("Wi-Fi", true);
  preferences.getString("ssid", wifi_ssid, sizeof(wifi_ssid));
  preferences.getString("key", wifi_key, sizeof(wifi_key));
  preferences.end();
  
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0, 2);
  M5.Lcd.println("RTC TEST");

  // connect to WiFi
  Serial.printf("Connecting to %s ", wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_key);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  // Set ntp time to local
  configTime(9 * 3600, 0, ntpServer);

  // Get local time
  struct tm timeInfo;
  if (getLocalTime(&timeInfo)) {
    // Set RTC time
    RTC_TimeTypeDef TimeStruct;
    TimeStruct.Hours   = timeInfo.tm_hour;
    TimeStruct.Minutes = timeInfo.tm_min;
    TimeStruct.Seconds = timeInfo.tm_sec;
    M5.Rtc.SetTime(&TimeStruct);

    RTC_DateTypeDef DateStruct;
    DateStruct.WeekDay = timeInfo.tm_wday;
    DateStruct.Month = timeInfo.tm_mon + 1;
    DateStruct.Date = timeInfo.tm_mday;
    DateStruct.Year = timeInfo.tm_year + 1900;
    M5.Rtc.SetData(&DateStruct);
  }

  //disconnect WiFi
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop() {
  M5.Rtc.GetTime(&RTC_TimeStruct);
  M5.Rtc.GetData(&RTC_DateStruct);
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.printf("Data: %04d-%02d-%02d\n", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Date);
  M5.Lcd.printf("Week: %d\n", RTC_DateStruct.WeekDay);
  M5.Lcd.printf("Time: %02d : %02d : %02d\n", RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
  delay(500);
}
