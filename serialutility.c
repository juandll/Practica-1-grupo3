#include "serialutility.h"
#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>

void Su_inicie_uart(uint16_t ubrr, Comunicacion *com)
{
    UBRR0L = (uint8_t)(ubrr & 0xFF);                // Configuracion de la tasa a 9600
    UBRR0H = (uint8_t)(ubrr >> 8);                  //
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);          // se enciende la recepcion y transmision
    com->estado=ESTADO0;                            //inicializacion de estados
}

char Su_Hubo_Tecla_Serial(Comunicacion *tec_enable){
    if(UCSR0A&(1<<7))                               // Condicion de recepcion de serial
    {
        tec_enable->tecla = UDR0;                   // Guarda el valor del serial
        return 1;                                   // Dice que se recibio algo
    }
    else
    {
        return 0;                                   // Dice que no recibio nada
    }
}

void Su_Trasmicion(int8_t *tempUnidades, int8_t *bandera, int8_t *tempDecenas){
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);          // Se asegura que el serial este activado
	if(*bandera==0)                                 // Condicion para saber que debe enviar
	{
		*bandera= 1;
		UDR0 = *tempDecenas + '0';                  // Envia las unidades
	}
	else if(*bandera==1)
	{
		*bandera=2;
		UDR0 = *tempUnidades + '0';                 // Envia las decenas
	}
	else if (*bandera==2)
	{
		*bandera=3;
		UDR0 = 'C';                                 // Envia los centigrados
	}
	else if(*bandera==3)
	{
		*bandera=0;
		UDR0 = ' ';                                 // Envia un espacio
	}
}


void Power_Down(){
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

void Power_Safe(){
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

void Standby(){ 
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

void Extended_Standby(){
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

void Su_Watchdog_Function(){
    WDTCSR = 0b00011000;                            // Enable de WDCE y WDE
    WDTCSR = 0b00100110;                            // Prescalizacion de 128k - Oscilador de 1s
    WDTCSR |= 0b01000000;                           // Activa interrupcion WDIE
    MCUCR |= (3 << 5);                              // Activa BODS y BODSE al mismo tiempo
    MCUCR = (MCUCR & ~(1 << 5)) | (1 << 6);         // Activa BODS y limpia BODSE al mismo tiempo
}

void Su_Interrupt_Enable(){
	DDRD &= 0xFB; 
    EICRA= (1<<ISC01)|(1<<ISC00);                   // Al elevarse el INT0 realiza interrupcion 
    EIMSK= (1<<INT0);                               // Activa INT0
}
//EIFR INTF0
void Su_Interrupt_Disable(){
    EIMSK= (0<<INT0);                               // Desactiva INT0
}



void Su_Atencion_Bajo_Consumo(Comunicacion *com){
    switch (com->estado)
    {
        case ESTADO0:                               // Verifica si ir a Power o Standby
			
            if ((com->tecla)==POWER_MODE){
				
                com->estado=ESTADO1;                // Salida a Power
            }
            else if((com->tecla)==STANDBY_MODE){
                 com->estado=ESTADO2;               // Salida a Standby
            }
            else
            {
                 com->estado=ESTADO0;
                 Su_Interrupt_Disable();
            }
                
                       
        break;

        case ESTADO1:                               // Power mode 
					
            if ((com->tecla)==DOWN){
                Power_Down();                       // Realiza Power Down
                com->estado=ESTADO0;
            }
            else if((com->tecla) == SAFE){
                Power_Safe();                       // Realiza Power save
                com->estado = ESTADO0;
                }
             else{
				 
                com->estado=ESTADO0;
                Su_Interrupt_Disable();
                }         
            
        break;
        
        case ESTADO2: //Standby mode 
            if ((com->tecla)==STANDBY){
                Standby();                          // Realiza standby
                com->estado=ESTADO0;
            }
            else if((com->tecla)==EXTENDED_STANDBY){
                Extended_Standby();                 // Realiza extended standby
                com->estado=ESTADO0;
                }
            else{
                com->estado=ESTADO0;
                Su_Interrupt_Disable();
                }
        break;

        default:
        break;
    }
}
