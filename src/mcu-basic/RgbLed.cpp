#include "RgbLed.h"
#include <avr/io.h>

using namespace lamp;

// PD3 - OC2B - red
// PD5 - OC0B - green
// PD6 - OC0A - blue

RgbLed::RgbLed() {
  DDRD |= _BV(PD3) | _BV(PD5) | _BV(PD6);
  TCCR0A = _BV(WGM00) | _BV(WGM01);
  TCCR0B = _BV(CS00);
  TCCR2A = _BV(WGM20) | _BV(WGM21);
  TCCR2B = _BV(CS20);
}

void RgbLed::setLedColor(RgbLedColor color) {
  if(color.r == 0) {
    TCCR2A &= ~_BV(COM2B1);
  } else {
    TCCR2A |= _BV(COM2B1);
    OCR2B = color.r;
  }
  if(color.g == 0) {
    TCCR0A &= ~_BV(COM0B1);
  } else {
    TCCR0A |= _BV(COM0B1);
    OCR0B = color.g;
  }
  if(color.b == 0) {
    TCCR0A &= ~_BV(COM0A1);
  } else {
    TCCR0A |= _BV(COM0A1);
    OCR0A = color.b;
  }
}