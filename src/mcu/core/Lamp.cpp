#include "Lamp.h"

using namespace lamp;

// TODO: logging

Lamp::Lamp(
  IUpdater* updater,
  IHttpServer* httpserver,
  IHttpClient* httpclient,
  IRgbLed* led,
  ILedBoardChain* leds
) {
  _updater = updater;
  _httpserver = httpserver;
  _httpclient = httpclient;
  _led = led;
  _leds = leds;
  _upgrade = new UpgradeManager(updater, httpclient);
  _handler = new HttpHandler(led, _upgrade);
}

void Lamp::start(const int port) const {
  const RgbLedColor color = { 0, 0, 255 };
  _led->setLedColor(color);
  _httpserver->start(port, _handler);
  _upgrade->boot();
}