#pragma once

#include "IIrReceiver.h"
#include "IRgbLed.h"
#include "ILedBoardChain.h"

#ifndef BASIC
#include "IWiFiClient.h"
#include "ILightSensor.h"
#include "IUpdater.h"
#include "ILogger.h"
#include "IHttpServer.h"
#include "IHttpClient.h"
#include "IIotFramework.h"
#include "INvs.h"
#endif

namespace lamp {

  struct IPlatform {

    virtual ~IPlatform() noexcept = default;

    virtual IIrReceiver* getIrReceiver() = 0;
    virtual IRgbLed* getRgbLed() = 0;
    virtual ILedBoardChain* getLedBoardChain() = 0;

    #ifndef BASIC
    virtual IWiFiClient* getWiFiClient() = 0;
    virtual ILightSensor* getLightSensor() = 0;
    virtual IUpdater* getUpdater() = 0;
    virtual ILogger* getLogger() = 0;
    virtual IHttpServer* getHttpServer() = 0;
    virtual IHttpClient* getHttpClient() = 0;
    virtual IIotFramework* getIotFramework() = 0;
    virtual INvs* getStorage() = 0;
    virtual int getHttpServerPort() = 0;
    #endif

  };

}
