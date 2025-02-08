#include "stm32f10x.h"                  // Device header
#include "CountSensor.h"
#include "delay.h"

//时钟开启（GPIOx，AFIO）GPIO口的初始化，选择作为外部中断的gpio引脚，EXTI的初始化，NVIC的初始化
void CountSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitTypeStruture;
	GPIO_InitTypeStruture.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitTypeStruture.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitTypeStruture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitTypeStruture);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
	
	EXTI_InitTypeDef EXTI_InitTypeStruture;
	EXTI_InitTypeStruture.EXTI_Line=EXTI_Line12;
	EXTI_InitTypeStruture.EXTI_LineCmd=ENABLE;
	EXTI_InitTypeStruture.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitTypeStruture.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitTypeStruture);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitTypeStruture;
	NVIC_InitTypeStruture.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitTypeStruture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeStruture.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitTypeStruture.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitTypeStruture);
}



