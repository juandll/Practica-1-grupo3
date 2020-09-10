
#include "display.h"

void D_inicie_display (D_Display *disp,int8_t *tempUnidadesD,int8_t *tempUnidadesB,int8_t *tempDecenasD,int8_t *tempDecenasB)
{
    disp->tempUnidadesD=tempUnidadesD;
    disp->tempUnidadesB=tempUnidadesB;
    disp->tempDecenasD=tempDecenasD;
    disp->tempDecenasB=tempDecenasB;
    disp->mostrando=UNIDADES;
}


void D_Procese_display (D_Display *disp)
{
    switch(disp->mostrando){
        case UNIDADES://mostramos unidades
            disp->mostrando=DECENAS;// cambiar estado
            //muestro la conversión a unidades leidas por el ADC y convertidas
            muestre_en_display(*disp->tempUnidadesD,UNIDADES,*disp->tempUnidadesB ); 

        break;
        case DECENAS://mostrar decenas
            disp->mostrando=UNIDADES;// cambiar estado
            muestre_en_display(*disp->tempDecenasD,DECENAS,*disp->tempDecenasB ); 
        default:
            disp->mostrando=DECENAS;
    }
}

void muestre_en_display(uint8_t digitoD  , uint8_t display , uint8_t digitoB )
{
    /*configura el puerto o los puertos que esten usando para si display*/

    /*primero bajar todos los bits del display en el puerto*/    
    PORTB &= ~0b00000111;
    /*subir solo los necesarios*/
    
    /*primero bafar todos los bits del display en el puerto*/    
    PORTD &= ~0b11111100;
    /*subir solo los necesarios*/

    /*y dependiendo del valor de display enciende el transistor correspondiente*/
    if(display==UNIDADES){
        PORTB |= ~digitoB;
        PORTD |= ~digitoD;
        PORTB |= 0b00000010;
    }
    if(display==DECENAS){
        PORTB |= ~digitoB;
        PORTD |= ~digitoD;
        PORTB |= 0b00000100;
    }
}

/*
     a
     _
f | g_  | b
e |  _  | c
     d
anodo o catodo?? prende con 1 o con 0??? prende con 1
0=0b00111111  // muestro un 0
1=0b00000110  // muestro un 1

*/

const int8_t tablaD[12]={0b11011100,0b00010000,0b11011100,0b10101100,0b01011000,0b11111000,0b11111100,0b10110000,0b11111100,0b11111000,0b00100000,0b11101100};/*del 0 al 9 y menos y error*/
const int8_t tablaB[12]={0b00000001,0b00000001,0b00000001,0b00000001,0b00000000,0b00000000,0b00000000,0b00000001,0b00000001,0b00000001,0b00000000,0b00000000};/*del 0 al 9 y menos y error*/

int8_t num2portD(int8_t num)
{
    if(num>=12||num<0)
        return tablaD[11];/*en la pos 11 de la tabla esta el error*/
    return tablaD[num];
}

int8_t num2portB(int8_t num)
{
    if(num>=12||num<0)
        return tablaB[11];/*en la pos 11 de la tabla esta el error*/
    return tablaB[num];
}



