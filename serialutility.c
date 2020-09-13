#include "serialutility.h"
#include <avr/io.h>

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
	
    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (0<<SM2) | (1<<SM1)| (0<<SM0);
    MCUCR |= 0b00111000; //BOD Disable ans PORTS
}

void Power_Safe(){
    /*Same as POWER DOWN BUT If Timer/Counter2 is enabled, 
    it will keep running during sleep. The device can 
    wake up from either Timer Overflow */

    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (0<<SM2) | (1<<SM1)| (1<<SM0);
    MCUCR |= 0b00111000; //BOD Disable ans PORTS
}

void Standby(){ 
    /*Same as Power Down BUT the oscillator keeps running 
    because is external crystal/resonator 
    Wakes up in 6 clock cycles*/

    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (1<<SM2) | (1<<SM1)| (0<<SM0);
    MCUCR |= 0b00111000; //BOD Disable ans PORTS
}

void Extended_Standby(){
    /*Same as Power Safe BUT the oscillator keeps running 
    because is external crystal/resonator 
    Wakes up in 6 clock cycles*/

    SMCR =  0X00;
    MCUCR = 0X00;
    SMCR = (1<<SM2) | (1<<SM1)| (1<<SM0);
    MCUCR |= 0b00111000; //BOD Disable ans PORTS
}

/* Only one of these events can wake up the MCU:
• External Reset
• Watchdog System Reset
• Watchdog Interrupt
• Brown-out Reset
• 2-wire Serial Interface address match
• External level interrupt on INT
• Pin change interrupt */

void External_Reset_Flag(){
    MCUSR = 0X00;
    MCUSR = (1<<EXTRF); // 0b00000010 This bit is set if an External Reset occurs.
}

void Watchdog_Function(){
    WDTCSR = 0X00;
    WDTCSR |= 0b00011000; // Watchdog Change Enable
    WDTCSR |= 0b00000110; // Watchdog timer prescaler of 128k

    //For seting up the watchdog
   // MCUSR &= ~(0b00001000);
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
            else if((com->tecla)==WAKEUP){
                 
                 com->estado=ESTADO3;
             }
             else
             {
                  com->estado=ESTADO0;
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
                }
        break;

        case ESTADO3: //Wakeup Mode
            if ((com->tecla)==EXTERNAL){
                External_Reset_Flag();
                com->estado=ESTADO0;
            }
            else if ((com->tecla)==WATCHDOG){
                Watchdog_Function();
                com->estado=ESTADO0;
                }
             else{
                 com->estado=ESTADO0;
                } 
        break;

        default:
        break;
    }
}
