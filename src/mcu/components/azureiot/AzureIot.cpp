#include "AzureIot.h"
#include "AzureIotClient.h"

using namespace lamp;

AzureIot::AzureIot(ILogger* logger) {
  _logger = logger;
  _dps = new DpsClient(logger);
}

IIotClient* AzureIot::connect(IIotHandler* handler) {
  auto reg = _dps->Register(_dpsUrl, _dpsIdScope);
  if (reg->registered) {
    _logger->logInfo(_tag, "IoTHub URL: " + reg->result->url);
    auto client = new AzureIotClient(reg->result->url, reg->result->deviceid, handler, _logger);
    client->connect();
    return client;
  }
  return NULL;

}