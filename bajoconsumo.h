
#if !defined(CONSUMO_H)
#define CONSUMO_H
#ifdef __cplusplus
extern "C" {
#endif

/*Hubo teclas presionadas*/
#define BANDERA_TECLAS_ON 0xFFU

/*Modos de APAGAR*/
#define POWER_MODE 'P'
#define DOWN 'D'
#define SAFE 'S'

#define STANDBY_MODE '$'
#define STANDBY 'Y'
#define EXTENDED_STANDBY 'E'

/*Modos de ENCENDER*/
#define WAKEUP 'W'
#define EXTERNAL 'X' //EXTERNAL_RESET_FLAG
#define WATCHDOG 'G' 

/*Estados Máquina*/
#define ESTADO0 1
#define ESTADO1 2
#define ESTADO2 3
#define ESTADO3 4
#define ESTADO4 5
#define ESTADO5 6
#define ESTADO6 7
#define ESTADO7 8
#define ESTADO8 9

typedef struct Comunicacion Comunicacion;
struct Comunicacion{
    char estado;
    char tecla;
    char teclado_enable;
};

void Atencion_Bajo_Consumo(Comunicacion *com);
char Hubo_Tecla(Comunicacion *tec_enable);

#ifdef __cplusplus
} // extern "C"
#endif
#endif