#pragma once

#include "IWiFiHandler.h"
#include "IWiFiClient.h"
#include "INvs.h"
#include "IHttpServer.h"
#include "IHttpHandler.h"

namespace lamp {

  class WiFiHandler : public IWiFiHandler {

    public:
      explicit WiFiHandler(IWiFiClient* wifi, INvs* storage, IHttpServer* httpserver, int port, IHttpHandler* httphandler) :
        _wifi(wifi), _storage(storage), _httpserver(httpserver), _port(port), _httphandler(httphandler) {}

      void onConnected() override;
      void onDisconnected() override;
      void onSmartConfig(string ssid, string password) override;

    private:
      IWiFiClient* _wifi;
      INvs* _storage;
      IHttpServer* _httpserver;
      int _port;
      IHttpHandler* _httphandler;

  };

}
