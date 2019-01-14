#include "Platform.h"
#include "MemMon.h"
#include "constants.h"

using namespace lamp;

Platform::Platform() {
  MemMon memmon(MEMMON_PERIOD);
  _spi1 = new SPI(SPI1_HOST, SPI1_DMA_CH, SPI1_MOSI_PIN, SPI1_MISO_PIN, SPI1_CLK_PIN, SPI1_CS_PIN);
  _ir = new IrReceiver();
  _led = new RgbLed(PIN_RGB_RED, PIN_RGB_GREEN, PIN_RGB_BLUE);
  _leds = new LedBoardChain(_spi1, PIN_LED_INT);
  _storage = new Storage();
  _wifi = new WiFiClient(_storage);
  _updater = new Updater(CHUNK_SIZE);
  _logger = new Logger();
  _httpserver = new HttpServer();
  _httpclient = new HttpClient(CHUNK_SIZE);
}

IIrReceiver* Platform::getIrReceiver() {
  return _ir;
}

IRgbLed* Platform::getRgbLed() {
  return _led;
}

ILedBoardChain* Platform::getLedBoardChain() {
  return _leds;
}

IWiFiClient* Platform::getWiFiClient() {
  return _wifi;
}

IUpdater* Platform::getUpdater() {
  return _updater;
}

ILogger* Platform::getLogger() {
  return _logger;
}

IHttpServer* Platform::getHttpServer() {
  return _httpserver;
}

IHttpClient* Platform::getHttpClient() {
  return _httpclient;
}

IStorage* Platform::getStorage() {
  return _storage;
}

int Platform::getHttpServerPort() {
  return HTTP_PORT;
}