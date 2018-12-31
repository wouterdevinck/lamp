#include "Timing.h"
#include <util/delay.h>
// #include <util/atomic.h>
// #include <avr/interrupt.h>

// #define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8)

using namespace lamp;

// volatile uint32_t timer1_millis;

// ISR (TIMER1_COMPA_vect) {
//   timer1_millis++;
// }

Timing::Timing() {
  // CCR1B |= /*(1 << WGM12) |*/ (1 << CS11);
  // OCR1AH = (CTC_MATCH_OVERFLOW >> 8);
  // OCR1AL = (uint8_t)CTC_MATCH_OVERFLOW;
  // TIMSK1 |= (1 << OCIE1A);
}

uint32_t Timing::millis() {
  // uint32_t m;
  // ATOMIC_BLOCK(ATOMIC_FORCEON) {
  //   m = timer1_millis;
  // }
  // return m;
  return 0;
}

void Timing::delay(uint32_t ms) {
  while (ms--) {
    _delay_ms(1);
  }
}