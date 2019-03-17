#pragma once

#include "IIotClient.h"

namespace lamp {

  class AzureIotClient : public IIotClient {

    public:
      explicit AzureIotClient();
      
  };

}