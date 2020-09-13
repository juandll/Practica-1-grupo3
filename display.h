
#if !defined(DISPLAY_H)
#define DISPLAY_H
#ifdef __cplusplus
extern "C" {
#endif
//#include <stddef.h>
typedef struct D_Display D_Display;

#define UNIDADES 1// si e estado esta en este muestro unidades
#define DECENAS  2// si el estado esta en este muestro decenas
#define PORTBMASK 0b00000111 //Mascara del puerto B
#define PORTDMASK 0b11111100 //Mascara del puerto D
/*el transistor que habilita el display de unidades esta en el puerto B bit 2*/
#define TRANDISPUNIDADES 0b00000010

/*el transistor que habilita el display de decenas esta en el puerto C bit 5*/
#define TRANDISPDECENAS 0b00000100

struct D_Display
{
    int8_t *tempUnidadesD;
    int8_t *tempUnidadesB;
    int8_t *tempDecenasD;
    int8_t *tempDecenasB;
    int8_t mostrando;
};

//funciones para  procesar display
void D_Procese_display (D_Display *disp);
void muestre_en_display(uint8_t digitoD  , uint8_t display , uint8_t digitoB );


//funciones de  display
void D_inicie_display (D_Display *disp,int8_t *tempUnidadesD,int8_t *tempUnidadesB,int8_t *tempDecenasD,int8_t *tempDecenasB);
void D_apague_display (D_Display *disp);

//lector de tabla de lookup entra numero sale en BCD
int8_t D_num2portD(int8_t num);
int8_t D_num2portB(int8_t num);



#ifdef __cplusplus
} // extern "C"
#endif
#endif


