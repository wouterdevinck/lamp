#include "RgbLed.h"
#include <avr/io.h>

using namespace lamp;

// PD3 - OC2B - red
// PD5 - OC0B - green
// PD6 - OC0A - blue

RgbLed::RgbLed() {
  DDRD = (1 << PD3) | (1 << PD5) | (1 << PD6);
  TCCR0A = (1 << WGM00) | (1 << WGM01);
  TCCR0B = (1 << CS00);
  TCCR2A = (1 << WGM20) | (1 << WGM21);
  TCCR2B = (1 << CS20);
}

void RgbLed::setLedColor(RgbLedColor color) {
  if(color.r == 0) {
    TCCR2A &= ~(1 << COM2B1);
  } else {
    TCCR2A |= (1 << COM2B1);
    OCR2B = color.r;
  }
  if(color.g == 0) {
    TCCR0A &= ~(1 << COM0B1);
  } else {
    TCCR0A |= (1 << COM0B1);
    OCR0B = color.g;
  }
  if(color.b == 0) {
    TCCR0A &= ~(1 << COM0A1);
  } else {
    TCCR0A |= (1 << COM0A1);
    OCR0A = color.b;
  }
}