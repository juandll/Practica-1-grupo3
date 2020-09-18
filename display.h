
#if !defined(DISPLAY_H)
#define DISPLAY_H
#ifdef __cplusplus
extern "C" {
	#endif
	#include <avr/io.h>
	//#include <stddef.h>
	typedef struct D_Display D_Display;

	#define UNIDADES 1// si e estado esta en este muestro unidades
	#define DECENAS  2// si el estado esta en este muestro decenas
	#define PORTCMASK 0b00001111 //Mascara del puerto C
	#define PORTDMASK 0b11111000 //Mascara del puerto D
	/*el transistor que habilita el display de unidades esta en el puerto C bit 1*/
	#define TRANDISPUNIDADES 0b00000010

	/*el transistor que habilita el display de decenas esta en el puerto C bit 2*/
	#define TRANDISPDECENAS 0b00000100

	struct D_Display
	{
		int8_t *tempUnidadesD;
		int8_t *tempUnidadesC;
		int8_t *tempDecenasD;
		int8_t *tempDecenasC;
		int8_t mostrando;
	};

	//funciones para  procesar display
	void D_Procese_display (D_Display *disp);
	void muestre_en_display(uint8_t digitoD  , uint8_t display , uint8_t digitoC );


	//funciones de  display
	void D_inicie_display (D_Display *disp,int8_t *tempUnidadesD,int8_t *tempUnidadesC,int8_t *tempDecenasD,int8_t *tempDecenasC);
	void D_apague_display (D_Display *disp);

	//lector de tabla de lookup entra numero sale en BCD
	int8_t D_num2portD(int8_t num);
	int8_t D_num2portC(int8_t num);



	#ifdef __cplusplus
} // extern "C"
#endif
#endif


