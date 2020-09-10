#include "bajoconsumo.h"

char Hubo_Tecla(Comunicacion *tec_enable){
    return (tec_enable->teclado_enable & BANDERA_TECLAS_ON);
}

void Atencion_Bajo_Consumo(Comunicacion *com){
    com->estado = ESTADO0;
    switch (com->estado)
    {
        case ESTADO0: //Verificar encendido
            if ((com->tecla)==POWER_MODE){
                com->estado=ESTADO1;
            }
            else{
                com->estado=ESTADO0;
            }            
        break;
        case ESTADO1: //Power mode = Power Down
            if ((com->tecla)==DOWN){
                Power_Down();
                com->estado=ESTADO0;
            }
            else{
                com->estado=ESTADO0;
            } 
        break;
        case ESTADO2: //Power mode = Power Safe
            if ((com->tecla)==SAFE){
                Power_Safe();
                com->estado=ESTADO0;
            }
            else{
                com->estado=ESTADO0;
            } 
        break;
        case ESTADO3: //Standby mode
            if ((com->tecla)==STANDBY_MODE){
                com->estado=ESTADO4;
            }
            else{
                com->estado=ESTADO0;
            } 
        break;
        case ESTADO4: //Standby mode = Standby
            if ((com->tecla)==STANDBY){
                Standby();
                com->estado=ESTADO0;
            }
            else{
                com->estado=ESTADO0;
            } 
        break;
        case ESTADO5: //Standby mode = Extended_Standby
            if ((com->tecla)==STANDBY_MODE){
                Extended_Standby();
                com->estado=ESTADO0;
            }
            else{
                com->estado=ESTADO0;
            } 
        break;
        case ESTADO6: //WAKEUPPPP
            if ((com->tecla)==WAKEUP){
                com->estado=ESTADO7;
            }
            else{
                com->estado=ESTADO0;
            } 
        break;
        case ESTADO7: //Wakeup con External Reset
            if ((com->tecla)==EXTERNAL){
                External_Reset_Flag();
                com->estado=ESTADO0;
            }
            else{
                com->estado=ESTADO0;
            } 
        break;
        case ESTADO8: //Wakeup con Warchdog
            if ((com->tecla)==WATCHDOG){
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
    WDTCSR |= 0b00010000; // Watchdog Change Enable
    WDTCSR |= 0b00000110; // Watchdog timer prescaler of 128k

    //For seting up the watchdog
    MCUSR &= ~(0b00001000);
}