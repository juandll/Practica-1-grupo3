
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
typedef enum{
    ESTADO0,
    ESTADO1,
    ESTADO2,
    ESTADO3,
    ESTADO4,
    ESTADO5,
    ESTADO6,
    ESTADO7,
    ESTADO8
}ESTADOS;

typedef struct Comunicacion Comunicacion;
struct Comunicacion{
    ESTADOS estado;
    char tecla;
    char teclado_enable;
};

void Atencion_Bajo_Consumo(Comunicacion *com);
char Hubo_Tecla(Comunicacion *tec_enable);

#ifdef __cplusplus
} // extern "C"
#endif
#endif