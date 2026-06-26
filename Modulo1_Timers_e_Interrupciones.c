typedef struct {
    volatile unsigned int MODER;
    volatile unsigned int ODR;
} GPIO_Registers_t;

// Estructura que simula los registros de un TIMER de Hardware real
typedef struct {
    volatile unsigned int CR1;    // Control Register 1 (Para encender/apagar el timer)
    volatile unsigned int ARR;    // Auto-Reload Register (El valor límite hasta donde cuenta)
    volatile unsigned int CNT;    // Counter Register (El valor que va contando en tiempo real)
    volatile unsigned int SR;     // Status Register (Indica con un bit si ya terminó de contar)
} TIMER_Registers_t;

void setup() {
    Serial.begin(115200);
    delay(1000);

    GPIO_Registers_t Puerto_A_Virtual;
    GPIO_Registers_t *GPIO_A = &Puerto_A_Virtual;
    
    TIMER_Registers_t Timer_2_Virtual;
    TIMER_Registers_t *TIM2 = &Timer_2_Virtual;

    // CONFIGURACIÓN DE HARDWARE
    GPIO_A->MODER |= (1 << 5); // Pin 5 como Salida (LED indicador)
    GPIO_A->ODR = 0;           // Iniciamos con LED apagado

    // El Timer está apagado inicialmente (CR1 = 0)
    TIM2->CR1 = 0;
    TIM2->CNT = 0;
    TIM2->SR = 0;

    Serial.println("--- DÍA 4: TEMPORIZADORES E INTERRUPCIONES DE HARDWARE ---");

    TIM2->ARR = 3; 
    TIM2->CR1 = 1; // El hardware empieza a contar de forma independiente al CPU

    for (int ciclo_cpu = 1; ciclo_cpu <= 6; ciclo_cpu++) {
        Serial.print("\n[CPU ejecutando tarea principal - Ciclo "); Serial.print(ciclo_cpu); Serial.println("]");
        
        // El hardware del Timer incrementa su contador en paralelo en cada ciclo
        if (TIM2->CR1 == 1) {
            TIM2->CNT++;
            Serial.print("  [Hardware TIM2] Contador interno incrementado a: "); Serial.println(TIM2->CNT);
            
            // Si el contador llega al límite (ARR), el hardware levanta la bandera de interrupción en el Status Register (SR = 1)
            if (TIM2->CNT >= TIM2->ARR) {
                TIM2->SR = 1; // Bandera de interrupción activa
                TIM2->CNT = 0; // El timer se reinicia automáticamente
            }
        }

        if(TIM2->SR == 1)
        {
          Serial.print(" ¡INTERRUPCIÓN DE HARDWARE DETECTADA! ");
          GPIO_A->ODR ^= (1<<5);
          TIM2->SR = 0;
        }

        // ----------------------
        
        Serial.print("  Estado del LED (ODR): "); Serial.println(GPIO_A->ODR, BIN);
    }
}

void loop() {}
