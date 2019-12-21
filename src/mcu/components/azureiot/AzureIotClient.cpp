#include "AzureIotClient.h"

#include "iothub.h"
#include "iothub_client_version.h"
#include "iothub_device_client_ll.h"
#include "iothubtransportmqtt.h"

using namespace lamp;

AzureIotClient::AzureIotClient(string hubUrl, string deviceId, IIotHandler* handler, ILogger* logger) {
  _hubUrl = hubUrl;
  _deviceId = deviceId;
  _handler = handler;
  _logger = logger;
}

void AzureIotClient::connect() {
  ::IoTHub_Init();
  _logger->logInfo(_tag, "Iothub API Version: " + string(::IoTHubClient_GetVersionString()));
  auto statusCallback = [] (IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* ctx) -> void { 
    (void)reason;
    if (result == IOTHUB_CLIENT_CONNECTION_AUTHENTICATED) {
      auto that = (AzureIotClient*)ctx;

      // printf("IOTHUB_CLIENT_CONNECTION_AUTHENTICATED\n");
      // that->_logger->logInfo(_tag, "IOTHUB_CLIENT_CONNECTION_AUTHENTICATED");

    }
  };
  IOTHUB_DEVICE_CLIENT_LL_HANDLE handle;
  if ((handle = ::IoTHubDeviceClient_LL_CreateFromDeviceAuth(_hubUrl.c_str(), _deviceId.c_str(), ::MQTT_Protocol)) == NULL) {
    // printf("failed create IoTHub client from connection string %s!\r\n", user_ctx.iothub_uri);
  } else {
    ::IoTHubDeviceClient_LL_SetConnectionStatusCallback(handle, statusCallback, NULL);//&iothub_info);
    
    // ::IoTHubDeviceClient_LL_SetMessageCallback(handle, receive_msg_callback, &iothub_info);

    // IOTHUB_MESSAGE_HANDLE msg_handle = ::IoTHubMessage_CreateFromByteArray((const unsigned char*)msgText, strlen(msgText));
    // ::IoTHubDeviceClient_LL_SendEventAsync(handle, msg_handle, NULL, NULL) != IOTHUB_CLIENT_OK)
    // ::IoTHubMessage_Destroy(msg_handle);
                    
    // ::IoTHubDeviceClient_LL_DoWork(handle);
    // ::ThreadAPI_Sleep(1);
          
    //::IoTHubDeviceClient_LL_Destroy(handle);
    //::IoTHub_Deinit();
  }
}

