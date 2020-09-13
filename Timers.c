#include "Timers.h"
#include <avr/interrupt.h>

void T_inicie_timer(){
    TCCR0A |= (1 << WGM01);
    OCR0A = 0xF9;
    TIMSK0 |= (1 << OCIE0A);
    sei();  
    TCCR0B |= (1 << CS02);
}

void T_Timer_1(){
    cli(); // stop interrupts
    // TIMER 1 for interrupt frequency 1 Hz:
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; // initialize counter value to 0
    // set compare match register for 1 Hz increments
    OCR1A = limite; // = 16000000 / (256 * 1) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12, CS11 and CS10 bits for 256 prescaler
    TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    sei(); // allow interrupts
}