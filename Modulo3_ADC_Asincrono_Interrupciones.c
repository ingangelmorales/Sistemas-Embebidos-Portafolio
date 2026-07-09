#include <stm32c0xx_hal.h> // Heredamos los nombres de los registros del fabricante

// Variables globales para la comunicación entre la interrupción y el programa principal
volatile uint32_t nuevo_voltaje = 0;
volatile uint8_t dato_listo_bandera = 0;

void ADC_Config_Interrupt(void) {
    // 1. Energizar relojes del ADC y Puerto A
    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // 2. Configurar Pin PA0 como Analógico
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. Configurar el LED de la tarjeta (PA5) como salida
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 4. Inicializar parámetros del módulo ADC por velocidad
    ADC_HandleTypeDef hadc = {0};
    hadc.Instance = ADC1;
    hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc.Init.Resolution = ADC_RESOLUTION_12B;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
    HAL_ADC_Init(&hadc);

    // Seleccionamos el canal 0 (PA0)
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    HAL_ADC_ConfigChannel(&hadc, &sConfig);

    ADC1->IER |= ADC_IER_EOCIE;

    // 5. Habilitar el canal de interrupciones del ADC en el controlador central ARM (NVIC)
    HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
}

int main(void) {
    HAL_Init();
    ADC_Config_Interrupt();

    Serial.begin(115200);
    delay(1000);

    Serial.println("--- DÍA 11: ADQUISICIÓN ASÍNCRONA POR INTERRUPCIONES ---");

    while (1) {
        // DISPARO AL METAL: Ordenamos al ADC que inicie una conversión
        ADC1->CR |= ADC_CR_ADSTART;

        // Revisamos si la interrupción nos dejó un dato nuevo listo en segundo plano
        if (dato_listo_bandera == 1) {
            // Limpiamos nuestra bandera de software
            dato_listo_bandera = 0;

            Serial.print("Voltaje procesado de forma asíncrona: ");
            Serial.println(nuevo_voltaje);

            // Modulamos la velocidad del LED
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        }

        // Simulación de una tarea pesada en el ciclo principal
        HAL_Delay(nuevo_voltaje + 50); 
    }
}

// ============================================================================
// PARTE 2: LA RUTINA DE SERVICIO DE INTERRUPCIÓN REAL (ISR)
// El microcontrolador saltará aquí automáticamente gracias al hardware 
// en el microsegundo exacto en el que el ADC termine de medir.
// ============================================================================
extern "C" void ADC1_IRQHandler(void) {
    
    // =================================================================
    // TU DESAFÍO DEL DÍA 11: LA LÓGICA DE CONTROL DE LA INTERRUPCIÓN
    // 
    // TU META: Escribe las líneas necesarias adentro de esta función para:
    // 1. Verificar si la bandera física de conversión terminada (EOC) está activa en el registro de estado (ADC1->ISR & ADC_ISR_EOC).
    // 2. Si está activa, lee el registro de datos Bare-Metal (ADC1->DR) y asigna ese valor a nuestra variable global 'nuevo_voltaje'.
    // 3. Activa nuestra bandera de software 'dato_listo_bandera = 1;' para avisarle al main que procesamos la información.
    // 4. CRÍTICO EN LA INDUSTRIA: Limpia la bandera de hardware escribiendo un '1' en el bit ADC_ISR_EOC del registro de estado (ADC1->ISR = ADC_ISR_EOC;) para apagar la alarma física del silicio.
    // =================================================================
    
    // --- TU CÓDIGO BARE-METAL AQUÍ ---

    if((ADC1->ISR & ADC_ISR_EOC) != 0 )
    {
       nuevo_voltaje = ADC1->DR;
       dato_listo_bandera = 1;
       ADC1->ISR = ADC_ISR_EOC;
    }
}
