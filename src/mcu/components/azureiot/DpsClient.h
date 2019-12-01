#pragma once

#include <string>
#include "ILogger.h"

using namespace std;

namespace lamp {

  class DpsClient;

  struct DpsResult {
    string url;
    string deviceid;
  };

  struct DpsContext {
    DpsClient* dps;
    DpsResult* result;
    bool registered;
    bool completed;
  };

  class DpsClient {

    public:
      explicit DpsClient(ILogger* logger);

      DpsContext* Register(string url, string idscope);
      
    private:
      ILogger* _logger;

      const string _tag = "AzureIot.DpsClient";

  };

}