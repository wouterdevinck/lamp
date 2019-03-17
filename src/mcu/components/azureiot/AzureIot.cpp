#include "AzureIot.h"
#include "AzureIotClient.h"

#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"

#include "azure_prov_client/prov_device_ll_client.h"
#include "azure_prov_client/prov_security_factory.h"
#include "azure_prov_client/prov_transport_mqtt_client.h" // TODO switch to MQTT_WS

#include "esp_log.h"

static char tag[] = "AzureIot";

// DEFINE_ENUM_STRINGS(PROV_DEVICE_RESULT, PROV_DEVICE_RESULT_VALUE);
// DEFINE_ENUM_STRINGS(PROV_DEVICE_REG_STATUS, PROV_DEVICE_REG_STATUS_VALUES);

static char dpsUrl[] = "global.azure-devices-provisioning.net";
static char dpsIdScope[] = "0ne00040DC7"; 

using namespace lamp;

typedef struct CLIENT_SAMPLE_INFO_TAG {
  unsigned int sleep_time;
  char* iothub_uri;
  char* access_key_name;
  char* device_key;
  char* device_id;
  int registration_complete;
} CLIENT_SAMPLE_INFO;

static void registation_status_callback(::PROV_DEVICE_REG_STATUS reg_status, void* user_context) {
  ESP_LOGI(tag, "Provisioning Status: %d", reg_status);
  //::ENUM_TO_STRING(PROV_DEVICE_REG_STATUS, reg_status)
}

static void register_device_callback(::PROV_DEVICE_RESULT register_result, const char* iothub_uri, const char* device_id, void* user_context) {
  if (user_context != NULL) {
    CLIENT_SAMPLE_INFO* user_ctx = (CLIENT_SAMPLE_INFO*)user_context;
    if (register_result == PROV_DEVICE_RESULT_OK)     {
        ESP_LOGI(tag, "Registration Information received from service: %s!", iothub_uri);
        ::mallocAndStrcpy_s(&user_ctx->iothub_uri, iothub_uri);
        ::mallocAndStrcpy_s(&user_ctx->device_id, device_id);
        user_ctx->registration_complete = 1;
    } else {
        ESP_LOGE(tag, "Failure encountered on registration: %d", register_result);
        // ::ENUM_TO_STRING(::PROV_DEVICE_RESULT, register_result)
        user_ctx->registration_complete = 2;
    }
  }
}

AzureIot::AzureIot() {}

IIotClient* AzureIot::connect(IIotHandler* handler) {

  ::prov_dev_security_init(::SECURE_DEVICE_TYPE_X509);

  // HTTP_PROXY_OPTIONS http_proxy;
  CLIENT_SAMPLE_INFO user_ctx;
  // memset(&http_proxy, 0, sizeof(HTTP_PROXY_OPTIONS));
  memset(&user_ctx, 0, sizeof(CLIENT_SAMPLE_INFO));
  user_ctx.registration_complete = 0;
  user_ctx.sleep_time = 10;

  ESP_LOGI(tag, "Provisioning API Version: %s", ::Prov_Device_LL_GetVersionString());
  // printf("Iothub API Version: %s\r\n", IoTHubClient_GetVersionString());

  ::PROV_DEVICE_LL_HANDLE handle;
  if ((handle = ::Prov_Device_LL_Create(dpsUrl, dpsIdScope, ::Prov_Device_MQTT_Protocol)) == NULL) {
    ESP_LOGE(tag, "Failed calling Prov_Device_LL_Create");
  } else {
    // Prov_Device_LL_SetOption(handle, PROV_OPTION_LOG_TRACE, &traceOn);
    // Prov_Device_SetOption(prov_device_handle, PROV_REGISTRATION_ID, "[REGISTRATION ID]");
    if (::Prov_Device_LL_Register_Device(handle, register_device_callback, &user_ctx, registation_status_callback, &user_ctx) != PROV_DEVICE_RESULT_OK) {
      ESP_LOGE(tag, "Failed calling Prov_Device_LL_Register_Device");
    } else {
      do {
        ::Prov_Device_LL_DoWork(handle);
        ::ThreadAPI_Sleep(user_ctx.sleep_time);
      } while (user_ctx.registration_complete == 0);
    }
    ::Prov_Device_LL_Destroy(handle);
  }
  if (user_ctx.registration_complete != 1) {
    ESP_LOGE(tag, "Registration failed!");
  }

  free(user_ctx.iothub_uri);
  free(user_ctx.device_id);
  ::prov_dev_security_deinit();

  return new AzureIotClient();
}