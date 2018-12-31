#pragma once

// TODO logs

#include "ILedBoardChain.h"
#include "ITiming.h"

#define BRIINIT 100
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
      explicit LedsManager(ILedBoardChain* leds, ITiming* timing) : 
        _leds(leds), _timing(timing) { 
          _bri = BRIINIT; 
          _color = LampColor::White;
      }

      void start() const;
      void changeColor(LampColor color);
      void increaseBrightness();
      void decreaseBrightness();
      void test() const;

    private:
      ILedBoardChain* _leds;
      ITiming* _timing;
      uint8_t _bri;
      LampColor _color;
      const static LedValue LampColors[];

  };

}