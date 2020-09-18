#include <driver/i2s.h>
#include <M5Core2.h>

#include "wav1.h"
#include "wav2.h"
#include "wav3.h"
#include "wav4.h"
#include "wav5.h"

const unsigned char *wavList[] = {wav1, wav2, wav3, wav4, wav5};
const size_t wavSize[] = {sizeof(wav1), sizeof(wav2), sizeof(wav3), sizeof(wav4), sizeof(wav5)};

#define CONFIG_I2S_BCK_PIN      12
#define CONFIG_I2S_LRCK_PIN     0
#define CONFIG_I2S_DATA_PIN     2
#define CONFIG_I2S_DATA_IN_PIN  34

#define SPEAKER_I2S_NUMBER      I2S_NUM_0

#define MODE_MIC                0
#define MODE_SPK                1

void InitI2SSpeakerOrMic(int mode)
{
  esp_err_t err = ESP_OK;

  i2s_driver_uninstall(SPEAKER_I2S_NUMBER);
  i2s_config_t i2s_config = {
    .mode                 = (i2s_mode_t)(I2S_MODE_MASTER),
    .sample_rate          = 16000,
    .bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format       = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count        = 6,
    .dma_buf_len          = 60,
    .use_apll             = false,
    .tx_desc_auto_clear   = true,
    .fixed_mclk           = 0
  };
  if (mode == MODE_MIC) {
    i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
  } else {
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
  M5.begin();
  delay(50);
  Serial.println();
  InitI2SSpeakerOrMic(MODE_MIC);
  delay(2000);
}

void loop() {
  TouchPoint_t pos = M5.Touch.getPressPoint();
  if (pos.x != -1) {
    size_t bytes_written;

    M5.Axp.SetSpkEnable(true);
    InitI2SSpeakerOrMic(MODE_SPK);

    // Random Play
    int wav = random(5);

    // Write Speaker
    i2s_write(SPEAKER_I2S_NUMBER, wavList[wav], wavSize[wav], &bytes_written, portMAX_DELAY);
    i2s_zero_dma_buffer(SPEAKER_I2S_NUMBER);

    // Set Mic Mode
    InitI2SSpeakerOrMic(MODE_MIC);
    M5.Axp.SetSpkEnable(false);
  }
}
