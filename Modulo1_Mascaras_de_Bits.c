// Definimos la estructura idéntica a la de ayer
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

    // El pin 4 (valor 16) y el pin 7 (valor 128) están activos. 
    GPIO_A->ODR = 0b10010000; // Estado inicial: pines 4 y 7 encendidos

    Serial.println("--- DÍA 2: MÁSCARAS DE BITS INDUSTRIALES ---");
    Serial.print("Estado Inicial del Puerto (Binario): ");
    Serial.println(GPIO_A->ODR, BIN);

    // =================================================================
    // PASO 1: ENCENDER EL PIN 2 SIN APAGAR EL 4 NI EL 7
    // Desplazamos un '1' dos posiciones a la izquierda y usamos el operador OR (|=)
    // =================================================================
    GPIO_A->ODR |= (1 << 2); 

    Serial.print("Paso 1 - Pin 2 Encendido (Debe conservar 4 y 7): ");
    Serial.println(GPIO_A->ODR, BIN);

    // Instrucciones: Usa el operador AND junto con NOT (&= ~) 
    // desplazando el número 1 hacia la posición del pin 7.
    // =================================================================
    
    GPIO_A->ODR &= ~(1 << 7); 

    // =================================================================
    
    Serial.print("Paso 2 - Pin 7 Apagado (Pines 4 y 2 deben seguir vivos): ");
    Serial.println(GPIO_A->ODR, BIN);
}

void loop() {}
