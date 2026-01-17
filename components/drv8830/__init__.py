import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import i2c

CODEOWNERS = ["@andrewsnodgrass"]

DEPENDENCIES = ["i2c"]

drv8830_ns = cg.esphome_ns.namespace("drv8830")
DRV8830 = drv8830_ns.class_("DRV8830", cg.Component)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(DRV8830),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x60))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    # Get I2C bus and address from config
    i2c_bus = await cg.get_variable(config[i2c.CONF_I2C_ID])
    cg.add(var.set_i2c_bus(i2c_bus))
    cg.add(var.set_i2c_address(config[i2c.CONF_ADDRESS]))
