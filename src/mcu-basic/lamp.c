#ifndef F_CPU
#define F_CPU 16000000UL
#endif
 
#include <avr/io.h>
#include <util/delay.h>

// PD3 - red
// PD5 - green
// PD6 - blue
 
int main(void) {
   DDRD = (1 << PD5);
   while(1) {
       PORTD = (1 << PD5);
       _delay_ms(500);
       PORTD = 0x00;
       _delay_ms(500);
   }
}