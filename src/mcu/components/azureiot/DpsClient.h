#pragma once

#include <string>
#include "ILogger.h"

using namespace std;

namespace lamp {

  struct DpsResult {
    string url;
    string deviceid;
  };

  class DpsClient {

    public:
      explicit DpsClient(ILogger* logger);

      DpsResult* Register(string idscope);
      
    private:
      ILogger* _logger;

      const string _url = "global.azure-devices-provisioning.net";
      const string _tag = "AzureIot.DpsClient";

  };

  struct DpsContext {
    DpsClient* dps;
    DpsResult* result;
    bool registered;
    bool completed;
  };

}