#include "AzureIot.h"
#include "AzureIotClient.h"

using namespace lamp;

AzureIot::AzureIot(ILogger* logger) {
  _logger = logger;
  _dps = new DpsClient(logger);
}

IIotClient* AzureIot::connect(IIotHandler* handler) {
  auto url = _dps->Register(_idscope);
  _logger->logInfo(_tag, "IoTHub URL: " + url);
  return new AzureIotClient();
}