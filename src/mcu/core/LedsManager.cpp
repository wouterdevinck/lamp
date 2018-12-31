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
    _bri++;
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

void LedsManager::test() const {
  const uint32_t delay = 0;
  for (uint16_t i = 0; i < 4096; i++) {
    _leds->setAllLeds({i,0,0,0});
    _timing->delay(delay);
  }
  for (uint16_t i = 0; i < 4096; i++) {
    _leds->setAllLeds({0,i,0,0});
    _timing->delay(delay);
  }
  for (uint16_t i = 0; i < 4096; i++) {
    _leds->setAllLeds({0,0,i,0});
    _timing->delay(delay);
  }
  for (uint16_t i = 0; i < 4096; i++) {
    _leds->setAllLeds({0,0,0,i});
    _timing->delay(delay);
  }
  auto groups = _leds->getChainInfo()->ledgroups;
  auto leds = new LedValue[groups];
  for (uint8_t g = 0; g < groups; g++) {
    leds[g] = {0,0,0,0};
  }
  for (uint8_t g = 0; g < groups; g++) {
    for (uint16_t i = 0; i < 4096; i++) {
      leds[g].r = i;
      _leds->setAllLeds(leds);
      _timing->delay(delay);
    }
    leds[g].r = 0;
    for (uint16_t i = 0; i < 4096; i++) {
      leds[g].g = i;
      _leds->setAllLeds(leds);
      _timing->delay(delay);
    }
    leds[g].g = 0;
    for (uint16_t i = 0; i < 4096; i++) {
      leds[g].b = i;
      _leds->setAllLeds(leds);
      _timing->delay(delay);
    }
    leds[g].b = 0;
    for (uint16_t i = 0; i < 4096; i++) {
      leds[g].w = i;
      _leds->setAllLeds(leds);
      _timing->delay(delay);
    }
    leds[g].w = 0;
  }
}