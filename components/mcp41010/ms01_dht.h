#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/log.h"
#include <Arduino.h> // Явно подключаем Arduino functions

namespace esphome {
namespace ms01_dht {

class MS01DHTComponent : public PollingComponent {
 public:
  void set_pin(GPIOPin *pin) { pin_ = pin; }
  void set_humidity_sensor(sensor::Sensor *s) { humidity_sensor_ = s; }
  void set_temperature_sensor(sensor::Sensor *s) { temperature_sensor_ = s; }

  void setup() override {
    ESP_LOGCONFIG("ms01_dht", "Setting up custom MS01 DHT sensor...");
    // setup() вызывается после инициализации пинов ESPHome
  }

  void update() override {
    // Вся логика чтения перенесена сюда, чтобы упростить синтаксис C++
    float humidity = readDHT();
    if (!isnan(humidity)) {
      if (humidity_sensor_) humidity_sensor_->publish_state(humidity);
    } else {
      ESP_LOGW("ms01_dht", "Communication failed!");
      if (humidity_sensor_) humidity_sensor_->publish_state(NAN);
    }
    // MS01 не выдает температуру, всегда NAN
    if (temperature_sensor_) temperature_sensor_->publish_state(NAN);
  }
  
 private:
    GPIOPin *pin_;
    sensor::Sensor *humidity_sensor_{nullptr};
    sensor::Sensor *temperature_sensor_{nullptr};

    // Функция чтения данных
    float readDHT() {
      // Битбэнгинг Tasmota/Adafruit
      uint8_t bits[5] = {0, 0, 0, 0, 0};
      
      // Step 1: Send start signal
      pinMode(pin_->get_pin(), OUTPUT);
      digitalWrite(pin_->get_pin(), LOW);
      delayMicroseconds(450); 
      digitalWrite(pin_->get_pin(), HIGH);
      delayMicroseconds(30);
      pinMode(pin_->get_pin(), INPUT_PULLUP);
      
      // Step 2: Read data (simplified timing check)
      unsigned long timeout = 1000;
      while(digitalRead(pin_->get_pin()) == HIGH && (micros() - timeout) < 1000); // Wait for sensor to pull low

      for (int i = 0; i < 40; i++) {
          unsigned long lowCycle = micros();
          while(digitalRead(pin_->get_pin()) == LOW && (micros() - lowCycle) < timeout);
          unsigned long highCycle = micros();
          while(digitalRead(pin_->get_pin()) == HIGH && (micros() - highCycle) < timeout);
          
          if ((highCycle - lowCycle) > 40) bits[i/8] |= (1 << (7 - (i % 8)));
      }
      
      // Step 3: Verify checksum
      if (bits[4] != ((bits[0] + bits[1] + bits[2] + bits[3]) & 0xFF)) {
          return NAN; // Checksum fail
      }

      // Return humidity value
      unsigned int rawHumidity = bits[0] << 8 | bits[1];
      return rawHumidity / 10.0;
    }
};

}  // namespace ms01_dht
}  // namespace esphome
