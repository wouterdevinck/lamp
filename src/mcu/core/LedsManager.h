#pragma once

// TODO logs

#include "ILedBoardChain.h"

#define BRIINIT 10
#define BRISTEP 5
#define BRIMAX 127

#define LEDMAX 4095

namespace lamp {

  enum struct LampColor : uint8_t {
    Red = 0,
    Green = 1,
    Blue = 2,
    White = 3
  };

  class LedsManager {

    public:
      explicit LedsManager(ILedBoardChain* leds) : _leds(leds) { 
        _bri = BRIINIT; 
        _color = LampColor::White;
      }

      void start() const;
      void changeColor(LampColor color);
      void increaseBrightness();
      void decreaseBrightness();

    private:
      ILedBoardChain* _leds;
      uint8_t _bri;
      LampColor _color;
      const static LedValue LampColors[];

  };

}