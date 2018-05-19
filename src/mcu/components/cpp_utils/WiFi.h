// Inspired by: https://github.com/nkolban/esp32-snippets/tree/master/cpp_utils/WiFi.h

#pragma once

#include <esp_err.h>
#include "FreeRTOS.h"
#include "WiFiEventHandler.h"

class WiFi {

  public:
    WiFi();
    void connectAP(const std::string& ssid, const std::string& password);
    void setWifiEventHandler(WiFiEventHandler *wifiEventHandler);

  private:
    static esp_err_t eventHandler(void* ctx, system_event_t* event);
    void init();
    WiFiEventHandler* m_pWifiEventHandler;
    bool m_eventLoopStarted;
    bool m_initCalled;
  	FreeRTOS::Semaphore m_connectFinished = FreeRTOS::Semaphore("ConnectFinished");

};