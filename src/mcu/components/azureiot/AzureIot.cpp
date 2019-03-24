#include "AzureIot.h"
#include "AzureIotClient.h"

using namespace lamp;

AzureIot::AzureIot(ILogger* logger) {
  _logger = logger;
  _dps = new DpsClient(logger);
}

// TODO Make it possible to store registration info to avoid re-doing DPS?

IIotClient* AzureIot::connect(IIotHandler* handler) {
  auto reg = _dps->Register(_idscope);
  _logger->logInfo(_tag, "IoTHub URL: " + reg->url);

/*
  ::IoTHub_Init();
  _logger->logInfo(_tag, "Iothub API Version: " + string(::IoTHubClient_GetVersionString()));

  auto statusCallback = [] (IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* ctx) -> void { 
    (void)reason;
    if (result == IOTHUB_CLIENT_CONNECTION_AUTHENTICATED) {
      auto that = (AzureIot*)ctx;

    }
  };

  IOTHUB_DEVICE_CLIENT_LL_HANDLE handle;
  if ((handle = ::IoTHubDeviceClient_LL_CreateFromDeviceAuth(reg->url, reg->deviceid, MQTT_Protocol)) == NULL) {
    // printf("failed create IoTHub client from connection string %s!\r\n", user_ctx.iothub_uri);
  } else {
    ::IoTHubDeviceClient_LL_SetConnectionStatusCallback(handle, statusCallback, &iothub_info);
    
    // ::IoTHubDeviceClient_LL_SetMessageCallback(handle, receive_msg_callback, &iothub_info);

    // IOTHUB_MESSAGE_HANDLE msg_handle = ::IoTHubMessage_CreateFromByteArray((const unsigned char*)msgText, strlen(msgText));
    // ::IoTHubDeviceClient_LL_SendEventAsync(handle, msg_handle, NULL, NULL) != IOTHUB_CLIENT_OK)
    // ::IoTHubMessage_Destroy(msg_handle);
                    
    // ::IoTHubDeviceClient_LL_DoWork(handle);
    // ::ThreadAPI_Sleep(1);
          
    //::IoTHubDeviceClient_LL_Destroy(handle);
    //::IoTHub_Deinit();
  }
*/

  return new AzureIotClient();
}

// IoTHubDeviceClient_LL_SetOption(handle, OPTION_TRUSTED_CERT, certificates);
// IoTHubDeviceClient_LL_SetOption(handle, OPTION_LOG_TRACE, true);