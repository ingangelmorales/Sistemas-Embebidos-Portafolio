#include <stm32c0xx_hal.h> // Se incluye solo para mapear los nombres de los registros
#include "driver_analogico.h"

void Driver_Sistemas_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_ADC_CLK_ENABLE();

    // 2. Encendido del ADC (Lo que acabamos de agregar)
    ADC1->CR |= ADC_CR_ADVREGEN; 
    ADC1->CR |= ADC_CR_ADCAL; 
    while (ADC1->CR & ADC_CR_ADCAL); 
    ADC1->CR |= ADC_CR_ADEN; 

    ADC1->CHSELR |= ADC_CHSELR_CHSEL0; 
}


uint32_t Driver_Leer_Voltaje(void) {
    uint32_t valor_actual = ADC1->DR;
    ADC1->CR |= ADC_CR_ADSTART; 
    
    return valor_actual;
}



void Driver_Escribir_Potencia(uint32_t potencia) {
    TIM3->CCR1 = potencia;
}
