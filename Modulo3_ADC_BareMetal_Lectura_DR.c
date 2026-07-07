#include <stm32c0xx_hal.h> // Heredamos las estructuras oficiales del fabricante

// Variable global para almacenar el valor de voltaje traducido
uint32_t valor_analogico = 0;

void ADC_Config(void) {
    // 1. Energizar el reloj del ADC y del Puerto A
    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // 2. Configurar el Pin PA0 en modo Analógico usando la HAL
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG; // Desconecta la lógica digital del pin
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. Configurar el LED integrado de la tarjeta (PA5) como salida
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 4. Inicializar los parámetros del módulo ADC por velocidad de desarrollo
    ADC_HandleTypeDef hadc = {0};
    hadc.Instance = ADC1;
    hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc.Init.Resolution = ADC_RESOLUTION_12B; // Resolución de 12 bits (0 a 4095)
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
    HAL_ADC_Init(&hadc);

    // Seleccionamos el canal 0 (asociado al pin PA0)
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    HAL_ADC_ConfigChannel(&hadc, &sConfig);
}

int main(void) {
    HAL_Init();
    ADC_Config();

    // Inicializamos la terminal nativa de Wokwi para ver el comportamiento
    Serial.begin(115200);
    delay(1000);

    Serial.println("--- DÍA 9: ADQUISICIÓN DE DATOS ANALÓGICOS (ADC) ---");

    while (1) {
        ADC1->CR |= ADC_CR_ADSTART;

        while ((ADC1->ISR & ADC_ISR_EOC) == 0) {
            // Espera segura por Hardware en segundo plano
        }
        valor_analogico = ADC1->DR;
        Serial.print("Lectura del Sensor (0-4095): ");
        Serial.println(valor_analogico);

        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(valor_analogico + 50); // Sumamos 50ms para evitar delays en cero
    }
}
