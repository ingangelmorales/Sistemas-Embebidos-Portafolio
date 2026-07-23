#include <stm32c0xx_hal.h>
#include "driver_analogico.h"
#include "fsm_control.h"
#include "driver_boton.h"

uint32_t lectura_sensor = 0;

int main(void) {
    HAL_Init();
    Driver_Sistemas_Init(); // Bare-metal Init (Relojes, ADC, PWM)
    FSM_Init();  
    Driver_Boton_Init();           // FSM Init (Estado inicial)
    
    Serial.begin(115200);
    delay(1000);
    Serial.println("--- SEMANA 5: ARQUITECTURA DE FIRMWARE MODULAR ---");

    while (1) {
        lectura_sensor = Driver_Leer_Voltaje();
        
        FSM_Actualizar(lectura_sensor);

        Serial.print("Dato integrado - ADC: ");
        Serial.print(lectura_sensor);
        Serial.print(" | PWM: ");
        Serial.println(TIM3->CCR1); // Verificamos directo en el registro del Timer

        HAL_Delay(100); 
    }
}
