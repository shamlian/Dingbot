//#define TESTMODE

#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

#define UART_UBRR_CALC(BAUD_,FREQ_) ((FREQ_)/((BAUD_)*16L)-1)
#define UART_2XUBRR_CALC(BAUD_,FREQ_) ((FREQ_)/((BAUD_)*8L)-1)

#define LED_ON  setmsk(PORTB, 0x01)
#define LED_OFF clrmsk(PORTB, 0x01)

void delay_ms(uint8_t ms)
{
	for(; ms >= 32; ms-=32)
		_delay_ms(32);
	_delay_ms(ms);
}

#include "midi.c"
#include "keys.c"

int main (void)
{
	DDRB |= 0x01; // LED pin is an output.
	LED_ON;
	
//set up keys
	keysinit();
	
// set up MIDI
	midiinit();

	LED_OFF;
	
	sei();

	while(1)
	{
		#ifndef TESTMODE
			midi_buf_parse();
		#else
			for (uint8_t i = 0; i < NUMKEYS; i++)
			{
				strike(i);
				LED_ON;
				delay_ms(100);
				LED_OFF;
				delay_ms(100);
			}
		#endif
	}
    return (0);
}
