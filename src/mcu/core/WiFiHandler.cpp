#ifndef BASIC

#include "WiFiHandler.h"

using namespace lamp;
	
void WiFiHandler::onConnected() {
  _httpserver->start(_port, _httphandler);
  auto client = _iot->connect(_iothandler);
  // TODO
}

void WiFiHandler::onDisconnected() {
  _wifi->reconnect();
}

void WiFiHandler::onSmartConfig(string ssid, string password) {
  _storage->setValue(STORE_SSID, ssid);
  _storage->setValue(STORE_PWD, password);
  _wifi->connect(ssid, password);
  _wifi->reconnect();
}

#endif