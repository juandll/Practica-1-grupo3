#if !defined(CONSUMO_H)
#define CONSUMO_H
#ifdef __cplusplus
extern "C" {
	#endif

	#include <avr/io.h>

	void C_Power_Down(void);
    void C_Power_Safe(void);
    void C_Standby(void);
    void C_Extended_Standby(void);
    void C_Watchdog_Function(void);
    void C_Interrupt_Enable(void);
    void C_Interrupt_Disable(void);

	#ifdef __cplusplus
} // extern "C"
#endif
#endif