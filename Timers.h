#if !defined(TIEMPO2_H)
#define TIEMPO2_H
#ifdef __cplusplus
extern "C" {
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#define limite 49999

void T_inicie_timer(void);
void T_Timer_1(void);

#ifdef __cplusplus
} // extern "C"
#endif
#endif