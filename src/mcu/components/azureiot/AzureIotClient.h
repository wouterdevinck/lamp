#pragma once

#include <string>

#include "IIotClient.h"
#include "IIotHandler.h"
#include "ILogger.h"

using namespace std;

namespace lamp {

  class AzureIotClient : public IIotClient {

    public:
      explicit AzureIotClient(string hubUrl, string deviceId, IIotHandler* handler, ILogger* logger);

      void connect();

    private:
      string _hubUrl;
      string _deviceId;
      IIotHandler* _handler;
      ILogger* _logger;
      
      const string _tag = "AzureIot.AzureIotClient";

  };

}