#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ms01 {

/**
 * Компонент для работы с датчиком MS01 (аналог DHTxx).
 * Реализует чтение влажности через GPIO с периодическим опросом.
 */
class MS01Component : public Component, public PollingComponent {
 public:
  /**
   * Устанавливает пин для подключения датчика.
   * @param pin Указатель на объект GPIOPin
   */
  void set_pin(GPIOPin *pin) { pin_ = pin; }

  /**
   * Сенсор влажности. Может быть сконфигурирован во внешнем YAML.
   */
  sensor::Sensor *humidity_sensor_{nullptr};

  // Методы жизненного цикла компонента
  void setup() override;
  void update() override;

 protected:
  /**
   * Основной метод чтения данных с датчика по протоколу DHT (Tasmota-логика).
   * @return Значение влажности в процентах или NAN при ошибке
   */
  float readDHT_tasmota_logic();

 private:
  GPIOPin *pin_;  ///< Пин подключения датчика
};

}  // namespace ms01
}  // namespace esphome

