#pragma once

class LGFX_ESP32_SSD1306_I2C : public lgfx::LGFX_Device
{
    lgfx::Panel_SSD1306 _panel_instance;
    lgfx::Bus_I2C       _bus_instance;

  public:

    LGFX_ESP32_SSD1306_I2C(int gnd, int vcc, int scl, int sda, int i2c_port = 0, int i2c_addr = 0x3C)
    {
      if (gnd != -1) {
        // GND
        pinMode(gnd, OUTPUT);
        digitalWrite(gnd, LOW);
      }
      if (vcc != -1) {
        // VCC
        pinMode(vcc, OUTPUT);
        digitalWrite(vcc, HIGH);
      }

      {
        auto cfg = _bus_instance.config();

        cfg.pin_scl = scl;
        cfg.pin_sda = sda;
        cfg.i2c_port = i2c_port;
        cfg.i2c_addr  = i2c_addr;

        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
      }

      {
        auto cfg = _panel_instance.config();
        _panel_instance.config(cfg);
      }

      setPanel(&_panel_instance);
    }
};
