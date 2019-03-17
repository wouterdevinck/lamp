#include "AzureIot.h"
#include "AzureIotClient.h"

using namespace lamp;

AzureIot::AzureIot() {}

IIotClient* AzureIot::connect(IIotHandler* handler) {
  return new AzureIotClient();
}