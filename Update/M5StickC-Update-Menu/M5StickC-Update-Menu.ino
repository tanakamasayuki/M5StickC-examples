#include <M5StickC.h>
#include <WiFi.h>
#include <Update.h>

WiFiClient client;

int menuIndex = 0;
String menuTitle[] = {"NTP", "LovyanGFX MovingIcons"};
String menuBin[] = {"/bin/M5StickC-Update-NTP.ino.m5stick_c.bin", "/bin/M5StickC-Update-MovingIcons.ino.m5stick_c.bin"};
int menuCount = 2;

// Utility to extract header value from headers
String getHeaderValue(String header, String headerName) {
  return header.substring(strlen(headerName.c_str()));
}

// OTA Logic
void execOTA(String host, int port, String bin) {
  Serial.println("Connecting to Wi-fi");

  // Connect to provided SSID and PSWD
  WiFi.begin();

  // Wait for connection to establish
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); // Keep the serial monitor lit!
    delay(500);
  }

  // Connection Succeed
  Serial.println("");
  Serial.println("Connected to Wi-Fi");

  long contentLength = 0;
  bool isValidContentType = false;

  Serial.println("Connecting to: " + String(host));
  if (client.connect(host.c_str(), port)) {
    Serial.println("Fetching Bin: " + String(bin));

    client.print(String("GET ") + bin + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Cache-Control: no-cache\r\n" +
                 "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) {
      String line = client.readStringUntil('\n');
      line.trim();

      if (!line.length()) {
        break;
      }

      if (line.startsWith("HTTP/1.1")) {
        if (line.indexOf("200") < 0) {
          Serial.println("Got a non 200 status code from server. Exiting OTA Update.");
          break;
        }
      }

      if (line.startsWith("Content-Length: ")) {
        contentLength = atol((getHeaderValue(line, "Content-Length: ")).c_str());
        Serial.println("Got " + String(contentLength) + " bytes from server");
      }

      if (line.startsWith("Content-Type: ")) {
        String contentType = getHeaderValue(line, "Content-Type: ");
        Serial.println("Got " + contentType + " payload.");
        if (contentType == "application/octet-stream") {
          isValidContentType = true;
        }
      }
    }
  } else {
    Serial.println("Connection to " + String(host) + " failed. Please check your setup");
  }

  Serial.println("contentLength : " + String(contentLength) + ", isValidContentType : " + String(isValidContentType));

  if (contentLength && isValidContentType) {
    bool canBegin = Update.begin(contentLength);

    if (canBegin) {
      Serial.println("Begin OTA. This may take 2 - 5 mins to complete. Things might be quite for a while.. Patience!");
      size_t written = Update.writeStream(client);

      if (written == contentLength) {
        Serial.println("Written : " + String(written) + " successfully");
      } else {
        Serial.println("Written only : " + String(written) + "/" + String(contentLength) + ". Retry?" );
      }

      if (Update.end()) {
        Serial.println("OTA done!");
        if (Update.isFinished()) {
          Serial.println("Update successfully completed. Rebooting.");
          ESP.restart();
        } else {
          Serial.println("Update not finished? Something went wrong!");
        }
      } else {
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
    } else {
      Serial.println("Not enough space to begin OTA");
      client.flush();
    }
  } else {
    Serial.println("There was no content in the response");
    client.flush();
  }
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("M5StickC Update Menu");

  M5.Lcd.setCursor(0, 8 * 3);
  for (int i = 0; i < menuCount; i++) {
    M5.Lcd.printf(" %s\n", menuTitle[i].c_str());
  }
  M5.Lcd.setCursor(0, 80 - 16);
  M5.Lcd.println("BtnA: Run");
  M5.Lcd.println("BtnB: Select");
}

void loop() {
  M5.update();
  M5.Lcd.setCursor(0, 8 * 3);
  for (int i = 0; i < menuCount; i++) {
    if (menuIndex == i) {
      M5.Lcd.println(">");
    } else {
      M5.Lcd.println(" ");
    }
  }

  if (M5.BtnA.wasPressed()) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("APP OTA");

    String host = "hub.lang-ship.com";
    int port = 80;
    String bin = menuBin[menuIndex];
    execOTA(host, port, bin);
  }
  if (M5.BtnB.wasPressed()) {
    menuIndex++;
    menuIndex = menuIndex % menuCount;
  }

  delay(1);
}
