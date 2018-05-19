#pragma once

#include "IHttpHandler.h"
#include "IRgbLed.h"

namespace lamp {

  class HttpHandler : public IHttpHandler {

    public:
      explicit HttpHandler(IRgbLed* led) : _led(led) {}
      string handleHttpRequest(string method, string path) override;

    private:
      IRgbLed* _led;

  };

}
