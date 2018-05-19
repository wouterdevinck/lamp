#pragma once

#include <cstdint>

namespace lamp {

  struct RgbLedColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };

  struct IRgbLed {

    virtual ~IRgbLed() noexcept = default;

    virtual void setLedColor(RgbLedColor color) = 0;

  };

}
