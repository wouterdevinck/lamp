#ifndef BASIC

#include "WiFiHandler.h"

using namespace lamp;
	
void WiFiHandler::onConnected() {
  _led->setLedColor({ 0, 50, 0 }); // Green = connected
  _httpserver->start(_port, _httphandler);
  auto client = _iot->connect(_iothandler);
  // TODO
}

void WiFiHandler::onDisconnected() {
  _led->setLedColor({ 50, 0, 0 }); // Red = not connected
  _wifi->reconnect();
}

void WiFiHandler::onSmartConfig(string ssid, string password) {
  _storage->setValue(STORE_SSID, ssid);
  _storage->setValue(STORE_PWD, password);
  _led->setLedColor({ 50, 50, 0 }); // Yellow = connecting
  _wifi->connect(ssid, password);
  _wifi->reconnect();
}

#endif