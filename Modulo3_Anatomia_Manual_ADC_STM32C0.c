#include <stdint.h>

// ============================================================================
// MAPA DE MEMORIA MANUAL DEL ADC PARA LA FAMILIA STM32C0 (Bare-Metal Puro)
// ============================================================================
typedef struct {
    volatile uint32_t ISR;       // Offset: 0x00 
    volatile uint32_t Reserv;    // Offset: 0x04 
    volatile uint32_t CR;        // Offset: 0x08 
    volatile uint32_t CFGR1;     // Offset: 0x0C
    volatile uint32_t Reserva[6]; 
    volatile uint32_t CHSELR;
    volatile uint32_t Res[5];    
    volatile uint32_t DR;        // Offset: 0x40 
} ADC_Registers_t;

// ENLACE A LA MEMORIA FÍSICA DEL SILICIO (Dirección Base del Chip STM32C0)
#define ADC_BASE_ADDRESS    (0x40012400UL)
// Forzamos el puntero para que herede la forma de nuestro struct
#define ADC1                ((ADC_Registers_t *) ADC_BASE_ADDRESS) 
