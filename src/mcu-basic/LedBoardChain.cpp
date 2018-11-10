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
  LedValue values[1] = { color };
  setAllLeds(values, [](uint8_t _) { return (uint8_t)0; });
}

void LedBoardChain::setAllLeds(LedValue values[]) {
  setAllLeds(values, [](uint8_t i) { return i; });
}

void LedBoardChain::setBrightness(uint8_t brightness) {
  uint8_t values[1] = { brightness };
  setBrightness(values, [](uint8_t _) { return (uint8_t)0; });
}

void LedBoardChain::setBrightness(uint8_t values[]) {
  setBrightness(values, [](uint8_t i) { return i; });
}

ChainInfo* LedBoardChain::getChainInfo() {
  return _info;
}

void LedBoardChain::setAllLeds(LedValue values[], uint8_t(*index)(uint8_t)) {
  PORTD &= ~_BV(PD7); // LAT low
  uint8_t data[3];
  for(uint8_t l = 0; l < _info->ledgroups; l++) {
    for(uint8_t i = 0; i < 4; i++) {
      uint16_t c = (l * 4) + i;
      auto led = values[index(l)];
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

void LedBoardChain::setBrightness(uint8_t values[], uint8_t(*index)(uint8_t)) {
  SPCR |= _BV(CPOL);  // SCK high
  for (uint8_t i = 0; i < 4; i++) {
    PORTD |= _BV(PD7);  // LAT high
    PORTD &= ~_BV(PD7); // LAT low
  }
  SPCR &= ~_BV(CPOL); // SCK low
  for (uint8_t d = 0; d < _info->drivers; d++) { 
    auto bri = values[index(d)];
    SPDR = bri << 1; 
    while (!(SPSR & _BV(SPIF)));
  }
  PORTD |= _BV(PD7);  // LAT high
  PORTD &= ~_BV(PD7); // LAT low
}
