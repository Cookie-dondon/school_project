#include "stm32f10x.h"                  // Device header


#define key_left  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) 
#define key_right GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)
#define key_below  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)
#define key_above    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)

void key_Init(void);
uint16_t key_read(void);
uint16_t key_return(void);
