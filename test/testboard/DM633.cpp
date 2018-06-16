/*
 * Arduino library for DM633 LED Driver
 *   Wouter Devinck
 *
 * Inspired by:
 *   DMDriver by Dmitry Reznkov
 *   TLC5940 library by Alex Leone
*/
#include "DM633.h"

LedDriver::LedDriver(uint8_t count, uint8_t latch) {
  _count = count;
  _latch = latch;
  _channels = _count * 16; 
  _bytes = _count * 24;
}

void LedDriver::init() {
  _buffer = new uint8_t[_bytes];
  _latBit = digitalPinToBitMask(_latch);
  uint8_t latPort = digitalPinToPort(_latch);
  volatile uint8_t *latReg = portModeRegister(latPort);
  _latOut = portOutputRegister(latPort);
  DDRB |= _BV(PB3);   // MOSI output
  DDRB |= _BV(PB5);   // SCK output
  DDRB |= _BV(PB2);   // SS output
  *latReg |= _latBit; // LAT output 
  PORTB &= ~_BV(PB5); // SCK low
  SPSR = _BV(SPI2X);
  SPCR = _BV(SPE) | _BV(MSTR); 
  clearAll();
  sendAll();
}

void LedDriver::setChannelValue(uint16_t channel, uint16_t val) {
  uint16_t place = (channel * 3) >> 1;
  if (channel & 1) {
    _buffer[place+1] = val >> 4;
    _buffer[place] = ((uint8_t)(val << 4)) | (_buffer[place] & 0x0F);
  } else { 
    _buffer[place+1] = (_buffer[place+1] & 0xF0) | (val >> 8);
    _buffer[place] = val & 0xFF;
  } 
}

void LedDriver::sendAll() {
  LAT_LOW();
  uint16_t k = _bytes;
  do {   
    k--;
    send(_buffer[k]);
  } while (k);
  LAT_PULSE();
}

void LedDriver::clearAll() {
  for (uint16_t k = 0; k < _bytes; k++) {
    _buffer[k] = 0;
  }
}

void LedDriver::setGlobalBrightness(uint8_t bri) {
  SPCR |= _BV(CPOL); // SCK high
  LAT_PULSE(); LAT_PULSE(); LAT_PULSE(); LAT_PULSE();
  SPCR &= ~_BV(CPOL); // SCK low
  for (uint8_t k = 0; k < _count; k++) { 
    send(bri << 1);
  }
  LAT_PULSE();
  sendAll();
}

void LedDriver::send(uint8_t value) {
  SPDR = value; 
  while (!(SPSR & _BV(SPIF)));
}
