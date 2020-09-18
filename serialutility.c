#include "serialutility.h"
#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>

void Su_inicie_uart(uint16_t ubrr, Comunicacion *com)
{
    // Configuracion de la tasa a 9600
    UBRR0L = (uint8_t)(ubrr & 0xFF);
    UBRR0H = (uint8_t)(ubrr >> 8);

    // se enciende la recepcion y transmision
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

    //inicializacion de estados
    com->estado=ESTADO0;
}

char Su_Hubo_Tecla_Serial(Comunicacion *tec_enable){
    if(UCSR0A&(1<<7))
    {
        tec_enable->tecla = UDR0;
        return 1;
    }
    else
    {
        return 0;
    }
}

void Su_Trasmicion(int8_t *tempUnidades, int8_t *bandera, int8_t *tempDecenas){
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	if(*bandera==0)
	{
    //coloca el dato de temperatura en la transmición
		*bandera= 1;
		UDR0 = *tempDecenas + '0';
	}
	else if(*bandera==1)
	{
		*bandera=2;
		UDR0 = *tempUnidades + '0';
	}
	else if (*bandera==2)
	{
		*bandera=3;
		UDR0 = 'C';
	}
	else if(*bandera==3)
	{
		*bandera=0;
		UDR0 = ' ';
	}
}


void Power_Down(){
    /*In this mode, the external Oscillator is stopped, 
    while the external interrupts, the 2-wire Serial
    Interface address watch, and the Watchdog continue 
    operating (if enabled).*/
	PORTC &= ~0b00001111;
	PORTD &= ~0b11111000;
    PRR |= PMASK; //Shutdown Peripherical
    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (0<<SM2) | (1<<SM1)| (0<<SM0); //Power Down Mode
    SMCR |= 1; //enable sleep, set SE bit
    ADCSRA &= ~(1<<7); // Disable ADC
    MCUCR |= 0b01100000; //BOD Disable ans PORTS

}

void Power_Safe(){
    /*Same as POWER DOWN BUT If Timer/Counter2 is enabled, 
    it will keep running during sleep. The device can 
    wake up from either Timer Overflow */
	PORTC &= ~0b00001111;
	PORTD &= ~0b11111000;
    PRR |= PMASK; //Shutdown Peripherical
    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (0<<SM2) | (1<<SM1)| (1<<SM0);
	SMCR |= 1; //enable sleep, set SE bit
	ADCSRA &= ~(1<<7); // Disable ADC
	MCUCR |= 0b01100000; //BOD Disable ans PORTS

}

void Standby(){ 
    /*Same as Power Down BUT the oscillator keeps running 
    because is external crystal/resonator 
    Wakes up in 6 clock cycles*/
	PORTC &= ~0b00001111;
	PORTD &= ~0b11111000;
    PRR |= PMASK; //Shutdown Peripherical
    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (1<<SM2) | (1<<SM1)| (0<<SM0);
	SMCR |= 1; //enable sleep, set SE bit
	ADCSRA &= ~(1<<7); // Disable ADC
	MCUCR |= 0b01100000; //BOD Disable ans PORTS
}

void Extended_Standby(){
    /*Same as Power Safe BUT the oscillator keeps running 
    because is external crystal/resonator 
    Wakes up in 6 clock cycles*/
	PORTC &= ~0b00001111;
	PORTD &= ~0b11111000;
    PRR |= PMASK; //Shutdown Peripherical
    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (1<<SM2) | (1<<SM1)| (1<<SM0);
	SMCR |= 1; //enable sleep, set SE bit
	ADCSRA &= ~(1<<7); // Disable ADC
	MCUCR |= 0b01100000; //BOD Disable ans PORTS
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
    WDTCSR = 0b00011000; //enable de WDCE and WDE
    WDTCSR = 0b00100110; //prescalizacion de 128k - Oscilador de 1s
    WDTCSR |= 0b01000000; //enable interrupcion WDIE
    MCUCR |= (3 << 5); //set both BODS and BODSE at the same time
    MCUCR = (MCUCR & ~(1 << 5)) | (1 << 6); //then set the BODS bit and clear the BODSE bit at the same time
}

void Su_Interrupt_Enable(){
	DDRD &= 0xFB; 
    EICRA= (1<<ISC01)|(1<<ISC00); //The rising edge of INT0 generates an interrupt request. 
    EIMSK= (1<<INT0); //Enable INT0
}
//EIFR INTF0
void Su_Interrupt_Disable(){
    EIMSK= (0<<INT0); //Disable INT0
}



void Su_Atencion_Bajo_Consumo(Comunicacion *com){
    switch (com->estado)
    {
        case ESTADO0: //Verificar encendido
			
            if ((com->tecla)==POWER_MODE){
				
                com->estado=ESTADO1;
            }
            else if((com->tecla)==STANDBY_MODE){
                 com->estado=ESTADO2;
            }
            else
            {
                 com->estado=ESTADO0;
                 Su_Interrupt_Disable();
            }
                
                       
        break;

        case ESTADO1: //Power mode 
					
            if ((com->tecla)==DOWN){
                Power_Down();
                com->estado=ESTADO0;
            }
            else if((com->tecla) == SAFE){
                Power_Safe();
                com->estado = ESTADO0;
                }
             else{
				 
                com->estado=ESTADO0;
                Su_Interrupt_Disable();
                }         
            
        break;
        
        case ESTADO2: //Standby mode 
            if ((com->tecla)==STANDBY){
                Standby();
                com->estado=ESTADO0;
            }
            else if((com->tecla)==EXTENDED_STANDBY){
                Extended_Standby();
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
