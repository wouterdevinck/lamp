#pragma once

#include "IWiFiClient.h"
#include "IWiFiHandler.h"
#include "Storage.h"
#include "esp_event_loop.h"

namespace lamp {

  class WiFiClient : public IWiFiClient {

    public:
      explicit WiFiClient(Storage* _);

      void setHandler(IWiFiHandler* handler) override;
      void connect(string ssid, string pwd) override;
      void reconnect() override;
      void startSmartConfig() override;

    private:
      static IWiFiHandler* _handler;
      static esp_err_t eventHandler(void *ctx, system_event_t *event);

  };

}