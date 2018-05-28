#pragma once

#include "IUpdater.h"
#include "IHttpServer.h"
#include "IHttpClient.h"
#include "IRgbLed.h"
#include "ILedBoardChain.h"
#include "HttpHandler.h"
#include "UpgradeManager.h"

#include <cstdint>

// #include <memory>
// TODO use shared_ptr

namespace lamp {

  class Lamp {

    public:
      explicit Lamp(
        IUpdater* updater, 
        IHttpServer* httpserver, 
        IHttpClient* httpclient, 
        IRgbLed* led, 
        ILedBoardChain* leds
      );

      void start(int port) const;

    private:
      IUpdater* _updater;
	  IHttpServer* _httpserver;
	  IHttpClient* _httpclient;
	  IRgbLed* _led;
	  ILedBoardChain* _leds;
	  IHttpHandler* _handler;
	  UpgradeManager* _upgrade;

  };

}
