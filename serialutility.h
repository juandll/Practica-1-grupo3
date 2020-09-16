
#if !defined(CONSUMO_H)
#define CONSUMO_H
#ifdef __cplusplus
extern "C" {
#endif

#include <avr/io.h>
/*Hubo teclas presionadas*/
#define BANDERA_TECLAS_ON 0xFFU

/*Modos de APAGAR*/
#define POWER_MODE 'P'
#define DOWN 'D'
#define SAFE 'S'

#define STANDBY_MODE '$'
#define STANDBY 'Y'
#define EXTENDED_STANDBY 'E'

#define	PMASK	0xEF;

/*Estados Máquina*/
typedef enum{
    ESTADO0,
    ESTADO1,
    ESTADO2
}ESTADOS;

typedef struct Comunicacion Comunicacion;
struct Comunicacion{
    ESTADOS estado;
    char tecla;
    char teclado_enable;
};
void Su_inicie_uart(uint16_t ubrr, Comunicacion *com);
void Su_Atencion_Bajo_Consumo(Comunicacion *com);
char Su_Hubo_Tecla_Serial(Comunicacion *tec_enable);
void Su_Trasmicion(int8_t *tempUnidades, int8_t *bandera, int8_t *tempDecenas);
void Su_Watchdog_Function(void);
void Su_Interrupt_Enable(void);
void Su_Interrupt_Disable(void);

#ifdef __cplusplus
} // extern "C"
#endif
#endif