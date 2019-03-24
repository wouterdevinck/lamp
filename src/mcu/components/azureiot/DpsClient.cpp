#include "DpsClient.h"

#include <sstream>

#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"

#include "azure_prov_client/prov_device_ll_client.h"
#include "azure_prov_client/prov_security_factory.h"
#include "azure_prov_client/prov_transport_mqtt_client.h" // TODO Switch to MQTT_WS

using namespace lamp;

DpsClient::DpsClient(ILogger* logger) {
  _logger = logger;
}

string DpsClient::Register(string idscope) {
  DpsContext ctx;
  ctx.registered = false;
  ctx.dps = this;

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
        context->url = string(uri);
        context->deviceid = string(id);
        context->registered = true;
        ostringstream log;
        log << "Registration succesful: id: " << context->deviceid; 
        log << " - hub: " << context->url;
        context->dps->_logger->logInfo(context->dps->_tag, log.str());
    } else {
        ostringstream log;
        log << "Registration failed: " << (int)result;
        context->dps->_logger->logError(context->dps->_tag, log.str());
    }
  };

  PROV_DEVICE_LL_HANDLE handle;
  if ((handle = ::Prov_Device_LL_Create(_url.c_str(), idscope.c_str(), ::Prov_Device_MQTT_Protocol)) == NULL) {
    _logger->logError(_tag, "Failed calling Prov_Device_LL_Create");
  } else {
    if (::Prov_Device_LL_Register_Device(handle, registerCallback, &ctx, statusCallback, &ctx) != PROV_DEVICE_RESULT_OK) {
      _logger->logError(_tag, "Failed calling Prov_Device_LL_Register_Device");
    } else {
      do {
        ::Prov_Device_LL_DoWork(handle);
        ::ThreadAPI_Sleep(20);
      } while (ctx.registered == false); // TODO - endless loop if fails...
    }
    ::Prov_Device_LL_Destroy(handle);
  }
  // if (user_ctx.registration_complete != 1) {
  //  _logger->logError(_tag, "Registration failed!");
  // }
  ::prov_dev_security_deinit();
  return ctx.url; // TODO Error handling?
}

// TODO ?
//  * Trusted certs: Prov_Device_SetOption(handle, OPTION_TRUSTED_CERT, certificates);
//  * Trace on: Prov_Device_SetOption(handle, PROV_OPTION_LOG_TRACE, true);