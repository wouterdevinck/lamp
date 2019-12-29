#pragma once

#include "IIotHandler.h"
#include "IRgbLed.h"

namespace lamp {

  class IotHandler : public IIotHandler {

    public:
      explicit IotHandler(IRgbLed* led) : _led(led) {}
      
      string handleDirectMethod(string method, string payload) override;

    private:
      IRgbLed* _led;

  };

}
