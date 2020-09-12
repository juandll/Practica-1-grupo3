#include <avr/io.h>
#include <avr/interrupt.h>
#include "nuestrostimers.h"
#include "display.h"
#include "definiciones_y_configuraciones.h"
#include "serialutility.h"


// En nuestra implementacion esta deberia ser un global 
// si vamos a trabajar por interrupción o deberia estar en el
// espacio de memoria del main.


void main (void)
{
    Tm_Periodico sondeoADC,sondeoDisplay;
    // suponemos que int8_t es un typedef de char entero con sigo de 8 bits
    D_Display disp;
    Comunicacion teclado;

    uint16_t ubrr=103; // valor para conseguir los 9600 baudios
    int8_t temperatura;//ya va a estar en celsius
    int8_t tempUnidades;//vamos a guardar las unidades
    int8_t tempUnidadesD;//vamos a guardar el BCD del puerto D unidades
    int8_t tempUnidadesB;//vamos a guardar el BCD del puerto B unidades
    int8_t tempDecenas;//vamos a guardar las unidades  
    int8_t tempDecenasD;//vamos a guardar el BCD del puerto D decenas  
    int8_t tempDecenasB;//vamos a guardar el BCD del puerto B decenas   
    int8_t banderaADC; //bandera que indica que se esta realizando la conversión ADC
    Tm_Inicie_periodico (&sondeoADC,TIEMPOADC);// iniciar periodico de ADC
    Tm_Inicie_periodico (&sondeoDisplay,TIEMPODISPLAY);// iniciar periodico de Display
    D_inicie_display(&disp,&tempUnidadesD,&tempUnidadesB,&tempDecenasD,&tempDecenasB);
    Su_inicie_uart(ubrr,&teclado);
    T_inicie_timer();
    DyC_inicialice_ADC(&banderaADC);

    for(;;)
    {
        //loop(..);
        if(Su_Hubo_Tecla_Serial(&teclado))
        {
            Su_Atencion_Bajo_Consumo(&teclado);
        }
        if(TIMER0_COMPA_vect)// supongamos que tenemos un timer por hardware de 1 ms
        {
            /*Atencion 1*/
            //reseteamos el timer
            Tm_Procese_tiempo (&sondeoADC);
            Tm_Procese_tiempo (&sondeoDisplay);
        }

        if(~(ADCSRA & (1 << ADSC)) && banderaADC)// Interrupción lea ADC
        {
            DyC_Procese_ADC(&temperatura,&tempUnidades,&tempDecenas,&tempUnidadesD,&tempUnidadesB,&tempDecenasD,&tempDecenasB, &banderaADC);
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
            D_Procese_Display(&disp);
        }
        //if (/*apague el timer tyt*/)
        //    Tm_Termine_periodico (&tyt);
    }
// Nunca deberia llegar aquí
}