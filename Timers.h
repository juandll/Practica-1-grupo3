#if !defined(TIEMPO2_H)
#define TIEMPO2_H
#ifdef __cplusplus
extern "C" {
	#endif
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#define limite 49999 //limite de conteo del timer 1

	void T_inicie_timer(void); //Se inician los timers

	#ifdef __cplusplus
} // extern "C"
#endif
#endif