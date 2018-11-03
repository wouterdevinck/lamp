#pragma once

#include "IIrHandler.h"

namespace lamp {

  struct IIrHandler;

  struct IIrReceiver {

    virtual ~IIrReceiver() noexcept = default;

    virtual void start(IIrHandler* handler) = 0;

  };

}
