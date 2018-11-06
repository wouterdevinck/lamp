#pragma once

#include "ILedBoardChain.h"

namespace lamp {

  class LedBoardChain : public ILedBoardChain {

    public:
      explicit LedBoardChain(uint8_t boards);

      void setAllLeds(LedValue color) override;
      void setAllLeds(LedValue values[]) override;
      void setBrightness(uint8_t brightness) override;
      void setBrightness(uint8_t values[]) override;
      ChainInfo* getChainInfo() override;

    private:
      ChainInfo* _info;

      void setAllLeds(LedValue(*value)(uint8_t));
      void setBrightness(uint8_t(*value)(uint8_t));

  };

}