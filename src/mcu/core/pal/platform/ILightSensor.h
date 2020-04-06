#pragma once

#include <cstdint>

using namespace std;

namespace lamp {

  struct ILightSensor {

    virtual ~ILightSensor() noexcept = default;

    virtual uint32_t getIlluminanceInLux() = 0;

  };

}