#pragma once

// TODO logs

#include "ILedBoardChain.h"

#define LEDMAX 4095
#define BRIMAX 127

namespace lamp {

  enum LampColor {
    Red = 0,
    Green = 1,
    Blue = 2,
    White = 3
  };

  class LedsManager {

    public:
      explicit LedsManager(ILedBoardChain* leds) : _leds(leds) {}

      void start() const;
      void changeColor(LampColor color) const;

    private:
      ILedBoardChain* _leds;
      const static LedValue LampColors[];

  };

}