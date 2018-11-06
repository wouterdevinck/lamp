#include <cstdint>
#include <avr/io.h>
#include "LedBoardChain.h"

using namespace lamp;

LedBoardChain::LedBoardChain(uint8_t boards) {
  _info = new ChainInfo(boards);
  DDRB |= _BV(PB3);   // MOSI output
  DDRB |= _BV(PB5);   // SCK output
  DDRB |= _BV(PB2);   // SS output
  DDRD |= _BV(PD7);   // LAT output 
  PORTB &= ~_BV(PB5); // SCK low
  SPSR = _BV(SPI2X);
  SPCR = _BV(SPE) | _BV(MSTR);
}

void LedBoardChain::setAllLeds(LedValue color) {
  // TODO
}

void LedBoardChain::setAllLeds(LedValue values[]) {

}

void LedBoardChain::setBrightness(uint8_t brightness) {
  // TODO
}

void LedBoardChain::setBrightness(uint8_t values[]) {
  // TODO
}

ChainInfo* LedBoardChain::getChainInfo() {
  return _info;
}

void LedBoardChain::setAllLeds(LedValue(*value)(uint8_t)) {
  PORTD &= ~_BV(PD7); // LAT low
  uint8_t data[3];
  for(uint8_t l = 0; l < _info->ledgroups; l++) {
    for(uint8_t i = 0; i < 4; i++) {
      uint16_t c = (l * 4) + i;
      auto led = value(l);
      uint16_t val = i == 0 ? led.r : (i == 1? led.g : (i == 2 ? led.b : led.w));
      if (c & 1) {
        data[2] = val >> 4;
        data[1] = ((uint8_t)(val << 4)) | (data[1] & 0x0F);
        for (uint8_t b = 0; b < 3; b++) {
          SPDR = data[b]; 
          while (!(SPSR & _BV(SPIF)));
        }
      } else {
        data[1] = val >> 8;
        data[0] = val & 0xFF;
      }
    }
  }
  PORTD |= _BV(PD7);  // LAT high
  PORTD &= ~_BV(PD7); // LAT low
}

void LedBoardChain::setBrightness(uint8_t(*value)(uint8_t)) {

}

//auto glambda = [](auto a, auto&& b) { return a < b; };