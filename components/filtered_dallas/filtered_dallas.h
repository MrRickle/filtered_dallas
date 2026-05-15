#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/log.h"
#include <cmath>
#include <limits>

namespace esphome {
namespace filtered_dallas {

class FilteredDallasSensor : public sensor::Sensor {
 public:
  void input(float x) {
    if (std::isnan(x))
      return;

    // first valid reading
    if (std::isnan(last_good_)) {
      last_good_ = x;
      publish_state(x);
      return;
    }

    float delta = fabsf(x - last_good_);

    if (delta > spike_threshold_) {
      reject_count_++;

      ESP_LOGW("filtered_dallas",
               "Spike rejected %.2f (last good %.2f, count %d)",
               x, last_good_, reject_count_);

      if (reject_count_ >= max_rejects_) {
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

  void set_spike_threshold(float t) { spike_threshold_ = t; }
  void set_max_rejects(int r) { max_rejects_ = r; }

 protected:
  float last_good_{NAN};
  float spike_threshold_{15.0f};
  int max_rejects_{3};
  int reject_count_{0};
};

}  // namespace filtered_dallas
}  // namespace esphome