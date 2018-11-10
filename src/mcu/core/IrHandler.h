#pragma once

#include "IIrHandler.h"
#include "IRgbLed.h"
#include "LedsManager.h"

namespace lamp {

  class IrHandler : public IIrHandler {

    public:
      explicit IrHandler(IRgbLed* led, LedsManager* leds) : _led(led), _leds(leds) {}
      void handleIrCommand(uint16_t code) override;

    private:
      IRgbLed* _led;
      LedsManager* _leds;

  };

}
