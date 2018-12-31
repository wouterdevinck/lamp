#include "IrReceiver.h"
#include <avr/io.h>

using namespace lamp;

IrReceiver::IrReceiver() {}

void IrReceiver::start(IIrHandler* handler) {
  _handler = handler;
  TCCR1B |= _BV(CS11);
  PORTD |= _BV(PD4);
}

void IrReceiver::loop() {
  uint32_t time;
  while(PIND & _BV(PD4));
  TCNT1 = 0;
  while(!(PIND & _BV(PD4)));
  time = TCNT1;
  TCNT1 = 0;
  if(time > CONV(9000) && time < CONV(9800)) {
    uint16_t code = 0;
    uint16_t invertedCode = 0;
    uint8_t bitVal;
    while(PIND & _BV(PD4));
    time = TCNT1;
    TCNT1 = 0;
    if(time <= CONV(4200)) {
      return;
    }
    for(uint8_t i = 0; i < 32; i++) {
      while(!(PIND & _BV(PD4)));
      while(PIND & _BV(PD4));
      time = TCNT1;
      TCNT1 = 0;
      if(time > CONV(1650))	{
        bitVal = 1;
      } else {
        bitVal = 0;
      }
      if((i < 8) || (i >= 16 && i < 24)){
        code = code << 1;
        code |= bitVal;
      }
      if((i >= 8 && i < 16) || (i >= 24 && i < 32))	{
        invertedCode = invertedCode << 1;
        invertedCode |= bitVal;
      }
    }
    if(code == ~invertedCode && code != 0) {
      _handler->handleIrCommand(code);
    }
  }
}