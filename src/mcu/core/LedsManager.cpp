#include "LedsManager.h"
#include <cassert>
#include <algorithm>

using namespace lamp;
using namespace std;

const LedValue LedsManager::LampColors[4] = {
  { LEDMAX, 0, 0, 0 },
  { 0, LEDMAX, 0, 0 },
  { 0, 0, LEDMAX, 0 },
  { 0, 0, 0, LEDMAX }
};

void LedsManager::start() const {	
  _leds->setBrightness(_bri);
  #ifdef BASIC
  // Animated fade in to _color
  auto fade = [](LedValue val, uint16_t step, uint16_t steps) -> LedValue { 
    assert(steps > 1);
    return {
      (unsigned int)(step * (val.r / steps)),
      (unsigned int)(step * (val.g / steps)),
      (unsigned int)(step * (val.b / steps)),
      (unsigned int)(step * (val.w / steps))
    }; 
  };
  auto groups = _leds->getChainInfo()->ledgroups;
  LedValue target = LampColors[(uint8_t)_color];
  uint8_t half = (groups / 2) - 1;
  auto leds = new LedValue[groups];
  for (uint16_t i = 0; i <= (uint16_t)(ANISTEPS + half * ANISPEED); i++) {
    for (uint8_t g = 0; g <= half; g++) {
      uint16_t d = (half - g) * ANISPEED;
      uint16_t s = min((uint16_t)(i > d ? i - d : 0), (uint16_t)ANISTEPS);
      auto val = fade(target, s, ANISTEPS);
      leds[g] = val;
      leds[groups - g - 1] = val;
    }
    _leds->setAllLeds(leds);
  }
  #endif
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
  for (uint16_t i = 0; i <= LEDMAX; i += TESTSTEP) {
    _leds->setAllLeds({i,0,0,0});
  }
  for (uint16_t i = 0; i <= LEDMAX; i += TESTSTEP) {
    _leds->setAllLeds({0,i,0,0});
  }
  for (uint16_t i = 0; i <= LEDMAX; i += TESTSTEP) {
    _leds->setAllLeds({0,0,i,0});
  }
  for (uint16_t i = 0; i <= LEDMAX; i += TESTSTEP) {
    _leds->setAllLeds({0,0,0,i});
  }
  auto groups = _leds->getChainInfo()->ledgroups;
  auto leds = new LedValue[groups];
  for (uint8_t g = 0; g < groups; g++) {
    leds[g] = {0,0,0,0};
  }
  for (uint8_t g = 0; g < groups; g++) {
    for (uint16_t i = 0; i <= LEDMAX; i += TESTSTEP) {
      leds[g].r = i;
      _leds->setAllLeds(leds);
    }
    leds[g].r = 0;
    for (uint16_t i = 0; i <= LEDMAX; i += TESTSTEP) {
      leds[g].g = i;
      _leds->setAllLeds(leds);
    }
    leds[g].g = 0;
    for (uint16_t i = 0; i <= LEDMAX; i += TESTSTEP) {
      leds[g].b = i;
      _leds->setAllLeds(leds);
    }
    leds[g].b = 0;
    for (uint16_t i = 0; i <= LEDMAX; i += TESTSTEP) {
      leds[g].w = i;
      _leds->setAllLeds(leds);
    }
    leds[g].w = 0;
  }
}