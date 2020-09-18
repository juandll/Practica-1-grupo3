#include "Timers.h"
#include <avr/interrupt.h>

void T_inicie_timer(){
	cli(); // stop interrupts
	TCCR0A |= (1 << WGM01);
	OCR0A = 0xF9;
	TIMSK0 |= (1 << OCIE0A);
	TCCR0B |= (1 << CS02);
	TCCR1A = 0; // set entire TCCR1A register to 0
	TCCR1B = 0; // same for TCCR1B
	TCNT1  = 0; // initialize counter value to 0
	// set compare match register for 10 Hz increments
	OCR1A = limite; // = 16000000 / (64 * 10) - 1 (must be <65536)
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12, CS11 and CS10 bits for 64 prescaler
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei();
	
}

void T_Timer_1(){
	// TIMER 1 for interrupt frequency 5 Hz:
	cli(); // stop interrupts
	TCCR1A = 0; // set entire TCCR1A register to 0
	TCCR1B = 0; // same for TCCR1B
	TCNT1  = 0; // initialize counter value to 0
	// set compare match register for 5 Hz increments
	OCR1A = limite; // = 16000000 / (64 * 5) - 1 (must be <65536)
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12, CS11 and CS10 bits for 64 prescaler
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei(); // allow interrupts
}