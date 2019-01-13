#pragma once

#include "IrHandler.h"
#include "LedsManager.h"
#include "IPlatform.h"

#ifndef BASIC
#include "WiFiHandler.h"
#include "HttpHandler.h"
#include "UpgradeManager.h"
#endif

namespace lamp {

  class Lamp {

    public:
      explicit Lamp(IPlatform* platform);
      void start() const;

    private:
      IPlatform* _platform;
      IIrHandler* _irhandler;
      LedsManager* _manager;
      #ifndef BASIC
      WiFiHandler* _wifihandler;
      HttpHandler* _httphandler;
      UpgradeManager* _upgrade;
      #endif

  };

}