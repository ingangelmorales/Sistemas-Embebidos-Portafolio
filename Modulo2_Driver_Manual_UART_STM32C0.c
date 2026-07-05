typedef struct{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t BRR;
	volatile uint32_t Reservado[3];
	volatile uint32_t ISR;
	volatile uint32_t Relleno;
	volatile uint32_t RDR;
	volatile uint32_t TDR;
} UART_Registers_t;

#define UART_BASE_ADDRESS (0x40013800)
#define UART1 ((UART_Registers_t *)(UART_BASE_ADDRESS))


UART1->CR1 |= (1<<0);
