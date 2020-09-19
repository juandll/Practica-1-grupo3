#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "nuestrostimers.h"
#include "display.h"
#include "definiciones_y_configuraciones.h"
#include "serialutility.h"
#include "Timers.h"


uint8_t banderaSerial=0;
uint8_t banderaMili=0;

// En nuestra implementacion esta deberia ser un global 
// si vamos a trabajar por interrupción o deberia estar en el
// espacio de memoria del main.


void main (void)
{
    Tm_Periodico sondeoADC,sondeoDisplay;
    // suponemos que int8_t es un typedef de char entero con sigo de 8 bits
    D_Display disp;
    Comunicacion teclado;

    uint16_t ubrr=103;      //valor para conseguir los 9600 baudios
	int8_t bandera_tx=0;    //bandera de transferencia de datos
    int8_t temperatura;     //ya va a estar en celsius
    int8_t tempUnidades;    //vamos a guardar las unidades
    int8_t tempUnidadesD;   //vamos a guardar el BCD del puerto D unidades
    int8_t tempUnidadesC;   //vamos a guardar el BCD del puerto B unidades
    int8_t tempDecenas;     //vamos a guardar las unidades  
    int8_t tempDecenasD;    //vamos a guardar el BCD del puerto D decenas  
    int8_t tempDecenasC;    //vamos a guardar el BCD del puerto B decenas   
    int8_t banderaADC;      //bandera que indica que se esta realizando la conversión ADC
    Tm_Inicie_periodico (&sondeoADC,TIEMPOADC);           // iniciar periodico de ADC
    Tm_Inicie_periodico (&sondeoDisplay,TIEMPODISPLAY);   // iniciar periodico de Display
    D_inicie_display(&disp,&tempUnidadesD,&tempUnidadesC,&tempDecenasD,&tempDecenasC); //inicializa la estructura disp
    Su_inicie_uart(ubrr,&teclado);     //inicializa el uart
    T_inicie_timer();                  //inicializa los timers 0 y 1
    DyC_inicialice_ADC(&banderaADC);   //inicializa el ADC

    for(;;)
    {
        //loop(..);
        if(Su_Hubo_Tecla_Serial(&teclado)) //Entra cuando detecta recepción del serial
        {
            Su_Interrupt_Enable();
            Su_Atencion_Bajo_Consumo(&teclado);
        }
        if(banderaMili)// supongamos que tenemos un timer por hardware de 1 ms
        {
            banderaMili=0;
            //reseteamos el timer
            Tm_Procese_tiempo (&sondeoADC);
            Tm_Procese_tiempo (&sondeoDisplay);
        }

        if(~(ADCSRA & (1 << ADSC)) && banderaADC)// Interrupción lea ADC
        {
            DyC_Procese_ADC(&temperatura,&tempUnidades,&tempDecenas,&tempUnidadesD,&tempUnidadesC,&tempDecenasD,&tempDecenasC, &banderaADC);
            /*en este lugar leemos el ADC y convertimos el valor leido a 
             grados y a unidades y decenas para el display
            */
        }

        if(Tm_Hubo_periodico (&sondeoADC))// condicion de ADC
        {
            Tm_Baje_periodico (&sondeoADC); //reset de condicion ADC
            DyC_empiece_ADC(&banderaADC); // en este lugar se empieza la conversión del ADC.
        }
        
        if(Tm_Hubo_periodico (&sondeoDisplay))// condicion de Display entra cada 
        //TIEMPODISPLAY 10 milisegundos
        {
            Tm_Baje_periodico (&sondeoDisplay);//reset de condicion
            D_Procese_display(&disp);
        }
       
        if(banderaSerial)// condicion de Display entra cada 
        //TIEMPODISPLAY 200 milisegundos
        {
            banderaSerial=0;
            Su_Trasmicion(&tempUnidades,&bandera_tx,&tempDecenas);
        }
        //if (/*apague el timer tyt*/)
        //    Tm_Termine_periodico (&tyt);
    }
// Nunca deberia llegar aquí
}

ISR(TIMER1_COMPA_vect){
	//se habilita la transmisión serial
	banderaSerial=1;
}
ISR(TIMER0_COMPA_vect){
	banderaMili=1;
}
ISR(INT0_vect){
    //Su_Watchdog_Function();
    SMCR &= (uint8_t)(~(1<<SE)); //baja el bit de bajo consumo
    PRR &= ~PMASK;               //Prender perifericos

}
