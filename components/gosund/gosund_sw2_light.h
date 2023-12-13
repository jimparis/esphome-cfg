#pragma once
#include "esphome.h"

#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/light/light_state.h"
#include "esphome/components/light/light_traits.h"

namespace esphome {
namespace gosund_sw2 {

class GosundSW2Output : public light::LightOutput, public uart::UARTDevice, public Component {
 public:
  // LightOutput methods
  light::LightTraits get_traits() override;
  void setup_state(light::LightState *state) override { this->light_state_ = state; }
  void write_state(light::LightState *state) override;

  // Component methods
  void setup() override{};
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return esphome::setup_priority::DATA; }

  // Misc setup called by code gen
  void set_status_led(output::BinaryOutput *status_led) { status_led_ = status_led; }

 protected:
  light::LightState *light_state_{nullptr};
  bool set_by_touch_ = false;
  output::BinaryOutput *status_led_;
  uint8_t buf[5] = {0};
};

} // namespace gosund
} // namespace esphome
