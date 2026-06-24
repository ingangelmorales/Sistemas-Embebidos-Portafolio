typedef struct {
    volatile unsigned int MODER;
    volatile unsigned int IDR;
    volatile unsigned int ODR;
} GPIO_Registers_t;

void setup() {
    Serial.begin(115200);
    delay(1000);

    GPIO_Registers_t Puerto_A_Virtual;
    GPIO_Registers_t *GPIO_A = &Puerto_A_Virtual;

    // CONFIGURACIÓN INICIAL (Simulación de registros del chip)
    // El Pin 0 será ENTRADA (Botón). El Pin 5 será SALIDA (LED indicador).
    GPIO_A->MODER = 0b00000000000000000000010000000000; // Pin 5 como salida, Pin 0 como entrada
    GPIO_A->ODR = 0; // Iniciamos con el LED apagado

    int contador_piezas = 0;
    int estado_anterior_boton = 0;

    Serial.println("--- DÍA 3: FILTRO DE ENTRADA INDUSTRIAL (DEBOUNCE) ---");

    // SIMULACIÓN DE UN CICLO DE TRABAJO (Imagina que esto corre miles de veces por segundo)
    for (int ciclo = 1; ciclo <= 5; ciclo++) {
        Serial.print("\n[Ciclo "); Serial.print(ciclo); Serial.println("]");

        // SIMULACIÓN DE ENTRADAS: En el ciclo 2 y 4 ocurre ruido eléctrico. 
        // En el ciclo 3 el operador presiona el botón de verdad de forma estable.
        if (ciclo == 2) GPIO_A->IDR = 0b00000001; // Ruido: El botón se activa un instante
        if (ciclo == 3) GPIO_A->IDR = 0b00000001; // Presión Real: El botón se mantiene activado
        if (ciclo == 4) GPIO_A->IDR = 0b00000000; // El botón se suelta
        if (ciclo == 5) GPIO_A->IDR = 0b00000000;

        // =================================================================
        // TU RETO PARTE 1: LECTURA ATÓMICA CON MÁSCARA AND
        // Necesitas aislar el bit 0 del registro IDR usando el operador '&'.
        // Recuerda: (GPIO_A->IDR & (1 << 0)) te dirá si el pin 0 está en 1.
        // =================================================================
        int boton_presionado = 0; 
        
        // Escribe aquí abajo el 'if' que verifique si el Pin 0 del IDR está activo.
        // Si está activo, asigna un 1 a la variable 'boton_presionado'.
        // PISTA INDUSTRIAL: El resultado de (GPIO_A->IDR & (1 << 0)) no es '1', es el valor de la posición del bit (en este caso 1, pero si fuera el pin 2 sería 4). Por eso evalúas si es diferente de cero (!= 0).
        
        // --- TU CÓDIGO AQUÍ PARA PARTE 1 ---

        if(GPIO_A->IDR & ((1 << 0)) != 0)
        {
          boton_presionado = 1;
        }
        // ------------------------------------

        // =================================================================
        // TU RETO PARTE 2: EL ALGORITMO ANTIREBOTE (DEBOUNCE)
        // Implementa la lógica de confirmación.
        // Si detectas que 'boton_presionado' es 1, y antes estaba en 0 (flanco de subida):
        // 1. Espera 20 milisegundos simulados (usa delay(20);).
        // 2. Vuelve a simular que lees el hardware para confirmar si sigue presionado.
        //    (Para simular esto en el Ciclo 2, cambia manualmente GPIO_A->IDR = 0 para simular que el ruido desapareció).
        //    (Para el Ciclo 3, déjalo en 1 porque la presión es real).
        // 3. Vuelve a evaluar con tu máscara AND. Si sigue presionado, incrementa 'contador_piezas' y enciende el LED (Pin 5 del ODR usando |=).
        // =================================================================
        
        // --- TU CÓDIGO AQUÍ PARA PARTE 2 ---

        if(boton_presionado==1 && estado_anterior_boton==0)
        {
          delay(20);
          if (ciclo == 2) 
          {
            GPIO_A->IDR = 0b00000000;
          }
          if(GPIO_A->IDR & (1 << 0) != 0)
          {
            contador_piezas++;
            GPIO_A->ODR |= (1<<5);
          }
          else
          {
            boton_presionado=0;
          }
        }
        else
        {
          GPIO_A->ODR &= ~(1<<5);
        }
        // ------------------------------------

        // Guardamos el estado para el siguiente ciclo
        estado_anterior_boton = boton_presionado;
        
        Serial.print("Contador de Piezas Real: "); Serial.println(contador_piezas);
        Serial.print("Estado del LED Indicador (ODR): "); Serial.println(GPIO_A->ODR, BIN);
    }
}

void loop() {}
