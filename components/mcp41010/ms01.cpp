#include "ms01.h"
#include "esphome/core/log.h"
#include <Arduino.h>

namespace esphome {
namespace ms01 {

static const char *const TAG = "ms01";

void MS01Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up custom MS01 sensor on pin %d...", pin_->get_pin());
  // Инициализация сенсора влажности
  humidity_sensor_ = new sensor::Sensor();
  humidity_sensor_->set_name("MS01 Humidity");
  humidity_sensor_->set_unit_of_measurement("%");
  humidity_sensor_->set_accuracy_decimals(1);
  humidity_sensor_->set_icon("mdi:water-percent");

  // Установка интервала опроса (30 секунд)
  this->set_update_interval(30000);
}

void MS01Component::update() {
  float humidity = readDHT_tasmota_logic();
  if (!isnan(humidity)) {
    if (humidity_sensor_) {
      humidity_sensor_->publish_state(humidity);
      ESP_LOGD(TAG, "Humidity: %.1f%%", humidity);
    }
  } else {
    ESP_LOGW(TAG, "Communication failed! Check pull-up resistor or timing.");
    if (humidity_sensor_) {
      humidity_sensor_->publish_state(NAN);
    }
  }
}

float MS01Component::readDHT_tasmota_logic() {
  // Временные переменные для хранения битов
  uint8_t data[5] = {0};

  // Здесь должен быть код битбэнгинга для чтения данных с датчика
  // ...

  // Проверка контрольной суммы (пример)
  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    // Преобразование сырых данных в значение влажности
    uint16_t raw_humidity = (data[0] << 8) | data[1];
    return static_cast<float>(raw_humidity) / 10.0f;
  } else {
    ESP_LOGW(TAG, "Checksum error in humidity data");
    return NAN;
  }
}

}  // namespace ms01
}  // namespace esphome
