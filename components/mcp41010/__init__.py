import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_PIN

ms01_ns = cg.esphome_ns.namespace("ms01")
# Наследуемся от cg.Component, как в рабочем примере MCP41010
MS01Component = ms01_ns.class_("MS01Component", cg.Component)

CONFIG_SCHEMA = ( 
    cv.Schema({    
        cv.GenerateID(): cv.declare_id(MS01Component),
        cv.Required(CONF_PIN): cv.int_, # Обязательный пин для данных (ожидается целое число)
    })
    .extend(cv.COMPONENT_SCHEMA) # Используем базовую схему компонента
    # Опрос и сенсоры определяются в C++ коде
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    # ИСПРАВЛЕНИЕ: Передаем номер пина напрямую в функцию set_pin() C++ класса.
    # Эта функция ожидает int, который мы получаем из config[CONF_PIN].
    cg.add(var.set_pin(config[CONF_PIN]))
    
    # С этого момента данные будут отправлены из C++ кода компоненту-шаблону.

