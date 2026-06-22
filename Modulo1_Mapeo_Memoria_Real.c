typedef struct {
    volatile unsigned int MODER;   // Dirección Base + 0x00
    volatile unsigned int IDR;     // Dirección Base + 0x10
    volatile unsigned int ODR;     // Dirección Base + 0x14
} GPIO_Registers_t;

#define GPIOA_BASE_ADDRESS  (0x40020000UL) 
#define GPIO_A  ((GPIO_Registers_t *) GPIOA_BASE_ADDRESS)

int main(void) {
   
    GPIO_A->MODER = 1;  // Los transistores físicos cambian a modo SALIDA
    GPIO_A->ODR = 0;    // El pin físico escupe 0 Voltios

    GPIO_A->ODR = 1;    // ¡El pin físico cambia instantáneamente a 3.3 Voltios!

    while(1) {
        // El código principal corre aquí para siempre
    }
}
