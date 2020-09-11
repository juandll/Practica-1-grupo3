#include "display.h"
#include "definiciones_y_configuraciones.h"
#include <avr/io.h>

short leaADC()
{
    ADMUX = 0x00; 
    ADCSRA = 0x00;

    ADMUX |= 0b00001000;  // ADMUX 8,  0b00001000
    ADMUX |= 0b11000000;  // REF 1.1V, 0b11000000
    ADCSRA |= 0b11000111; //preescalización de 128 (1 << ADPS2)(1 << ADPS1)(1 << ADEN) e inicia la conversión (1 << ADSC)
    while(ADCSRA & (1 << ADSC)); // Entra al loop cuando está realizando la conversión, sale cuando la termina
    short adcval= (ADCL | (ADCH << 8)); //Guarda el dato de los registros de datos del ADC 
    return adcval;
}

int8_t convierta_a_Celsius(short adcval)
{
    int8_t tempVal; //Guarda valor temperatura en Celcius
    tempVal = (adcval - TEMPOFFSET)/VREFF;
    return tempVal; //Retorna el valor de temperatura en Celcius
}

int8_t saqueUnidades(int8_t tempVal)
{
    int8_t decenas= tempVal/10;       //Divide el valor de temperatura en 10 para hallar las decenas
    int8_t unidades=tempVal - (10*decenas);  //Se restan las decenas al valor de temperatura para hallar las unidades
    return unidades;                  //Retorna el valor de unidades
}

int8_t saqueDecenas(int8_t tempVal)
{
    int8_t decenas= tempVal/10;       //Divide el valor de temperatura en 10 para hallar las decenas
    return decenas;                   //Retorna el valor de decenas
}

void DyC_Procese_ADC(int8_t *temperatura,int8_t *tempUnidades,int8_t *tempDecenas,int8_t *tempUnidadesD,int8_t *tempUnidadesB,int8_t *tempDecenasD,int8_t *tempDecenasB)
{
    short adcval=leaADC(); //Lectura del ADC
    *temperatura=convierta_a_Celsius(adcval); //Conversion del temperatura a Celcius
    *tempUnidades=saqueUnidades(*temperatura); //
    *tempDecenas=saqueDecenas(*temperatura);
    *tempUnidadesD=D_num2portD(*tempUnidades);
    *tempUnidadesB=D_num2portB(*tempUnidades);
    *tempDecenasD=D_num2portD(*tempDecenas);
    *tempDecenasB=D_num2portB(*tempDecenas);


    //no tengo retun :P
}

