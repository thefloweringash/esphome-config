#pragma once

#include <TM1640.h>

namespace esphome {
namespace light {

class TM16xxLight : public AddressableLight {
  public:
    TM16xxLight(TM16xx *controller, int32_t num_leds) {
      this->controller_ = controller;
      this->num_leds_ = num_leds;

      this->leds_ = new esphome::Color[num_leds];
    };

    inline int32_t size() const override { return this->num_leds_; }

    LightTraits get_traits() override {
      auto traits = LightTraits();
      traits.set_supported_color_modes({ColorMode::BRIGHTNESS});
      return traits;
    }

  protected:
    void setup() override {
      // Global display settings (on/off, brightness) are set on every
      // write_state, so no setupDisplay() is required here.
    }
    void dump_config() override {
      ESP_LOGCONFIG("tm16xx", "TM16xxLight:");
      ESP_LOGCONFIG("tm16xx", "  Num LEDs: %u", this->num_leds_);
    };

    ESPColorView get_view_internal(int32_t index) const override {
      return {
        &this->leds_[index].r, &this->leds_[index].g, &this->leds_[index].b, &this->leds_[index].w,
        nullptr, &this->correction_,
      };
    }

    void write_state(LightState *state) {
      // FIXME: AddressableLight transitions try to operate per led, but we
      // only have global brightness. These global properties will apply at the
      // end of the transition.

      bool binary;
      float brightness;

      state->current_values_as_binary(&binary);
      state->current_values_as_brightness(&brightness);

      // this->controller_->setupdisplay(binary, 1 + int(brightness * 6));
      this->controller_->setupDisplay(binary, 2);

      int8_t segment = 0;

      for (int32_t i = 0; i < this->num_leds_;) {
        int8_t buf = 0;
        // Take up to 8 bits
        for (int32_t j = 0; j < 8 && i < this->num_leds_; j++) {
          buf |= this->leds_[i++].is_on() << j;
        }
        // emit whatever we took
        this->controller_->setSegments(buf, segment++);
      }
    }
    void clear_effect_data() {}; // no effects

  private:
    TM16xx *controller_{nullptr};
    int num_leds_{0};
    esphome::Color *leds_;

};

}
}
