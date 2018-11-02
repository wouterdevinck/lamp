#pragma once

#include <cstdint>

#include "IRgbLed.h"

#ifndef BASIC
#include "IUpdater.h"
#include "ILogger.h"
#include "IHttpServer.h"
#include "IHttpClient.h"
#include "ILedBoardChain.h"
#include "HttpHandler.h"
#include "UpgradeManager.h"
// #include <memory>
// TODO use shared_ptr
#endif

namespace lamp {

  class Lamp {

    public:
      #ifndef BASIC
      explicit Lamp(
        IUpdater* updater, 
        ILogger* logger,
        IHttpServer* httpserver, 
        IHttpClient* httpclient, 
        IRgbLed* led, 
        ILedBoardChain* leds
      );
      #else
      explicit Lamp(
        IRgbLed* led
      );
      #endif

      void start(int port) const;

    private:
      IRgbLed* _led;
      #ifndef BASIC
      IUpdater* _updater;
      ILogger* _logger;
      IHttpServer* _httpserver;
      IHttpClient* _httpclient;
      ILedBoardChain* _leds;
      IHttpHandler* _handler;
      UpgradeManager* _upgrade;
      #endif

  };

}