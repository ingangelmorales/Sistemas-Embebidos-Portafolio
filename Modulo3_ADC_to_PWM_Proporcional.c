#include <stm32c0xx_hal.h> // Heredamos las estructuras oficiales del fabricante

// Variables para el control de datos
uint32_t valor_adc = 0;

void ADC_and_PWM_Config(void) {
    // 1. Energizar los relojes del ADC, del Puerto A y del Timer 3
    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();

    // 2. Configurar Pin PA0 como Entrada Analógica (Potenciómetro)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. Configurar Pin PA6 como FUNCIÓN ALTERNA para el Timer 3 (Canal 1 - PWM)
    // El pin PA5 de la tarjeta es solo digital, pero PA6 está conectado al hardware PWM interno
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Función alterna Push-Pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM3; // Enlaza físicamente el pin al Timer 3
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 4. Inicializar parámetros básicos del ADC por software
    ADC_HandleTypeDef hadc = {0};
    hadc.Instance = ADC1;
    hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc.Init.Resolution = ADC_RESOLUTION_12B; // 0 a 4095
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
    HAL_ADC_Init(&hadc);

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    HAL_ADC_ConfigChannel(&hadc, &sConfig);

    // 5. CONFIGURACIÓN DEL TIMER PWM AL METAL (TIM3)
    TIM3->PSC = 12 - 1;       // Prescaler: Divide el reloj para contar rápido
    TIM3->ARR = 4095;         // Límite de conteo: Hacemos que coincida EXACTAMENTE con el máximo del ADC (4095)
    
    // Configura el Canal 1 en modo PWM1 (Modificando el registro de modo CCMR1)
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos); 
    // Habilita la salida física del pin (Registro CCER)
    TIM3->CCER |= TIM_CCER_CC1E;              
    // Enciende el Timer (Registro CR1)
    TIM3->CR1 |= TIM_CR1_CEN;                 
}

int main(void) {
    HAL_Init();
    ADC_and_PWM_Config();

    Serial.begin(115200);
    delay(1000);

    Serial.println("--- DÍA 12: CONTROL PROPORCIONAL DE POTENCIA (PWM) ---");

    while (1) {
        // Disparamos y leemos el ADC de forma síncrona para simplificar el flujo hoy
        ADC1->CR |= ADC_CR_ADSTART;
        while ((ADC1->ISR & ADC_ISR_EOC) == 0) {}
        valor_adc = ADC1->DR; // Guardamos el valor neto (0 a 4095)
        // --- TU CÓDIGO BARE-METAL AQUÍ ---

        TIM3->CCR1 = valor_adc;
        // =================================================================

        Serial.print("ADC: "); Serial.print(valor_adc);
        Serial.print(" | PWM Duty Cycle asignado a CCR1: "); Serial.println(TIM3->CCR1);

        HAL_Delay(100); // Pequeña pausa para no saturar la pantalla de datos
    }
}
