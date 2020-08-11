#pragma once

#include "IPlatform.h"

#include "IrReceiver.h"
#include "RgbLed.h"
#include "LedBoardChain.h"
#include "WiFiClient.h"
#include "LightSensor.h"
#include "Updater.h"
#include "Logger.h"
#include "HttpServer.h"
#include "HttpClient.h"
#include "AzureIot.h"
#include "Storage.h"
#include "Microphone.h"

#include "Spi.h"
#include "SpiFlash.h"

namespace lamp {

  class Platform : public IPlatform {

    public:
      explicit Platform();

      IIrReceiver* getIrReceiver() override;
      IRgbLed* getRgbLed() override;
      ILedBoardChain* getLedBoardChain() override;
      IWiFiClient* getWiFiClient() override;
      ILightSensor* getLightSensor() override;
      IUpdater* getUpdater() override;
      ILogger* getLogger() override;
      IHttpServer* getHttpServer() override;
      IHttpClient* getHttpClient() override;
      IIotFramework* getIotFramework() override;
      INvs* getStorage() override;
      // IMicrophone* getMicrophone() override; // TODO
      int getHttpServerPort() override;

    private:
      Spi* _spi1;
      SpiFlash* _spi2;
      IrReceiver* _ir;
      RgbLed* _led;
      LedBoardChain* _leds;
      WiFiClient* _wifi;
      LightSensor* _lux;
      Updater* _updater;
      Logger* _logger;
      HttpServer* _httpserver;
      HttpClient* _httpclient;
      AzureIot* _iot;
      Storage* _storage;
      Microphone* _microphone;

  };

}