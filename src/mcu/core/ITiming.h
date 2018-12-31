#pragma once

#include <cstdint>

namespace lamp {

  struct ITiming {

    virtual ~ITiming() noexcept = default;

    virtual uint32_t millis() = 0;
    virtual void delay(uint32_t ms) = 0;

  };

}
