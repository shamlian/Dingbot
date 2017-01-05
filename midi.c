#include "keys.h"

#define MIDI_BAUD_RATE 31250
#define MIDI_BUF_LEN 16

#define MIDI_CMD_OFF 0x8
#define MIDI_CMD_ON 0x9

uint8_t _midi_note, _midi_rawcmd, _midi_cmd, _midi_chan, _midi_vel;  // for midi commands
uint8_t _midi_buf[MIDI_BUF_LEN], _midi_bget = 0, _midi_bput = 0, _midi_blen = 0; // for circular buffer

uint8_t midi_channel;

uint8_t midi_buf_get(void) // blocking
{
	uint8_t c;
	
	UCSR0B &= ~(1<<RXCIE0);  // Disable receiver interrupt	
	if (_midi_blen == 0) // buffer is empty
		return (uint8_t)(-1);
		
	c = _midi_buf[_midi_bget];
	
	_midi_bget++;
	if (_midi_bget >= MIDI_BUF_LEN)
		_midi_bget = 0;
		
	_midi_blen--;
	
	UCSR0B |= (1<<RXCIE0);  // Enable receiver interrupt	
	
	return c;
}

ISR(USART0_RX_vect) // midi usart receive
{
	if (_midi_blen == MIDI_BUF_LEN) // buffer is full
		return;  //fail silently :P
	
	_midi_buf[_midi_bput] = UDR0;
	
	_midi_bput++;
	if (_midi_bput >= MIDI_BUF_LEN)
		_midi_bput = 0;
	
	_midi_blen++;
}

void midi_buf_parse(void)
{
	uint8_t c;
	
	while (_midi_blen != 0) // there's stuff in the buffer
	{
		c = midi_buf_get();
	
	//A simple midi parsing routine.  Will break if receiving 0xF0, 0xF2, OxF3.  Will ignore any command except note on and note off.
		if (c >= 0x80) // we got a command
		{
			if (!(c >= 0xF0))
			{
				_midi_rawcmd = c;
				_midi_note = _midi_vel = 0xFF;
				_midi_cmd = _midi_rawcmd >> 4;
				_midi_chan = _midi_rawcmd & 0xF;
			}
		}
		else // we got data
		{
			if (_midi_cmd == MIDI_CMD_OFF || _midi_cmd == MIDI_CMD_ON) // previous command was note on or off
			{
				if (_midi_note == 0xFF)
					_midi_note = c;
				else
				{
					_midi_vel = c;
					
					// start processing.
					if (_midi_chan == midi_channel || (midi_channel == 9 && _midi_chan != 9)) // play if it's our channel or for all notes if the board is set to 9, which is the drum channel
					{
						while (_midi_note < STARTKEY)
							_midi_note += 12;
						while (_midi_note > (STARTKEY+NUMKEYS-1))
							_midi_note -= 12;
							
						if (_midi_cmd == MIDI_CMD_ON && _midi_vel != 0) 
							queuenote(_midi_note - STARTKEY);
					}
					
					_midi_note = 0xFF; // clear the note (could also clear velocity)
				}
			}
		}
	}
}

void delay_ms(uint8_t ms);

void midiinit(void)
{
	uint8_t d;
	
	// set up MIDI port as one start bit (must be 0), eight data bits, one stop bit (must be 1), no parity and no hardware handshaking. The Baud rate is 31250 bps (±1%)
	UBRR0H = (uint8_t)(UART_2XUBRR_CALC(MIDI_BAUD_RATE,F_CPU)>>8); // Set baud rate
	UBRR0L = (uint8_t)UART_2XUBRR_CALC(MIDI_BAUD_RATE,F_CPU);
	UCSR0A = (1<<U2X0);				// set baud rate speed doubler
	UCSR0B = (1<<RXEN0) | (1<<RXCIE0);  // Enable receiver and receiver interrupt
	UCSR0C = (0<<USBS0) | (3<<UCSZ00); // Set frame format: 8data, 1 start bit, one stop bit, no parity
	
	_midi_note = _midi_rawcmd = _midi_cmd = _midi_vel = 0xFF;
	
	// set up channel input and store channel number
	PORTE |= 0xF0; // turn on pull-ups
	delay_ms(100);  // let things settle
	d = (~PINE) >> 4; // sample and decode
	midi_channel =  ((d & 1) ? 4 : 0);
	midi_channel += ((d & 2) ? 8 : 0);
	midi_channel += ((d & 4) ? 1 : 0);
	midi_channel += ((d & 8) ? 2 : 0);
	PORTE &= 0x0F; // shut off pull ups to save power (unnecessary but meh)
}
