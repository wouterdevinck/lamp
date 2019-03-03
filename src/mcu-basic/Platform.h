#pragma once

#include "IPlatform.h"
#include "IrReceiver.h"
#include "RgbLed.h"
#include "LedBoardChain.h"

namespace lamp {

  class Platform : public IPlatform {

    public:
      explicit Platform(uint8_t boards);

      IIrReceiver* getIrReceiver() override;
      IRgbLed* getRgbLed() override;
      ILedBoardChain* getLedBoardChain() override;

      void loop();

    private:
      IrReceiver* _ir;
      RgbLed* _led;
      LedBoardChain* _leds;

  };

}