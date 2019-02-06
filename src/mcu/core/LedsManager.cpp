#include "LedsManager.h"

using namespace lamp;

const LedValue LedsManager::LampColors[4] = {
  { LEDMAX, 0, 0, 0 },
  { 0, LEDMAX, 0, 0 },
  { 0, 0, LEDMAX, 0 },
  { 0, 0, 0, LEDMAX }
};

void LedsManager::start() const {
  _leds->setAllLeds(LampColors[White]);
}

void LedsManager::changeColor(LampColor color) const {
  _leds->setAllLeds(LampColors[color]);
}

void LedsManager::test() const {
  for (uint16_t i = 0; i < 4096; i++) {
    _leds->setAllLeds({i, 0, 0, 0});
  }
}