#include "DpsClient.h"

#include <sstream>

#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_prov_client/prov_device_ll_client.h"
#include "azure_prov_client/prov_security_factory.h"
#include "azure_prov_client/prov_transport_mqtt_client.h"

using namespace lamp;

DpsClient::DpsClient(ILogger* logger) {
  _logger = logger;
}

DpsContext* DpsClient::Register(string url, string idscope) {
  auto ctx = new DpsContext();
  ctx->result = new DpsResult();
  ctx->dps = this;
  ctx->registered = false;
  ctx->completed = false;
  ::prov_dev_security_init(SECURE_DEVICE_TYPE_X509);
  _logger->logInfo(_tag, "Provisioning API Version: " + string(::Prov_Device_LL_GetVersionString()));
  auto statusCallback = [] (PROV_DEVICE_REG_STATUS status, void* ctx) -> void { 
    auto context = (DpsContext*)ctx;
    ostringstream log;
    log << "Provisioning Status: " << (int)status;
    context->dps->_logger->logInfo(context->dps->_tag, log.str());
  };
  auto registerCallback = [] (PROV_DEVICE_RESULT result, const char* uri, const char* id, void* ctx) -> void {
    auto context = (DpsContext*)ctx;
    if (result == PROV_DEVICE_RESULT_OK) {
        context->result->url = string(uri);
        context->result->deviceid = string(id);
        context->registered = true;
        context->completed = true;
        ostringstream log;
        log << "Registration succesful: id: " << context->result->deviceid; 
        log << " - hub: " << context->result->url;
        context->dps->_logger->logInfo(context->dps->_tag, log.str());
    } else {
        context->completed = true;
        ostringstream log;
        log << "Registration failed: " << (int)result;
        context->dps->_logger->logError(context->dps->_tag, log.str());
    }
  };
  PROV_DEVICE_LL_HANDLE handle;
  if ((handle = ::Prov_Device_LL_Create(url.c_str(), idscope.c_str(), ::Prov_Device_MQTT_Protocol)) == NULL) {
    _logger->logError(_tag, "Failed calling Prov_Device_LL_Create");
  } else {
    if (::Prov_Device_LL_Register_Device(handle, registerCallback, ctx, statusCallback, ctx) != PROV_DEVICE_RESULT_OK) {
      _logger->logError(_tag, "Failed calling Prov_Device_LL_Register_Device");
    } else {
      do {
        ::Prov_Device_LL_DoWork(handle);
        ::ThreadAPI_Sleep(20);
      } while (ctx->completed == false);
    }
    ::Prov_Device_LL_Destroy(handle);
  }
  if (ctx->registered == false) {
    _logger->logError(_tag, "Registration failed!");
  }
  ::prov_dev_security_deinit();
  return ctx;
}