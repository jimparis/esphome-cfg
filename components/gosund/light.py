import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, light, output
from esphome.const import (
    CONF_OUTPUT_ID,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
)

DEPENDENCIES = ["uart", "light"]

gosund_sw2_ns = cg.esphome_ns.namespace("gosund_sw2")
GosundSW2Output = gosund_sw2_ns.class_(
    "GosundSW2Output", cg.Component, uart.UARTDevice, light.LightOutput
)

CONFIG_SCHEMA = (
    light.BRIGHTNESS_ONLY_LIGHT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(GosundSW2Output),
            cv.Required("status_led"): cv.use_id(output.BinaryOutput),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)

FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "gosund_sw2", baud_rate=115200, require_tx=True, require_rx=True
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    await light.register_light(var, config)
    await output.register_output(var, config)

    status_led_ = await cg.get_variable(config["status_led"])
    cg.add(var.set_status_led(status_led_))
