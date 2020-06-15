#include <M5Atom.h>
#include "SPIFFS.h"

#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

#define CONFIG_I2S_BCK_PIN      19
#define CONFIG_I2S_LRCK_PIN     33
#define CONFIG_I2S_DATA_PIN     22

#define FILENAME "/11-fanfare.mp3"

void setup()
{
  M5.begin(true, false, true);
  delay(50);
  Serial.println();
  SPIFFS.begin();

  Serial.printf("MP3 playback begins...\n");

  audioLogger = &Serial;
  file = new AudioFileSourceSPIFFS(FILENAME);
  id3 = new AudioFileSourceID3(file);
  out = new AudioOutputI2S();
  out->SetPinout(CONFIG_I2S_BCK_PIN, CONFIG_I2S_LRCK_PIN, CONFIG_I2S_DATA_PIN);
  out->SetChannels(1);
  out->SetGain(0.3);
  mp3 = new AudioGeneratorMP3();
}

void loop()
{
  M5.update();
  if (M5.Btn.isPressed())
  {
    if (!mp3->isRunning()) {
      Serial.println("Play");
      mp3->begin(id3, out);
    }
  }

  if (mp3->isRunning()) {
    if (!mp3->loop()){
      mp3->stop();
    }
  }
  delay(1);
}
