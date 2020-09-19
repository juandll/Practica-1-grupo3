
#include "display.h"
#include <avr/io.h>


void D_inicie_display (D_Display *disp,int8_t *tempUnidadesD,int8_t *tempUnidadesC,int8_t *tempDecenasD,int8_t *tempDecenasC)
{
    //Se inicializa la estructura disp con los valores de decenas y unidades para las puertos D y C
    disp->tempUnidadesD=tempUnidadesD;
    disp->tempUnidadesC=tempUnidadesC;
    disp->tempDecenasD=tempDecenasD;
    disp->tempDecenasC=tempDecenasC;
    disp->mostrando=UNIDADES;
}


void D_Procese_display (D_Display *disp)
{
    switch(disp->mostrando){
        case UNIDADES:                //mostramos unidades
            disp->mostrando=DECENAS;  // cambiar estado
            //muestro la conversión a unidades leidas por el ADC y convertidas
            muestre_en_display(*disp->tempUnidadesD,UNIDADES,*disp->tempUnidadesC ); 

        break;
        case DECENAS:                    //mostrar decenas
            disp->mostrando=UNIDADES;    // cambiar estado
            muestre_en_display(*disp->tempDecenasD,DECENAS,*disp->tempDecenasC ); 
		break;
        default:
            disp->mostrando=DECENAS;
		break;
    }
}

void muestre_en_display(uint8_t digitoD  , uint8_t display , uint8_t digitoC )
{
    /*configura el puerto o los puertos que se estan usando para el display*/

    /*primero bajar todos los bits del display en el puerto C*/    
    PORTC &= ~PORTCMASK;
    
    /*primero bajar todos los bits del display en el puerto C*/    
    PORTD &= ~PORTDMASK;

    /*dependiendo del valor de display enciende el transistor correspondiente*/
    if(display==UNIDADES){
        PORTC |= digitoC;          //Se activan los pines del purto C para las unidades
        PORTD |= digitoD;          //Se activan los pines del purto D para las unidades
        PORTC |= TRANDISPUNIDADES; //Se activa el transistor que habilita el display de unidades
    }
    if(display==DECENAS){
        PORTC |= digitoC;          //Se activan los pines del purto C para las decenas
        PORTD |= digitoD;          //Se activan los pines del purto D para las decenas
        PORTC |= TRANDISPDECENAS;  //Se activa el transistor que habilita el display de decenas
    }
}

/*
     a
     _
f | g_  | b
e |  _  | c
     d

*/

const int8_t tablaD[12]={0b11011000,0b00010000,0b10101000,0b10111000,0b01110000,0b11111000,0b11111000,0b10010000,0b11111000,0b11111000,0b00100000,0b11101000};/*del 0 al 9 y menos y error*/
const int8_t tablaC[12]={0b00001001,0b00000001,0b00001001,0b00000001,0b00000001,0b00000000,0b00001000,0b00000001,0b00001001,0b00000001,0b00000000,0b00001000};/*del 0 al 9 y menos y error*/

//Tabla de look up del puert D
int8_t D_num2portD(int8_t num)
{
    if(num>=12||num<0)
        return tablaD[11];/*en la pos 11 de la tabla esta el error*/
    return tablaD[num];
}

//Tabla de look up del puert C
int8_t D_num2portC(int8_t num)
{
    if(num>=12||num<0)
        return tablaC[11];/*en la pos 11 de la tabla esta el error*/
    return tablaC[num];
}



