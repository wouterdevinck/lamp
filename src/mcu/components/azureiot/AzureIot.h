#pragma once

#include "IIotFramework.h"

namespace lamp {

  class AzureIot : public IIotFramework {

    public:
      explicit AzureIot();

      IIotClient* connect(IIotHandler* handler) override;

  };

}