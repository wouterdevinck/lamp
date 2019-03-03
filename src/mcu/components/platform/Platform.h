#pragma once

#include "IPlatform.h"

#include "IrReceiver.h"
#include "RgbLed.h"
#include "LedBoardChain.h"
#include "WiFiClient.h"
#include "Updater.h"
#include "Logger.h"
#include "HttpServer.h"
#include "HttpClient.h"
#include "Storage.h"

#include "SPI.h"

namespace lamp {

  class Platform : public IPlatform {

    public:
      explicit Platform();

      IIrReceiver* getIrReceiver() override;
      IRgbLed* getRgbLed() override;
      ILedBoardChain* getLedBoardChain() override;
      IWiFiClient* getWiFiClient() override;
      IUpdater* getUpdater() override;
      ILogger* getLogger() override;
      IHttpServer* getHttpServer() override;
      IHttpClient* getHttpClient() override;
      INvs* getStorage() override;
      int getHttpServerPort() override;

    private:
      SPI* _spi1;
      IrReceiver* _ir;
      RgbLed* _led;
      LedBoardChain* _leds;
      WiFiClient* _wifi;
      Updater* _updater;
      Logger* _logger;
      HttpServer* _httpserver;
      HttpClient* _httpclient;
      Storage* _storage;

  };

}