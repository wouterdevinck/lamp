#pragma once

#include "IWiFiClient.h"
#include "IWiFiHandler.h"
#include "Storage.h"
#include "esp_event.h"

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
      static void eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

  };

}