#include "LedsManager.h"

using namespace lamp;

const LedValue LedsManager::LampColors[4] = {
  { LEDMAX, 0, 0, 0 },
  { 0, LEDMAX, 0, 0 },
  { 0, 0, LEDMAX, 0 },
  { 0, 0, 0, LEDMAX }
};

void LedsManager::start() const {	
  _leds->setBrightness(_bri);
  _leds->setAllLeds(LampColors[(uint8_t)_color]);
}

void LedsManager::changeColor(LampColor color) {
  _color = color;
  _leds->setAllLeds(LampColors[(uint8_t)color]);
}

void LedsManager::increaseBrightness() {
  if (_bri < BRIMAX) {
    _bri++; // TODO steps
    _leds->setBrightness(_bri);
    _leds->setAllLeds(LampColors[(uint8_t)_color]);
  }
}

void LedsManager::decreaseBrightness() {
  if (_bri > 0) {
    _bri--;
    _leds->setBrightness(_bri);
    _leds->setAllLeds(LampColors[(uint8_t)_color]);
  }
} 