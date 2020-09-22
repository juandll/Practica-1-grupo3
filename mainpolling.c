#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "nuestrostimers.h"
#include "display.h"
#include "definiciones_y_configuraciones.h"
#include "serialutility.h"
#include "Timers.h"
#include "Consumo.h"

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
    int8_t banderaADC2;      //bandera que indica que se esta realizando la conversión ADC
    Tm_Inicie_periodico (&sondeoADC,TIEMPOADC);           // iniciar periodico de ADC
    Tm_Inicie_periodico (&sondeoDisplay,TIEMPODISPLAY);   // iniciar periodico de Display
    D_inicie_display(&disp,&tempUnidadesD,&tempUnidadesC,&tempDecenasD,&tempDecenasC); //inicializa la estructura disp
    Su_inicie_uart(ubrr,&teclado);     //inicializa el uart
    T_inicie_timer();                  //inicializa los timers 0 y 1
    DyC_inicialice_ADC(&banderaADC2);   //inicializa el ADC

    for(;;)
    {
        //loop(..);
        if(Su_Hubo_Tecla_Serial(&teclado)) //Entra cuando detecta recepción del serial
        {
            C_Interrupt_Enable();
            Su_Atencion_Bajo_Consumo(&teclado);
        }
        if(TIFR0 & (1 << OCF0A)// supongamos que tenemos un timer por hardware de 1 ms
        {
            //reseteamos el timer
            Tm_Procese_tiempo (&sondeoADC);
            Tm_Procese_tiempo (&sondeoDisplay);
        }

        if(~(ADCSRA & (1 << ADSC)) && banderaADC2)// Interrupción lea ADC
        {
            DyC_Procese_ADC(&temperatura,&tempUnidades,&tempDecenas,&tempUnidadesD,&tempUnidadesC,&tempDecenasD,&tempDecenasC, &banderaADC2);
            /*en este lugar leemos el ADC y convertimos el valor leido a 
             grados y a unidades y decenas para el display
            */
        }

        if(Tm_Hubo_periodico (&sondeoADC))// condicion de ADC
        {
            Tm_Baje_periodico (&sondeoADC); //reset de condicion ADC
            DyC_empiece_ADC(&banderaADC2); // en este lugar se empieza la conversión del ADC.
        }
        
        if(Tm_Hubo_periodico (&sondeoDisplay))// condicion de Display entra cada 
        //TIEMPODISPLAY 2 milisegundos
        {
            Tm_Baje_periodico (&sondeoDisplay);//reset de condicion
            D_Procese_display(&disp);
        }
       
        if(TIFR1 & (1 << OCF1A))// condicion de Display entra cada 
        //TIEMPODISPLAY 200 milisegundos
        {
            Su_Trasmicion(&tempUnidades,&bandera_tx,&tempDecenas);
        }

        if(EICRA & (1 << ISC00))//Aqui se despierta el microcontrolador de bajo consumo
        {
            C_Despertar();
        }
        //if (/*apague el timer tyt*/)
        //    Tm_Termine_periodico (&tyt);
    }
// Nunca deberia llegar aquí
}
