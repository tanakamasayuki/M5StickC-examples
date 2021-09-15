#pragma once

class LGFX_ESP32_ST7735S_SPI : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7735S _panel_instance;
    lgfx::Bus_SPI       _bus_instance;
    lgfx::Light_PWM     _light_instance;

  public:

    LGFX_ESP32_ST7735S_SPI(int width, int height, int sclk, int mosi, int rst, int dc, int cs, int bl, int miso = -1, int busy = -1)
    {
      {
        auto cfg = _bus_instance.config();

        cfg.spi_host = VSPI_HOST;
        cfg.spi_mode = 0;
        cfg.freq_write = 20000000;
        cfg.freq_read  = 16000000;
        cfg.spi_3wire  = true;
        cfg.use_lock   = true;
        cfg.dma_channel = 1;
        cfg.pin_sclk = sclk;
        cfg.pin_mosi = mosi;
        cfg.pin_miso = miso;
        cfg.pin_dc   = dc;

        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
      }

      {
        auto cfg = _panel_instance.config();

        cfg.pin_cs           = cs;
        cfg.pin_rst          = rst;
        cfg.pin_busy         = busy;

        cfg.memory_width     = 132;
        cfg.memory_height    = 162;
        cfg.panel_width      = width;
        cfg.panel_height     = height;
        cfg.offset_x         = 26;
        cfg.offset_y         = 1;
        cfg.offset_rotation  = 2;
        cfg.dummy_read_pixel = 8;
        cfg.dummy_read_bits  = 1;
        cfg.readable         = true;
        cfg.invert           = true;
        cfg.rgb_order        = false;
        cfg.dlen_16bit       = false;
        cfg.bus_shared       = true;

        _panel_instance.config(cfg);
      }

      {
        auto cfg = _light_instance.config();

        cfg.pin_bl = bl;
        cfg.invert = false;
        cfg.freq   = 44100;
        cfg.pwm_channel = 7;

        _light_instance.config(cfg);
        _panel_instance.setLight(&_light_instance);
      }

      setPanel(&_panel_instance);
    }
};
