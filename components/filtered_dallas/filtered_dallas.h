#pragma once

#include "esphome.h"
#include <cmath>        // Required for std::isnan

namespace esphome {
namespace filtered_dallas {

class FilteredDallasSensor : public PollingComponent, public sensor::Sensor {
 public:
  FilteredDallasSensor(uint64_t address) : address_(address) {}

  void set_parent(sensor::Sensor *parent) { parent_ = parent; }

  void setup() override {}

  void update() override {
    if (parent_ == nullptr) return;

    float x = parent_->state;

  //  if (std::isnan(x)) return;

  //  if (std::isnan(last_good_)) {
      last_good_ = x;
      publish_state(x);
      return;
    }

    float delta = fabsf(x - last_good_);

    if (delta > 15.0f) {
      reject_count_++;

      ESP_LOGW("filtered_dallas", "Spike rejected %.2f (last good %.2f)", x, last_good_);

      if (reject_count_ >= 3) {
        last_good_ = x;
        reject_count_ = 0;
      }

      publish_state(last_good_);
      return;
    }

    reject_count_ = 0;
    last_good_ = x;
    publish_state(x);
  }

 protected:
  uint64_t address_;
  sensor::Sensor *parent_{nullptr};

  float last_good_{NAN};
  int reject_count_{0};
};

}  // namespace filtered_dallas
}  // namespace esphome