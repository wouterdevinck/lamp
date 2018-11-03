#pragma once

#include <cstdint>

#include "IRgbLed.h"
#include "IIrReceiver.h"
#include "IrHandler.h"

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
        IIrReceiver* ir, 
        IHttpServer* httpserver, 
        IHttpClient* httpclient, 
        IRgbLed* led, 
        ILedBoardChain* leds
      );
      #else
      explicit Lamp(
        IIrReceiver* ir, 
        IRgbLed* led
      );
      #endif

      void start(int port) const;

    private:
      IIrReceiver* _ir; 
      IRgbLed* _led;
      IIrHandler* _irhandler;
      #ifndef BASIC
      IUpdater* _updater;
      ILogger* _logger;
      IHttpServer* _httpserver;
      IHttpClient* _httpclient;
      ILedBoardChain* _leds;
      IHttpHandler* _httphandler;
      UpgradeManager* _upgrade;
      #endif

  };

}