#include "AzureIotClient.h"

#include "iothub.h"
#include "iothubtransportmqtt.h"
#include "iothub_client_version.h"
#include "iothub_message.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace lamp;

AzureIotClient::AzureIotClient(string hubUrl, string deviceId, IIotHandler* handler, ILogger* logger) {
  _handler = handler;
  _logger = logger;
  ::IoTHub_Init();
  _logger->logInfo(_tag, "Iothub API Version: " + string(::IoTHubClient_GetVersionString()));
  // TODO use capturing lambda trick? or context struct
  auto statusCallback = [] (IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* ctx) -> void { 
    (void)reason;
    if (result == IOTHUB_CLIENT_CONNECTION_AUTHENTICATED) {
      auto that = (AzureIotClient*)ctx;
      auto logger = that->getLogger();
      logger->logInfo("statusCallback", "IOTHUB_CLIENT_CONNECTION_AUTHENTICATED");
    }
  };
  auto messageCallback = [] (IOTHUB_MESSAGE_HANDLE message, void* ctx) -> IOTHUBMESSAGE_DISPOSITION_RESULT { 
    (void)message;
    auto that = (AzureIotClient*)ctx;
    auto logger = that->getLogger();
    logger->logInfo("messageCallback", "Message received");
    return IOTHUBMESSAGE_ACCEPTED;
  };
  if ((_handle = ::IoTHubDeviceClient_LL_CreateFromDeviceAuth(hubUrl.c_str(), deviceId.c_str(), ::MQTT_Protocol)) == NULL) {
    logger->logError(_tag, "Failed to create IoT Hub client from connection string");
  } else {
    ::IoTHubDeviceClient_LL_SetConnectionStatusCallback(_handle, statusCallback, this);//&iothub_info);
    ::IoTHubDeviceClient_LL_SetMessageCallback(_handle, messageCallback, this);//&iothub_info);
    if (::xTaskCreate(&doWork, "doIotWork", 5 * 1024, (void*)_handle, 5, NULL) != pdPASS) {
      _logger->logError(_tag, "Create task failed");
    }
  }
}

void AzureIotClient::doWork(void* parameter) {
  auto handle = (IOTHUB_DEVICE_CLIENT_LL_HANDLE)parameter;
  while(true) {
    ::IoTHubDeviceClient_LL_DoWork(handle);
    ::vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

ILogger* AzureIotClient::getLogger() {
  return _logger;
}

// IOTHUB_MESSAGE_HANDLE msg_handle = ::IoTHubMessage_CreateFromByteArray((const unsigned char*)msgText, strlen(msgText));
// ::IoTHubDeviceClient_LL_SendEventAsync(handle, msg_handle, NULL, NULL) != IOTHUB_CLIENT_OK)
// ::IoTHubMessage_Destroy(msg_handle);

//::IoTHubDeviceClient_LL_Destroy(handle);
//::IoTHub_Deinit();