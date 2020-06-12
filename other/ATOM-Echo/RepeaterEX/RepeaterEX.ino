/*Press button to record,released button to playback*/
// https://github.com/m5stack/M5-ProductExampleCodes/blob/master/Core/Atom/AtomEcho/Arduino/Repeater/Repeater.ino

#include <driver/i2s.h>
#include <M5Atom.h>

#define CONFIG_I2S_BCK_PIN      19
#define CONFIG_I2S_LRCK_PIN     33
#define CONFIG_I2S_DATA_PIN     22
#define CONFIG_I2S_DATA_IN_PIN  23

#define SPEAKER_I2S_NUMBER      I2S_NUM_0

#define MODE_MIC                0
#define MODE_SPK                1
#define DATA_SIZE               1024

#define DATA_PRINT              0
#define REC_DELAY_MS            200

uint8_t microphonedata0[1024 * 80];
int data_offset = 0;
int16_t data_min;
int16_t data_max;

void InitI2SSpeakerOrMic(int mode)
{
  esp_err_t err = ESP_OK;

  i2s_driver_uninstall(SPEAKER_I2S_NUMBER);
  i2s_config_t i2s_config = {
    .mode                 = (i2s_mode_t)(I2S_MODE_MASTER),
    .sample_rate          = 16000,
    .bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
    .channel_format       = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count        = 6,
    .dma_buf_len          = 60,
    .use_apll             = false,
    .tx_desc_auto_clear   = true,
    .fixed_mclk           = 0
  };
  if (mode == MODE_MIC)
  {
    i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
  }
  else
  {
    i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  }

  err += i2s_driver_install(SPEAKER_I2S_NUMBER, &i2s_config, 0, NULL);

  i2s_pin_config_t tx_pin_config = {
    .bck_io_num           = CONFIG_I2S_BCK_PIN,
    .ws_io_num            = CONFIG_I2S_LRCK_PIN,
    .data_out_num         = CONFIG_I2S_DATA_PIN,
    .data_in_num          = CONFIG_I2S_DATA_IN_PIN,
  };
  err += i2s_set_pin(SPEAKER_I2S_NUMBER, &tx_pin_config);

  if (mode != MODE_MIC) {
    err += i2s_set_clk(SPEAKER_I2S_NUMBER, 16000, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
  }

  i2s_zero_dma_buffer(SPEAKER_I2S_NUMBER);
}

void setup() {
  M5.begin(true, false, true);
  delay(50);
  Serial.println();
  M5.dis.drawpix(0, CRGB(128, 128, 0));
  InitI2SSpeakerOrMic(MODE_MIC);
  delay(2000);
}

void loop() {
  if (M5.Btn.isPressed())
  {
    size_t byte_read;
    data_offset = 0;
    data_min = 32767;
    data_max = -32768;

    // Delay Rec
    delay(REC_DELAY_MS);

    // Red Color
    M5.dis.drawpix(0, CRGB(0, 128, 0));

    while (1)
    {
      uint8_t recBuffer[DATA_SIZE];
      i2s_read(SPEAKER_I2S_NUMBER, (char*)(recBuffer), DATA_SIZE, &byte_read, (100 / portTICK_RATE_MS));
      for (int i = 0; i < byte_read; i += 4) {
        if (data_offset < sizeof(microphonedata0)) {
          int16_t* val = (int16_t*)&recBuffer[i];
          int16_t* p = (int16_t*)&microphonedata0[data_offset];
          *p = *val;
          data_min = min(data_min, *p);
          data_max = max(data_max, *p);
          data_offset += 2;
        }
      }
      M5.update();
      if (M5.Btn.isReleased())
        break;
    }

    M5.dis.drawpix(0, CRGB(128, 128, 0));

    size_t bytes_written;
    InitI2SSpeakerOrMic(MODE_SPK);

    // Auto Gain
    int16_t *p = (int16_t*)microphonedata0;
    for (int i = 0; i < data_offset / 2; i++) {
      *p = map(*p, data_min, data_max, -32768 * 0.8, 32767 * 0.8);
      p++;
    }
    Serial.printf("data_min  : %d\n", data_min);
    Serial.printf("data_max  : %d\n", data_max);

    // Write Speaker
    i2s_write(SPEAKER_I2S_NUMBER, microphonedata0, data_offset, &bytes_written, portMAX_DELAY);
    i2s_zero_dma_buffer(SPEAKER_I2S_NUMBER);

    // Set Mic Mode
    InitI2SSpeakerOrMic(MODE_MIC);

    // Data print
    if (DATA_PRINT) {
      for (int i = 0; i < data_offset; i++) {
        Serial.printf("%02X", microphonedata0[i]);
      }
      Serial.println();
    }
  }

  M5.update();
}
