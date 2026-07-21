#include <stm32c0xx_hal.h> 
#include "fsm_control.h"
#include "driver_analogico.h" 

static EstadoSistema_t estado_actual = ESTADO_MONITOREO;

// Inicializa la máquina de estados
void FSM_Init(void) {
    estado_actual = ESTADO_MONITOREO;
}

// Lógica principal de transiciones y acciones
void FSM_Actualizar(uint32_t lectura_adc) {
    
    // --- EVALUACIÓN DE TRANSICIONES (Zonas de voltaje ficticias para el ejemplo) ---
    if (lectura_adc > 3500) {
        estado_actual = ESTADO_FALLA;
    } else if (estado_actual == ESTADO_FALLA && lectura_adc > 2000) {
        estado_actual = ESTADO_FALLA;
    } else if (lectura_adc > 2500 && lectura_adc <= 3500) {
        estado_actual = ESTADO_ALERTA;
    } else {
        estado_actual = ESTADO_MONITOREO;
    }

    // --- EJECUCIÓN DE ACCIONES SEGÚN EL ESTADO ---
    switch (estado_actual) {
        case ESTADO_MONITOREO:
            Driver_Escribir_Potencia(lectura_adc);
            break;

        case ESTADO_ALERTA:
            Driver_Escribir_Potencia(lectura_adc);
            //Encender led de precaucion
            break;

        case ESTADO_FALLA:
            Driver_Escribir_Potencia(0);
            break;
    }
}
