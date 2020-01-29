#include <Preferences.h>

Preferences preferences;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  char wifi_ssid[37] = {};
  char wifi_key[66] = {};
  preferences.begin("nvs.net80211", true);
  preferences.getBytes("sta.ssid", wifi_ssid, sizeof(wifi_ssid));
  preferences.getBytes("sta.pswd", wifi_key, sizeof(wifi_key));

  Serial.printf("sta.ssid : %s\n", &wifi_ssid[4]);
  Serial.printf("sta.pswd : %s\n", wifi_key);
}

void loop() {
}
