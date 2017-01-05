#include "keys.h"

uint8_t key[NUMKEYS] = {0};
uint8_t queuekey[NUMKEYS] = {0};

#define setmsk(_port, _msk) _port |= (_msk)
#define clrmsk(_port, _msk) _port &= ~(_msk)

ISR(TIMER0_OVF_vect)  // isr_keyhandle
{
	uint8_t i;

	cli();
	for (i = 0; i < NUMKEYS; i++)
	{
		if (key[i] == 1)
			release(i);

		if (key[i] != 0)
			key[i]--;
		else
		{
			if (queuekey[i] != 0)
			{
				queuekey[i] = 0;
				strike(i);
			}
		}
	}
	sei();
}

void keysinit(void)
{
	TCCR0A = (1 << CS01); // no output compare, normal counting mode, overflow is 8MHz/256/8 == 256us
	TIMSK0 = (1 << TOIE0); // Overflow Interrupt Enable
	
	// set up key pins
	DDRA |= 0xFF; //NOTE8 ...NOTE15
	DDRC |= 0xFF; //NOTE16...NOTE23
	DDRD |= 0xFF; //NOTE24...NOTE31
	DDRF |= 0xFF; //NOTE0 ...NOTE7
}

void queuenote(uint8_t k)
{
	cli();
	queuekey[k] = 1;
	sei();
}

void strike(uint8_t k) // go high
{
	// set the timer
	#ifdef lSTRIKE
		key[k] = lSTRIKE;
	#else
		key[k] = lstrike[k];
	#endif

	// whack it
	switch(k)
	{
		case 0:
			setmsk(PORTD, 0x04);
			break;
		case 1:
			setmsk(PORTD, 0x08);
			break;
		case 2:
			setmsk(PORTD, 0x10);
			break;
		case 3:
			setmsk(PORTD, 0x20);
			break;
		case 4:
			setmsk(PORTD, 0x40);
			break;
		case 5:
			setmsk(PORTD, 0x80);
			break;
		case 6:
			setmsk(PORTC, 0x01);
			break;
		case 7:
			setmsk(PORTC, 0x02);
			break;
		case 8:
			setmsk(PORTC, 0x04);
			break;
		case 9:
			setmsk(PORTC, 0x08);
			break;
		case 10:
			setmsk(PORTC, 0x10);
			break;
		case 11:
			setmsk(PORTC, 0x20);
			break;
		case 12:
			setmsk(PORTC, 0x40);
			break;
		case 13:
			setmsk(PORTC, 0x80);
			break;
		case 14:
			setmsk(PORTA, 0x80);
			break;
		case 15:
			setmsk(PORTA, 0x40);
			break;
		case 16:
			setmsk(PORTA, 0x20);
			break;
		case 17:
			setmsk(PORTA, 0x10);
			break;
		case 18:
			setmsk(PORTA, 0x08);
			break;
		case 19:
			setmsk(PORTA, 0x04);
			break;
		case 20:
			setmsk(PORTA, 0x02);
			break;
		case 21:
			setmsk(PORTA, 0x01);
			break;
		case 22:
			setmsk(PORTF, 0x80);
			break;
		case 23:
			setmsk(PORTF, 0x40);
			break;
		case 24:
			setmsk(PORTF, 0x20);
			break;
		case 25:
			setmsk(PORTF, 0x10);
			break;
		case 26:
			setmsk(PORTF, 0x08);
			break;
		case 27:
			setmsk(PORTF, 0x04);
			break;
		case 28:
			setmsk(PORTF, 0x02);
			break;
		case 29:
			setmsk(PORTF, 0x01);
			break;
		default:
			setmsk(PORTB, 0x01);
			break;
	}
}

void release(uint8_t k) // go low
{
	switch(k)
	{
		case 0:
			clrmsk(PORTD, 0x04);
			break;
		case 1:
			clrmsk(PORTD, 0x08);
			break;
		case 2:
			clrmsk(PORTD, 0x10);
			break;
		case 3:
			clrmsk(PORTD, 0x20);
			break;
		case 4:
			clrmsk(PORTD, 0x40);
			break;
		case 5:
			clrmsk(PORTD, 0x80);
			break;
		case 6:
			clrmsk(PORTC, 0x01);
			break;
		case 7:
			clrmsk(PORTC, 0x02);
			break;
		case 8:
			clrmsk(PORTC, 0x04);
			break;
		case 9:
			clrmsk(PORTC, 0x08);
			break;
		case 10:
			clrmsk(PORTC, 0x10);
			break;
		case 11:
			clrmsk(PORTC, 0x20);
			break;
		case 12:
			clrmsk(PORTC, 0x40);
			break;
		case 13:
			clrmsk(PORTC, 0x80);
			break;
		case 14:
			clrmsk(PORTA, 0x80);
			break;
		case 15:
			clrmsk(PORTA, 0x40);
			break;
		case 16:
			clrmsk(PORTA, 0x20);
			break;
		case 17:
			clrmsk(PORTA, 0x10);
			break;
		case 18:
			clrmsk(PORTA, 0x08);
			break;
		case 19:
			clrmsk(PORTA, 0x04);
			break;
		case 20:
			clrmsk(PORTA, 0x02);
			break;
		case 21:
			clrmsk(PORTA, 0x01);
			break;
		case 22:
			clrmsk(PORTF, 0x80);
			break;
		case 23:
			clrmsk(PORTF, 0x40);
			break;
		case 24:
			clrmsk(PORTF, 0x20);
			break;
		case 25:
			clrmsk(PORTF, 0x10);
			break;
		case 26:
			clrmsk(PORTF, 0x08);
			break;
		case 27:
			clrmsk(PORTF, 0x04);
			break;
		case 28:
			clrmsk(PORTF, 0x02);
			break;
		case 29:
			clrmsk(PORTF, 0x01);
			break;
		default:
			clrmsk(PORTB, 0x01);
			break;
	}
}
