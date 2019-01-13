#pragma once

#include <string>

using namespace std;

namespace lamp {

  struct IWiFiHandler {

    virtual ~IWiFiHandler() noexcept = default;

    virtual void onConnected() = 0;
    virtual void onDisconnected() = 0;
    virtual void onSmartConfig(string ssid, string password) = 0;

  };

}
