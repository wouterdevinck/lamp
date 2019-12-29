#pragma once

#include <string>

#include "iothub_device_client_ll.h"

#include "IIotClient.h"
#include "IIotHandler.h"
#include "ILogger.h"

using namespace std;

namespace lamp {

  class AzureIotClient : public IIotClient {

    public:
      explicit AzureIotClient(string hubUrl, string deviceId, IIotHandler* handler, ILogger* logger);

      ILogger* getLogger();

    private:
      IIotHandler* _handler;
      ILogger* _logger;

      IOTHUB_DEVICE_CLIENT_LL_HANDLE _handle;

      static void doWork(void* parameter);
      
      const string _tag = "AzureIot.AzureIotClient";

  };

}