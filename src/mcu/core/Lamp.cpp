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
  auto iot = _platform->getIotFramework();
  auto port = _platform->getHttpServerPort();
  auto wifi = _platform->getWiFiClient();
  auto storage = _platform->getStorage();
  _upgrade = new UpgradeManager(updater, logger, httpclient);
  _httphandler = new HttpHandler(led, _upgrade, _manager);
  _iothandler = new IotHandler();
  _wifihandler = new WiFiHandler(wifi, storage, httpserver, port, _httphandler, iot, _iothandler);
  wifi->setHandler(_wifihandler);
  #endif
}

void Lamp::start() const {
  #ifndef BASIC
  auto logger = _platform->getLogger();
  logger->logInfo("Lamp", "Lamp is starting");
  _upgrade->start();
  auto wifi = _platform->getWiFiClient();
  auto storage = _platform->getStorage();
  auto ssid = storage->getValue(STORE_SSID);
  auto pwd = storage->getValue(STORE_PWD);
  if (ssid.empty() || pwd.empty()) {
    wifi->startSmartConfig();
  } else {
    wifi->connect(ssid, pwd);
  }
  #endif
  _manager->start();
  auto ir = _platform->getIrReceiver();
  auto led = _platform->getRgbLed();  
  ir->start(_irhandler);
  const RgbLedColor color = { 255, 255, 255 };
  led->setLedColor(color);
}