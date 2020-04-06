#pragma once

#include "ILightSensor.h"
#include "TSL2591.h"

namespace lamp {

  class LightSensor : public ILightSensor {

    public:
      explicit LightSensor();

      uint32_t getIlluminanceInLux() override;

    private:
      TSL2591 _tsl;

  };

}