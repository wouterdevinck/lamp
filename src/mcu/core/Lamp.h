#pragma once

#include <cstdint>

#include "IIrReceiver.h"
#include "IrHandler.h"
#include "IRgbLed.h"
#include "ILedBoardChain.h"
#include "ITiming.h"
#include "LedsManager.h"

#ifndef BASIC
#include "IUpdater.h"
#include "ILogger.h"
#include "IHttpServer.h"
#include "IHttpClient.h"
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
        ILedBoardChain* leds,
        ITiming* timing
      );
      #else
      explicit Lamp(
        IIrReceiver* ir, 
        IRgbLed* led,
        ILedBoardChain* leds,
        ITiming* timing
      );
      #endif

      void start(int port) const;

    private:
      IIrReceiver* _ir;
      IIrHandler* _irhandler;
      IRgbLed* _led;
      ILedBoardChain* _leds;
      ITiming* _timing;
      LedsManager* _manager;
      #ifndef BASIC
      IUpdater* _updater;
      ILogger* _logger;
      IHttpServer* _httpserver;
      IHttpClient* _httpclient;
      IHttpHandler* _httphandler;
      UpgradeManager* _upgrade;
      #endif

  };

}