#include <avr/io.h>
#include "Consumo.h"

void C_Power_Down(){
    /*In this mode, the external Oscillator is stopped, 
    while the external interrupts, the 2-wire Serial
    Interface address watch, and the Watchdog continue 
    operating (if enabled).*/
	PORTC &= ~0b00001111;
	PORTD &= ~0b11111000;
    PRR |= PMASK;                                   // Se apagan los perifericos
    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (0<<SM2) | (1<<SM1)| (0<<SM0);           // Se configura a Power Down
    SMCR |= 1;                                      // Se activa el sleep
    ADCSRA &= ~(1<<7);                              // Desabilita el ADC
    MCUCR |= 0b01100000;                            // Desabilita BOD y los puertos

}

void C_Power_Safe(){
    /*Same as POWER DOWN BUT If Timer/Counter2 is enabled, 
    it will keep running during sleep. The device can 
    wake up from either Timer Overflow */
	PORTC &= ~0b00001111;
	PORTD &= ~0b11111000;
    PRR |= PMASK;                                   // Se apagan los perifericos
    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (0<<SM2) | (1<<SM1)| (1<<SM0);           // Se configura a Power Save
	SMCR |= 1;                                      // Se activa el sleep
	ADCSRA &= ~(1<<7);                              // Desabilita el ADC
	MCUCR |= 0b01100000;                            // Desabilita BOD y los puertos

}

void C_Standby(){ 
    /*Same as Power Down BUT the oscillator keeps running 
    because is external crystal/resonator 
    Wakes up in 6 clock cycles*/
	PORTC &= ~0b00001111;
	PORTD &= ~0b11111000;
    PRR |= PMASK;                                   // Se apagan los perifericos
    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (1<<SM2) | (1<<SM1)| (0<<SM0);
	SMCR |= 1;                                      // Se activa el sleep
	ADCSRA &= ~(1<<7);                              // Desabilita el ADC
	MCUCR |= 0b01100000;                            // Desabilita BOD y los puertos
}

void C_Extended_Standby(){
    /*Same as Power Safe BUT the oscillator keeps running 
    because is external crystal/resonator 
    Wakes up in 6 clock cycles*/
	PORTC &= ~0b00001111;
	PORTD &= ~0b11111000;
    PRR |= PMASK;                                   // Se apagan los perifericos
    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (1<<SM2) | (1<<SM1)| (1<<SM0);
	SMCR |= 1;                                      // Se activa el sleep
	ADCSRA &= ~(1<<7);                              // Desabilita el ADC
	MCUCR |= 0b01100000;                            // Desabilita BOD y los puertos
}

/* Only one of these events can wake up the MCU:
• External Reset
• Watchdog System Reset
• Watchdog Interrupt
• Brown-out Reset
• 2-wire Serial Interface address match
• External level interrupt on INT
• Pin change interrupt */

void C_Watchdog_Function(){
    WDTCSR = 0b00011000;                            // Enable de WDCE y WDE
    WDTCSR = 0b00100110;                            // Prescalizacion de 128k - Oscilador de 1s
    WDTCSR |= 0b01000000;                           // Activa interrupcion WDIE
    MCUCR |= (3 << 5);                              // Activa BODS y BODSE al mismo tiempo
    MCUCR = (MCUCR & ~(1 << 5)) | (1 << 6);         // Activa BODS y limpia BODSE al mismo tiempo
}

void C_Despertar(){
    SMCR &= (uint8_t)(~(1<<SE)); //baja el bit de bajo consumo
    PRR &= ~PMASK;               //Prender perifericos
}

void C_Interrupt_Enable(){
	DDRD &= 0xFB; 
    EICRA= (1<<ISC01)|(1<<ISC00);                   // Al elevarse el INT0 realiza interrupcion 
    EIMSK= (1<<INT0);                               // Activa INT0
}
//EIFR INTF0
void C_Interrupt_Disable(){
    EIMSK= (0<<INT0);                               // Desactiva INT0
}
