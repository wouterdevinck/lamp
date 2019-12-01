#pragma once

#include "IIotFramework.h"
#include "ILogger.h"
#include "DpsClient.h"

namespace lamp {

  class AzureIot : public IIotFramework {

    public:
      explicit AzureIot(ILogger* loggger);

      IIotClient* connect(IIotHandler* handler) override;

    private:
      ILogger* _logger;
      DpsClient* _dps;
      
      const string _tag = "AzureIot";

      const string _dpsUrl = "global.azure-devices-provisioning.net";
      const string _dpsIdScope = "0ne00094B68";

  };

}