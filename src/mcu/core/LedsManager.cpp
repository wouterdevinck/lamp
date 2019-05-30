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
    _bri += BRISTEP;
    if (_bri > BRIMAX) {
      _bri = BRIMAX;
    }
    _leds->setBrightness(_bri);
    _leds->setAllLeds(LampColors[(uint8_t)_color]);
  }
}

void LedsManager::decreaseBrightness() {
  if (_bri > 0) {
    if (_bri < BRISTEP) {
      _bri = 0;
    } else {
      _bri -= BRISTEP;
    }
    _leds->setBrightness(_bri);
    _leds->setAllLeds(LampColors[(uint8_t)_color]);
  }
} 

void LedsManager::test() const {
  const uint8_t step = 5;
  for (uint16_t i = 0; i <= LEDMAX; i += step) {
    _leds->setAllLeds({i,0,0,0});
  }
  for (uint16_t i = 0; i <= LEDMAX; i += step) {
    _leds->setAllLeds({0,i,0,0});
  }
  for (uint16_t i = 0; i <= LEDMAX; i += step) {
    _leds->setAllLeds({0,0,i,0});
  }
  for (uint16_t i = 0; i <= LEDMAX; i += step) {
    _leds->setAllLeds({0,0,0,i});
  }
  auto groups = _leds->getChainInfo()->ledgroups;
  auto leds = new LedValue[groups];
  for (uint8_t g = 0; g < groups; g++) {
    leds[g] = {0,0,0,0};
  }
  for (uint8_t g = 0; g < groups; g++) {
    for (uint16_t i = 0; i <= LEDMAX; i += step) {
      leds[g].r = i;
      _leds->setAllLeds(leds);
    }
    leds[g].r = 0;
    for (uint16_t i = 0; i <= LEDMAX; i += step) {
      leds[g].g = i;
      _leds->setAllLeds(leds);
    }
    leds[g].g = 0;
    for (uint16_t i = 0; i <= LEDMAX; i += step) {
      leds[g].b = i;
      _leds->setAllLeds(leds);
    }
    leds[g].b = 0;
    for (uint16_t i = 0; i <= LEDMAX; i += step) {
      leds[g].w = i;
      _leds->setAllLeds(leds);
    }
    leds[g].w = 0;
  }
}