import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID

DEPENDENCIES = ["sensor"]

filtered_dallas_ns = cg.esphome_ns.namespace("filtered_dallas")

FilteredDallasSensor = filtered_dallas_ns.class_(
    "FilteredDallasSensor",
    sensor.Sensor,
)

CONF_SPIKE_THRESHOLD = "spike_threshold"
CONF_MAX_REJECTS = "max_rejects"

CONFIG_SCHEMA = sensor.sensor_schema(
    FilteredDallasSensor
).extend(
    {
        cv.GenerateID(): cv.declare_id(FilteredDallasSensor),

        cv.Optional(CONF_SPIKE_THRESHOLD, default=15.0): cv.float_,

        cv.Optional(CONF_MAX_REJECTS, default=3): cv.int_,
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await sensor.register_sensor(var, config)

    cg.add(var.set_spike_threshold(config[CONF_SPIKE_THRESHOLD]))
    cg.add(var.set_max_rejects(config[CONF_MAX_REJECTS]))