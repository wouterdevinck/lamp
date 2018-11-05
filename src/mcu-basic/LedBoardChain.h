#pragma once

#include "ILedBoardChain.h"

namespace lamp {

  class LedBoardChain : public ILedBoardChain {

    public:
      explicit LedBoardChain();

      void setAllLeds(LedValue color) override;
      void setAllLeds(LedValue values[]) override;
      void setBrightness(uint8_t brightness) override;
      void setBrightness(uint8_t values[]) override;
      ChainInfo getChainInfo() override;

  };

}