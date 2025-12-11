#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ms01 { 

// Наследуемся от cg::Component и PollingComponent
class MS01Component : public Component, public PollingComponent {
 public:
  void set_pin(GPIOPin *pin) { pin_ = pin; }
  
  // Объявляем сенсоры публичными, чтобы их можно было создать в setup()
  sensor::Sensor *humidity_sensor_{nullptr};

  void setup() override;
  void update() override;

 protected:
  float readDHT_tasmota_logic();

 private:
    GPIOPin *pin_;
};

}  // namespace ms01
}  // namespace esphome
