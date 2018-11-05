#include <cstdint>
#include "LedBoardChain.h"

using namespace lamp;

LedBoardChain::LedBoardChain(uint8_t drivers) {
  _drivers = drivers;
  _channels = _drivers * 16; 
  _bytes = _drivers * 24;
}

void LedBoardChain::setAllLeds(LedValue color) {
  // TODO
}

void LedBoardChain::setAllLeds(LedValue values[]) {
  // TODO
}

void LedBoardChain::setBrightness(uint8_t brightness) {
  // TODO
}

void LedBoardChain::setBrightness(uint8_t values[]) {
  // TODO
}

ChainInfo LedBoardChain::getChainInfo() {
  return ChainInfo {
    _channels,
    _drivers
  };
}