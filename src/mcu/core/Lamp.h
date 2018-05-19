#pragma once

#include "IUpdater.h"
#include "IHttpServer.h"
#include "IRgbLed.h"
#include "ILedBoardChain.h"
#include "HttpHandler.h"

#include <cstdint>

namespace lamp {

  class Lamp {

    public:
      explicit Lamp(IUpdater* updater, IHttpServer* http, IRgbLed* led, ILedBoardChain* leds) : 
        _updater(updater), _http(http), _handler(new HttpHandler(led)), _led(led), _leds(leds) {}

      void start(int port) const;

    private:
      IUpdater* _updater;
      IHttpServer* _http;
      IHttpHandler* _handler;
      IRgbLed* _led;
      ILedBoardChain* _leds;

  };

}
