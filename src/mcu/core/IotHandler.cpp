#ifndef BASIC

#include "IotHandler.h"

using namespace lamp;
	
string IotHandler::handleDirectMethod(string method, string payload) {
  if (method.compare("statusLed") == 0) {
    if (payload.compare("\"red\"") == 0) {
      _led->setLedColor({ 50, 0, 0 });
      return "{ \"color\": \"red\" }";
    } else if (payload.compare("\"green\"") == 0) {
      _led->setLedColor({ 0, 50, 0 });
      return "{ \"color\": \"red\" }";
    } else if (payload.compare("\"blue\"") == 0) {
      _led->setLedColor({ 0, 0, 50 });
      return "{ \"color\": \"blue\" }";
    }
  }
  return "{}";
}

#endif