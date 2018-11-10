#pragma once

#include "IHttpHandler.h"
#include "IRgbLed.h"
#include "UpgradeManager.h"
#include "LedsManager.h"

namespace lamp {

  class HttpHandler : public IHttpHandler {

    public:
      explicit HttpHandler(IRgbLed* led, UpgradeManager* upgrade, LedsManager* leds) :
        _led(led), _upgrade(upgrade), _leds(leds) {}
      string handleHttpRequest(string method, string path) override;

    private:
      IRgbLed* _led;
      UpgradeManager* _upgrade;
      LedsManager* _leds;

  };

}
