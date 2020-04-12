#pragma once

#include "IWiFiHandler.h"

namespace lamp {

  struct IWifiHandler;

  struct IWiFiClient {

    virtual ~IWiFiClient() noexcept = default;

    virtual void setHandler(IWiFiHandler* handler) = 0;
    virtual void connect(string ssid, string pwd) = 0;
    virtual void reconnect() = 0;
    virtual void startSmartConfig() = 0;

  };

}