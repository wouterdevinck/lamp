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
      void setAllLeds(LedValue color) override;
      void setAllLeds(LedValue values[]) override;
      void setBrightness(uint8_t brightness) override;
      void setBrightness(uint8_t values[]) override;
      ChainInfo getChainInfo() override;

    private:
      SPI* _spi;
      queue<KeyFrame> _frames;
      bool _ready;
      void transferNextKeyframe();

  };

}