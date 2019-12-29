#include "AzureIotClient.h"

#include "iothub.h"
#include "iothubtransportmqtt.h"
#include "iothub_client_version.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace lamp;

AzureIotClient::AzureIotClient(string hubUrl, string deviceId, IIotHandler* handler, ILogger* logger) {
  _ctx = new IotContext();
  _ctx->logger = logger;
  _ctx->tag = _tag;
  _ctx->handler = handler;
  ::IoTHub_Init();
  logger->logInfo(_tag, "Iothub API Version: " + string(::IoTHubClient_GetVersionString()));
  _ctx->handle = ::IoTHubDeviceClient_LL_CreateFromDeviceAuth(hubUrl.c_str(), deviceId.c_str(), ::MQTT_Protocol);
  if (_ctx->handle == NULL) {
    logger->logError(_tag, "Failed to create IoT Hub client from connection string");
  } else {
    ::IoTHubDeviceClient_LL_SetConnectionStatusCallback(_ctx->handle, &statusCallback, (void*)_ctx);
    ::IoTHubDeviceClient_LL_SetMessageCallback(_ctx->handle, &messageCallback, (void*)_ctx);
    ::IoTHubDeviceClient_LL_SetDeviceMethodCallback(_ctx->handle, &dmCallback, (void*)_ctx);
    if (::xTaskCreate(&doWork, "doIotWork", 5 * 1024, (void*)_ctx, 5, NULL) != pdPASS) {
      logger->logError(_tag, "Create task failed");
    }
  }
}

void AzureIotClient::doWork(void* ctx) {
  auto handle = ((IotContext*)ctx)->handle;
  while(true) {
    ::IoTHubDeviceClient_LL_DoWork(handle);
    ::vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void AzureIotClient::statusCallback(IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* ctx) {
  (void)reason;
  auto that = (IotContext*)ctx;
  if (result == IOTHUB_CLIENT_CONNECTION_AUTHENTICATED) {
    that->logger->logInfo(that->tag, "Connected to IoT Hub");
  } else {
    that->logger->logError(that->tag, "Disconnected from IoT Hub");
  }
}

IOTHUBMESSAGE_DISPOSITION_RESULT AzureIotClient::messageCallback(IOTHUB_MESSAGE_HANDLE message, void* ctx) {
  (void)message;
  auto that = (IotContext*)ctx;
  that->logger->logInfo(that->tag, "Message received");
  // TODO
  return IOTHUBMESSAGE_ACCEPTED;
}

int AzureIotClient::dmCallback(const char* method, const unsigned char* payload, size_t size, unsigned char** response, size_t* response_size, void* ctx) {
  auto that = (IotContext*)ctx;
  string m(method);
  string p((const char*)payload, size);
  that->logger->logInfo(that->tag, "DM received: " + m);
  string resp = that->handler->handleDirectMethod(m, p);
  // that->logger->logInfo(that->tag, "Resp: " + resp);
  *response_size = resp.length();
  *response = (unsigned char*)malloc(*response_size);
  memcpy(*response, resp.c_str(), *response_size );
  return 200;
}

// IOTHUB_MESSAGE_HANDLE msg_handle = ::IoTHubMessage_CreateFromByteArray((const unsigned char*)msgText, strlen(msgText));
// ::IoTHubDeviceClient_LL_SendEventAsync(handle, msg_handle, NULL, NULL) != IOTHUB_CLIENT_OK)
// ::IoTHubMessage_Destroy(msg_handle);

//::IoTHubDeviceClient_LL_Destroy(handle);
//::IoTHub_Deinit();