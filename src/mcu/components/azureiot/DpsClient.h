#pragma once

#include <string>
#include "ILogger.h"

using namespace std;

namespace lamp {

  class DpsClient {

    public:
      explicit DpsClient(ILogger* logger);

      string Register(string idscope);
      
    private:
      ILogger* _logger;

      const string _url = "global.azure-devices-provisioning.net";
      const string _tag = "AzureIot.DpsClient";

  };

  struct DpsContext {
    DpsClient* dps;
    string url;
    string deviceid;
    bool registered;
  };

}