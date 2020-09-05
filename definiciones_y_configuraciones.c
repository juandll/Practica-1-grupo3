#include "display.h"

void DyC_Procese_ADC(int8_t *temperatura,int8_t *tempUnidades,int8_t *tempDecenas)
{
    short adcval=leaADC(); //Lectura del ADC
    *temperatura=convierta_a_Celsius(adcval); //Conversion del temperatura a Celcius
    *tempUnidades=saqueUnidades(*temperatura); //
    *tempDecenas=saqueDecenas(*temperatura);
    *tempUnidades=num2BCD(*tempUnidades);
    *tempDecenas=num2BCD(*tempDecenas);

    //no tengo retun :P
}

void leaADC()
{
    ADMUX = 0x00; 
    ADCSRA = 0x00;

    ADMUX |= 0b00001000;  // ADMUX 8,  0b00001000
    ADMUX |= 0b11000000;  // REF 1.1V, 0b11000000
    ADCSRA |= 0b00000111; //preescalización de 128 (1 << ADPS2)(1 << ADPS1)(1 << ADEN)
}

int8_t convierta_a_Celsius(short adcval)
{
    unsigned int tempVal; //Guarda valor temperatura en Celcius
    unsigned int tempOffset = 273;  //Segun el datasheet de calibracion se debe restar un valor de 273
    int Vreff = 1.22; //El valor de voltaje de referencia sobre el canal es de 1.22V
    tempVal = (adcval - tempOffset)/Vreff;
    return tempVal; //Retorna el valor de temperatura en Celcius
}