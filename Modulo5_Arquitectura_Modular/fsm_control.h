#ifndef FSM_CONTROL_H
#define FSM_CONTROL_H

#include <stdint.h>

typedef enum {
    ESTADO_MONITOREO = 0,
    ESTADO_ALERTA,
    ESTADO_FALLA
} EstadoSistema_t;

void FSM_Init(void);
void FSM_Actualizar(uint32_t lectura_adc);

#endif
