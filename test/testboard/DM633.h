/*
 * Arduino library for DM633 LED Driver
 *   Wouter Devinck
 *
 * Inspired by:
 *   DMDriver by Dmitry Reznkov
 *   TLC5940 library by Alex Leone
*/
#ifndef DM633
#define DM633

#include "Arduino.h"

#define LAT_LOW() *_latOut &= ~_latBit
#define LAT_PULSE() *_latOut |= _latBit; *_latOut &= ~_latBit

class LedDriver {
	
public:
  LedDriver(uint8_t count, uint8_t latch);
  void init(); 
  void setChannelValue(uint16_t channel, uint16_t val); 
  void setGlobalBrightness(uint8_t bri);
  void sendAll();
  void clearAll(); 
   
private:
  uint8_t _count, _latch;
  uint8_t _channels, _bytes;   
  uint8_t _latBit;
  volatile uint8_t *_latOut;
  volatile uint8_t *_buffer;
  
  void send(uint8_t value);
   
};

#endif
