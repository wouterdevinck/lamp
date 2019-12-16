#pragma once

#include "IIotFramework.h"
#include "ILogger.h"
#include "DpsClient.h"
#include "constants.h"

namespace lamp {

  class AzureIot : public IIotFramework {

    public:
      explicit AzureIot(ILogger* loggger);

      IIotClient* connect(IIotHandler* handler) override;

    private:
      ILogger* _logger;
      DpsClient* _dps;
      
      const string _tag = "AzureIot";

      const string _dpsUrl = IOT_DPS_URL;
      const string _dpsIdScope = IOT_DPS_IDS;

  };

}