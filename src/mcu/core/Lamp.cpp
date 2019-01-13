#include "Lamp.h"

using namespace lamp;

Lamp::Lamp(IPlatform* platform) {
  _platform = platform;
  auto leds = _platform->getLedBoardChain();
  auto led = _platform->getRgbLed();
  _manager = new LedsManager(leds);
  _irhandler = new IrHandler(led, _manager);
  #ifndef BASIC
  auto updater = _platform->getUpdater();
  auto logger = _platform->getLogger();
  auto httpclient = _platform->getHttpClient();
  auto httpserver = _platform->getHttpServer();
  auto port = _platform->getHttpServerPort();
  auto wifi = _platform->getWiFiClient();
  auto storage = _platform->getStorage();
  _upgrade = new UpgradeManager(updater, logger, httpclient);
  _httphandler = new HttpHandler(led, _upgrade, _manager);
  _wifihandler = new WiFiHandler(wifi, storage, httpserver, port, _httphandler);
  #endif
}


void Lamp::start() const {
  #ifndef BASIC
  auto logger = _platform->getLogger();
  logger->logInfo("Lamp", "Lamp is starting");
  _upgrade->start();
  
  // TODO Load wifi settings from storage, 
  //   if no wifi settings in storage, run smartconfig

  auto wifi = _platform->getWiFiClient();
  wifi->connect("", "");

  #endif
  _manager->start();
  auto ir = _platform->getIrReceiver();
  auto led = _platform->getRgbLed();  
  ir->start(_irhandler);
  const RgbLedColor color = { 0, 0, 255 };
  led->setLedColor(color);
}