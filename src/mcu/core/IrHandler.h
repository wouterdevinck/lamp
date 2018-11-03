#pragma once

#include "IIrHandler.h"
#include "IRgbLed.h"

namespace lamp {

  class IrHandler : public IIrHandler {

    public:
      explicit IrHandler(IRgbLed* led) : _led(led) {}
      void handleIrCommand(uint16_t code) override;

    private:
      IRgbLed* _led;

  };

}
