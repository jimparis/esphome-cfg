#include "gosund_sw2_light.h"

namespace esphome {
namespace gosund_sw2 {

static const char *TAG = "gosund.light.sw2";
static const byte ON_MASK = 0x80;

static const uint8_t MIN_VALUE = 0;
static const uint8_t MAX_VALUE = 100;

light::LightTraits GosundSW2Output::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::BRIGHTNESS});
  return traits;
}

void GosundSW2Output::write_state(light::LightState *state) {
  // don't go into an infinite loop when set by touch
  if (set_by_touch_) {
    set_by_touch_ = false;
    return;
  }

  auto values = state->current_values;
  uint8_t output = (uint8_t) roundf(100 * values.get_brightness());
  if (output > 100) {
    output = 100;
  }

  if (values.get_state() > 0 && values.get_brightness() > 0) {
    output += ON_MASK;
    status_led_->turn_on();
  } else {
    status_led_->turn_off();
  }

  this->write_byte(output);

  ESP_LOGD(TAG, "write_state() called with state: %0.1f, brightness: %.02f => output: %02X",
           values.get_state(), values.get_brightness(), output);
}

void GosundSW2Output::loop() {
  // pattern is 0x24 0xYY 0x01 0x1E 0x23, where YY is dimmer value
  while (this->available()) {
    memmove(&buf[0], &buf[1], 4);
    this->read_byte(&buf[4]);

    if (buf[0] == 0x24 && buf[2] == 0x01 && buf[3] == 0x1e && buf[4] == 0x23) {
      float brightness = buf[1] / 100.0;
      if (brightness > 1)
        brightness = 1;

      set_by_touch_ = true;
      auto call = light_state_->make_call();
      // Touch sensor only works when turned on
      call.set_state(true);
      call.set_brightness(brightness);
      call.perform();

      ESP_LOGD(TAG, "Dimmer value %d", buf[1]);
    }
  }
}

void GosundSW2Output::dump_config() {
  ESP_LOGCONFIG(TAG, "Gosund SW2");
}

} // namespace gosund
} // namespace esphome
