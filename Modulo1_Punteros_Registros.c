#include <Arduino.h> // Necesario solo para que Wokwi habilite el canal de la consola

// 1. Definimos la estructura que imita los registros de un puerto físico del chip
typedef struct {
    volatile unsigned int MODER;   // Registro de Modo (0 = Entrada, 1 = Salida)
    volatile unsigned int IDR;     // Registro de Datos de Entrada (Lee pines)
    volatile unsigned int ODR;     // Registro de Datos de Salida (Escribe en pines)
} GPIO_Registers_t;

// En Wokwi (entorno Arduino), usamos setup() en lugar de main() para iniciar el programa
void setup() {
    Serial.begin(115200); 
    delay(1000);          

    GPIO_Registers_t Puerto_A_Virtual;
    GPIO_Registers_t *GPIO_A = &Puerto_A_Virtual;

    GPIO_A->MODER = 1; 
    GPIO_A->ODR = 0;

    Serial.print("Hardware configurado con éxito. Estado actual del pin: ");
    Serial.println(GPIO_A->ODR);

    Serial.println("\n[Simulación] Se detecta una señal externa...");
    
    GPIO_A->ODR = 1;

    Serial.print("Estado final del pin controlado por el puntero: ");
    Serial.println(Puerto_A_Virtual.ODR);
}

void loop() {

}
