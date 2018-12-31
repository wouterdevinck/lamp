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
  ILedBoardChain* leds,
  ITiming* timing
) {
  _updater = updater;
  _logger = logger;
  _ir = ir;
  _httpserver = httpserver;
  _httpclient = httpclient;
  _led = led;
  _leds = leds;
  _timing = timing;
  _upgrade = new UpgradeManager(updater, logger, httpclient);
  _manager = new LedsManager(leds, timing);
  _httphandler = new HttpHandler(led, _upgrade, _manager);
  _irhandler = new IrHandler(led, _manager);
}
#else
Lamp::Lamp(
  IIrReceiver* ir,
  IRgbLed* led,
  ILedBoardChain* leds,
  ITiming* timing
) {
  _ir = ir;
  _led = led;
  _leds = leds;
  _timing = timing;
  _manager = new LedsManager(leds, timing);
  _irhandler = new IrHandler(led, _manager);
}
#endif

void Lamp::start(const int port) const {
  #ifndef BASIC
  _logger->logInfo("Lamp", "Lamp is starting");
  _httpserver->start(port, _httphandler);
  _upgrade->start();
  #endif
  _manager->start();
  _ir->start(_irhandler);
  const RgbLedColor color = { 0, 0, 255 };
  _led->setLedColor(color);
}