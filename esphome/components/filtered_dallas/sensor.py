import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_NAME

DEPENDENCIES = ["sensor"]

filtered_dallas_ns = cg.esphome_ns.namespace("filtered_dallas")
FilteredDallasSensor = filtered_dallas_ns.class_(
    "FilteredDallasSensor", cg.PollingComponent, sensor.Sensor
)

CONF_ADDRESS = "address"

CONFIG_SCHEMA = sensor.sensor_schema().extend(
    {
        cv.GenerateID(): cv.declare_id(FilteredDallasSensor),
        cv.Required(CONF_ADDRESS): cv.hex_uint64_t,
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_ADDRESS])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)