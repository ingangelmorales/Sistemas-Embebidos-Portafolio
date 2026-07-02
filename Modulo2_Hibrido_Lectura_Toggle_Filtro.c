#include <stm32c0xx_hal.h>

void GPIO_Config(void) {
    // 1. Encedemos el reloj físico del Puerto A 
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // CONFIGURACIÓN DEL LED (PA5) - SALIDA
    GPIO_InitStruct.Pin = GPIO_PIN_5;         
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
    GPIO_InitStruct.Pull = GPIO_NOPULL;       
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; 
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // CONFIGURACIÓN DEL BOTÓN (PA0) - ENTRADA
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;     // Modo entrada digital
    GPIO_InitStruct.Pull = GPIO_PULLUP;        // Activamos resistencia Pull-Up interna
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // CONFIGURACIÓN DEL BOTÓN (PA1) - ENTRADA
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;     // Modo entrada digital
    GPIO_InitStruct.Pull = GPIO_PULLUP;        // Activamos resistencia Pull-Up interna
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int main(void) {
    HAL_Init();
    GPIO_Config();

    while (1) {
        // --- USANDO HAL ---
        if ( (GPIOA->IDR & (1 << 0)) == 0 ) 
        {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            HAL_Delay(100); 
        }
        else 
        {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            HAL_Delay(600);
        }

        // --- USANDO BARE PARA LA ESCRITURA---
        if ( (GPIOA->IDR & (1 << 1)) == 0 ) 
        {
            GPIOA->ODR ^= (1 << 5);
            HAL_Delay(100); 
        }
        else 
        {
            GPIOA->ODR ^= (1 << 5);
            HAL_Delay(600);
        }
    }
}
