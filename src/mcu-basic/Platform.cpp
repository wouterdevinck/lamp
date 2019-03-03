#include "Platform.h"

using namespace lamp;

Platform::Platform(uint8_t boards) {
  _ir = new IrReceiver();
  _led = new RgbLed();
  _leds = new LedBoardChain(boards);
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

void Platform::loop() {
  _ir->loop();
}