#pragma once

#include <queue>

#include "ILedBoardChain.h"
#include "SPI.h"

#define FRAMERATE (120)

namespace lamp {

  enum Command {
    SendFrame = 0
  };

  class LedBoardChain : public ILedBoardChain {

    public:
      explicit LedBoardChain(SPI* spi, int pinInt);

      void addKeyframe(KeyFrame keyframe) override;

    private:
      SPI* _spi;
      queue<KeyFrame> _frames;
      bool _ready;
      void transferNextKeyframe();

  };

}