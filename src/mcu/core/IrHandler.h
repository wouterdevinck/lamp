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

  enum struct IrCommand : uint16_t {
    // Row 1
    BrightnessUp = 0x00A0,
    BrightnessDown = 0x0020,
    Off = 0x0060,
    On = 0x00E0,
    // Row 2
    Red = 0x0090,
    Green1 = 0x0010,
    Blue4 = 0x0050,
    White = 0x00D0,
    // Row 3
    Orange1 = 0x00B0,
    Green2 = 0x0030,
    Blue5 = 0x0070,
    Flash = 0x00F0,
    // Row 4
    Orange2 = 0x00A8,
    Blue1 = 0x0028,
    Purple1 = 0x0068,
    Strobe = 0x00E8,
    // Row 5
    Orange3 = 0x0098,
    Blue2 = 0x0018,
    Purple2 = 0x0058,
    Fade = 0x00D8,
    // Row 6
    Yellow = 0x0088,
    Blue3 = 0x0008,
    Purple3 = 0x0048,
    Smooth = 0x00C8
  };
  
}
