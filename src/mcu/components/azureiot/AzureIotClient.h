#pragma once

#include <string>

#include "iothub_device_client_ll.h"
#include "iothub_message.h"

#include "IIotClient.h"
#include "IIotHandler.h"
#include "ILogger.h"

using namespace std;

namespace lamp {

  struct IotContext {
    ILogger* logger;
    string tag;
    IIotHandler* handler;
    IOTHUB_DEVICE_CLIENT_LL_HANDLE handle;
  };

  class AzureIotClient : public IIotClient {

    public:
      explicit AzureIotClient(string hubUrl, string deviceId, IIotHandler* handler, ILogger* logger);

    private:
      IotContext* _ctx;

      static void doWork(void* ctx);
      static void statusCallback(IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* ctx);
      static IOTHUBMESSAGE_DISPOSITION_RESULT messageCallback(IOTHUB_MESSAGE_HANDLE message, void* ctx);
      
      const string _tag = "AzureIot.AzureIotClient";

  };

}