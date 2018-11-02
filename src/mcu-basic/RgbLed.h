#pragma once

#include "IRgbLed.h"

namespace lamp {

  class RgbLed : public IRgbLed {

    public:
      explicit RgbLed();

      void setLedColor(RgbLedColor color) override;

  };

}