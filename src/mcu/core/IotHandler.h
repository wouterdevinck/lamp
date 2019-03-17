#pragma once

#include "IIotHandler.h"

namespace lamp {

  class IotHandler : public IIotHandler {

    public:
      explicit IotHandler() {}
      
      string handleDirectMethod(string method, string payload) override;

  };

}
