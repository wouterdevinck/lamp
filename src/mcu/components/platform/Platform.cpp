#include "Platform.h"
#include "MemMon.h"
#include "constants.h"

using namespace lamp;

Platform::Platform() {
  MemMon memmon(MEMMON_PERIOD);
  _spi1 = new Spi(SPI1_ESP_HOST, SPI1_DMA_CH, SPI1_MOSI_PIN, SPI1_MISO_PIN, SPI1_CLK_PIN, SPI1_CS_PIN);
  _spi2 = new SpiFlash(SPI2_ESP_HOST, SPI2_DMA_CH, SPI2_MOSI_PIN, SPI2_MISO_PIN, SPI2_CLK_PIN, SPI2_CS_PIN);
  _ir = new IrReceiver();
  _led = new RgbLed(PIN_RGB_RED, PIN_RGB_GREEN, PIN_RGB_BLUE);
  _leds = new LedBoardChain(_spi1, PIN_LED_INT);
  _storage = new Storage();
  _wifi = new WiFiClient(_storage);
  _lux = new LightSensor();
  _updater = new Updater(_spi2, _storage, CHUNK_SIZE);
  _logger = new Logger();
  _httpserver = new HttpServer();
  _httpclient = new HttpClient(CHUNK_SIZE);
  _iot = new AzureIot(_logger);
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

ILightSensor* Platform::getLightSensor() {
  return _lux;
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

IIotFramework* Platform::getIotFramework() {
  return _iot;
}

INvs* Platform::getStorage() {
  return _storage;
}

int Platform::getHttpServerPort() {
  return HTTP_PORT;
}