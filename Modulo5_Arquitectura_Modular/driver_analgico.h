#ifndef DRIVER_ANALOGICO_H
#define DRIVER_ANALOGICO_H

#include <stdint.h>

void Driver_Sistemas_Init(void);
uint32_t Driver_Leer_Voltaje(void);
void Driver_Escribir_Potencia(uint32_t potencia);

#endif 
