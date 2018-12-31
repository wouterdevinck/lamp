#pragma once

#include "ITiming.h"

namespace lamp {

  class Timing : public ITiming {

    public:
      explicit Timing();

      uint32_t millis() override;
      void delay(uint32_t ms) override;

  };

}