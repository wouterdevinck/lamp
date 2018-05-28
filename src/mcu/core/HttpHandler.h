#pragma once

#include "IHttpHandler.h"
#include "IRgbLed.h"
#include "UpgradeManager.h"

namespace lamp {

  class HttpHandler : public IHttpHandler {

    public:
      explicit HttpHandler(IRgbLed* led, UpgradeManager* upgrade) :
        _led(led), _upgrade(upgrade) {}
      string handleHttpRequest(string method, string path) override;

    private:
      IRgbLed* _led;
      UpgradeManager* _upgrade;

  };

}
