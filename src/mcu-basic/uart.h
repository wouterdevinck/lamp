// #include "uart.h"
// #include <stdio.h>
// #include <stdlib.h>
// USART_Init(MYUBRR);
// SendString("Boot\n");
// char* msg = (char*)malloc(50);
// sprintf(msg, "0x%04X\n", code);
// SendString(msg);

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void SendString(char *StringPtr);

void USART_Init( unsigned int ubrr) {
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit( unsigned char data ) {
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

unsigned char USART_Receive( void ) {
   return UDR0;
}

void SendString(char *StringPtr) {
	while(*StringPtr != 0x00) {  
	  USART_Transmit(*StringPtr);    
	  StringPtr++;
	}
}