#pragma once

#include "IWiFiHandler.h"
#include "IWiFiClient.h"
#include "IStorage.h"
#include "IHttpServer.h"
#include "IHttpHandler.h"

namespace lamp {

  class WiFiHandler : public IWiFiHandler {

    public:
      explicit WiFiHandler(IWiFiClient* wifi, IStorage* storage, IHttpServer* httpserver, int port, IHttpHandler* httphandler) :
        _wifi(wifi), _storage(storage), _httpserver(httpserver), _port(port), _httphandler(httphandler) {}

      void onConnected() override;
      void onDisconnected() override;
      void onSmartConfig(string ssid, string password) override;

    private:
      IWiFiClient* _wifi;
      IStorage* _storage;
      IHttpServer* _httpserver;
      int _port;
      IHttpHandler* _httphandler;

  };

}
