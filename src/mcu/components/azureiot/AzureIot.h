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
      
      const string _idscope = "0ne00094B68";
      const string _tag = "AzureIot";

  };

}