#pragma once

#include <cstdint>

namespace lamp {

  struct IIrHandler {

    virtual ~IIrHandler() noexcept = default;

    virtual void handleIrCommand(uint16_t code) = 0;

  };

}
