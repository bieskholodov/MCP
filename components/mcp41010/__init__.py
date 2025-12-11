import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID, 
    CONF_PIN, 
    CONF_HUMIDITY, 
    UNIT_PERCENT, 
    ICON_WATER, 
    CONF_TEMPERATURE, 
    UNIT_CELSIUS, 
    ICON_THERMOMETER
)

# DEPENDENCIES = ["spi"] # Не нужно для этого компонента

ms01_dht_ns = cg.esphome_ns.namespace("ms01_dht")
# Определяем класс компонента MS01DHTComponent, наследуемся от PollingComponent
MS01DHTComponent = ms01_dht_ns.class_("MS01DHTComponent", cg.PollingComponent)

CONFIG_SCHEMA = ( 
    cv.Schema({    
        cv.GenerateID(): cv.declare_id(MS01DHTComponent),
        # Используем cv.int_ для номера пина, как в исправленной версии
        cv.Required(CONF_PIN): cv.int_, 
        # Определяем опции для сенсоров влажности и температуры
        cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(UNIT_PERCENT, ICON_WATER, 0),
        cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 1),
    })
    .extend(cv.COMPONENT_SCHEMA) # Используем COMPONENT_SCHEMA вместо polling_component_schema напрямую
    # .extend(spi.spi_device_schema(cs_pin_required=True)) # Не нужно, это не SPI устройство
    .extend(cv.polling_component_schema("30s")) # Добавляем интервал опроса
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    # Регистрация пина
    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))

    # Регистрация сенсоров (влажность и температура)
    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))
    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
