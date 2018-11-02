#include "Lamp.h"

using namespace lamp;

#ifndef BASIC
Lamp::Lamp(
  IUpdater* updater,
  ILogger* logger,
  IHttpServer* httpserver,
  IHttpClient* httpclient,
  IRgbLed* led,
  ILedBoardChain* leds
) {
  _updater = updater;
  _logger = logger;
  _httpserver = httpserver;
  _httpclient = httpclient;
  _led = led;
  _leds = leds;
  _upgrade = new UpgradeManager(updater, logger, httpclient);
  _handler = new HttpHandler(led, _upgrade);
}
#else
Lamp::Lamp(
  IRgbLed* led
) {
  _led = led;
}
#endif

void Lamp::start(const int port) const {
  #ifndef BASIC
  _logger->logInfo("Lamp", "Lamp is starting");
  _httpserver->start(port, _handler);
  _upgrade->boot();
  #endif
  const RgbLedColor color = { 0, 0, 255 };
  _led->setLedColor(color);
}