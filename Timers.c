#include "Timers.h"
#include <avr/interrupt.h>

void T_inicie_timer(){
	cli(); 						// Detiene las interrupciones
	TCCR0A |= (1 << WGM01); 	// Se coloca el timer 0 en CTC
	OCR0A = 0xF9; 				// Se coloca el limite de conteo del timer 0
	TIMSK0 |= (1 << OCIE0A); 	// Se configura el preescalizadore del timer 0
	TCCR0B |= (1 << CS02); 		// Se configura el timer en interrupcion a 
								// comparacion el timer 0 
	TCCR1A = 0; 				// Se coloca en 0 el registro
	TCCR1B = 0; 				// Se coloca en 0 el registro
	TCNT1  = 0;					// Se coloca en 0 el registro
								// Se acomoda el limite del registro para que cuente a 5Hz
	OCR1A = limite; 			// = 16M / (64 * 5) - 1 (must be <65536) en timer 1
	TCCR1B |= (1 << WGM12); 	// Se aplica el CTC en timer 1
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);// Se coloca un preescalizado de 64
								// en timer 1
	TIMSK1 |= (1 << OCIE1A);	// Se configura el timer en interrupcion a 
								// comparacion el timer 1
	sei();
	
}
