#pragma once

#include "IWiFiHandler.h"
#include "IWiFiClient.h"
#include "INvs.h"
#include "IHttpServer.h"
#include "IHttpHandler.h"
#include "IIotFramework.h"
#include "IIotHandler.h"
#include "IRgbLed.h"

namespace lamp {

  class WiFiHandler : public IWiFiHandler {

    public:
      explicit WiFiHandler(IWiFiClient* wifi, INvs* storage, 
        IHttpServer* httpserver, int port, IHttpHandler* httphandler,
        IIotFramework* iot, IIotHandler* iothandler, IRgbLed* led) :
        _wifi(wifi), _storage(storage), 
        _httpserver(httpserver), _port(port), _httphandler(httphandler),
        _iot(iot), _iothandler(iothandler), _led(led) {}

      void onConnected() override;
      void onDisconnected() override;
      void onSmartConfig(string ssid, string password) override;

    private:
      IWiFiClient* _wifi;
      INvs* _storage;
      IHttpServer* _httpserver;
      int _port;
      IHttpHandler* _httphandler;
      IIotFramework* _iot;
      IIotHandler* _iothandler;
      IRgbLed* _led;

  };

}
