#include "Timers.h"
#include <avr/interrupt.h>

void T_inicie_timer(){
    TCCR0A |= (1 << WGM01);
    OCR0A = 0xF9;
    TIMSK0 |= (1 << OCIE0A);
    sei();  
    TCCR0B |= (1 << CS02);
}
