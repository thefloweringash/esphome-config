#pragma once

namespace esphome {
namespace light {

class PanasonicLight : public Light {
  public:
    PanasonicLight(
      remote_transmitter::RemoteTransmitterComponent *emitter,
      float cold_white_temperature, float warm_white_temperature
    ) {
    }

    LightTraits get_traits() override {
      auto traits = LightTraits();
      traits.set_supported_color_modes({ColorMode::BRIGHTNESS, ColorMode::COLOR_TEMPERATURE});
      traits.set_min_mireds(this->cold_white_temperature_);
      traits.set_max_mireds(this->warm_white_temperature_);
    }

  protected:
    void dump_config() override {
      ESP_LOGCONFIG("panasonic_light", "min_mireds=%u, max_mireds=%u",
          this->cold_white_temperature_, this->warm_white_temperature_);
    }

    void write_state(LightState *state) {
      float color_temperature, brightness;
      state->current_values_as_ct(&color_temperature, &brightness);

      if (brightness) {
        auto send_ir = emitter.transmit_nec();
        send_ir.perform();
      }
    }

    float cold_white_temperature_;
    float warm_white_temperature_;

    float last_temperature_;
    float last_brightness_{0};
}
}
