import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_PIN
from esphome.components import gpio  # Добавляем импорт для работы с пинами

ms01_ns = cg.esphome_ns.namespace("ms01")
MS01Component = ms01_ns.class_("MS01Component", cg.Component)

CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(MS01Component),
        cv.Required(CONF_PIN): gpio.gpio_pin_schema,  # Используем валидатор из gpio
    })
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # Создаём объект GPIOPin из конфигурации
    pin = await gpio.new_gpio_pin(config[CONF_PIN])
    
    # Передаем указатель на GPIOPin в метод set_pin()
    cg.add(var.set_pin(pin))
