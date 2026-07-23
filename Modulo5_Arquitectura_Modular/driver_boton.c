#include <stm32c0xx_hal.h> 
#include "driver_boton.h"

void Driver_Boton_Init(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIOA->MODER &= ~(3 << 2);

	GPIOA->PUPDR &= ~(3 << 2); 
	GPIOA->PUPDR |= (1 << 3);
}


uint8_t Driver_Boton_Leer(void){
	static uint8_t c_rebote = 0;

	if((GPIOA->IDR & (1<<1))!=0)
	{
		c_rebote ++;
		if(c_rebote >= 20)
		{
			return 1;
		}
	}
	else{
		c_rebote = 0;
		return 0;
	}
}
