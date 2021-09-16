#pragma once

class LGFX_ESP32_ST7789_SPI : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7789  _panel_instance;
    lgfx::Bus_SPI       _bus_instance;
    lgfx::Light_PWM     _light_instance;

  public:

    LGFX_ESP32_ST7789_SPI(int width, int height, int sclk, int mosi, int rst, int dc, int cs, int bl, int miso = -1, int busy = -1)
    {
      {
        auto cfg = _bus_instance.config();

        cfg.spi_host = VSPI_HOST;
        if (cs == -1) {
          cfg.spi_mode = 3;
        } else {
          cfg.spi_mode = 0;
        }
        cfg.freq_write = 40000000;
        cfg.freq_read  = 15000000;
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

        cfg.memory_width     = 240;
        cfg.memory_height    = 320;
        cfg.panel_width      = width;
        cfg.panel_height     = height;
        if (width == 80 && height == 160) {
          // 0.96
          cfg.offset_x       = 52;
          cfg.offset_y       = 40;
        } else {
          cfg.offset_x       = 0;
          cfg.offset_y       = 0;
        }
        cfg.offset_rotation  = 2;
        cfg.dummy_read_pixel = 16;
        cfg.dummy_read_bits  = 1;
        if (cs == -1) {
          cfg.readable         = false;
        } else {
          cfg.readable         = true;
        }
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
