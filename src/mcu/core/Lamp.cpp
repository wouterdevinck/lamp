#include "Lamp.h"

using namespace lamp;

#ifndef BASIC
Lamp::Lamp(
  IUpdater* updater,
  ILogger* logger,
  IIrReceiver* ir,
  IHttpServer* httpserver,
  IHttpClient* httpclient,
  IRgbLed* led,
  ILedBoardChain* leds
) {
  _updater = updater;
  _logger = logger;
  _ir = ir;
  _httpserver = httpserver;
  _httpclient = httpclient;
  _led = led;
  _leds = leds;
  _upgrade = new UpgradeManager(updater, logger, httpclient);
  _httphandler = new HttpHandler(led, _upgrade);
  _irhandler = new IrHandler(led);
}
#else
Lamp::Lamp(
  IIrReceiver* ir,
  IRgbLed* led
) {
  _ir = ir;
  _led = led;
  _irhandler = new IrHandler(led);
}
#endif

void Lamp::start(const int port) const {
  #ifndef BASIC
  _logger->logInfo("Lamp", "Lamp is starting");
  _httpserver->start(port, _httphandler);
  _upgrade->boot();
  #endif
  _ir->start(_irhandler);
  const RgbLedColor color = { 0, 0, 255 };
  _led->setLedColor(color);
}