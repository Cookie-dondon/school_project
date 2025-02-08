#include "Timer.h"


void Timer_int(void)//这里用了一个50ms的定时器，每隔50ms就读取一下编码器的计数值 从而通过中断读取速度
{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
//	TIM_InternalClockConfig(TIM4);
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruture;
//	TIM_TimeBaseInitStruture.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInitStruture.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruture.TIM_Period=500-1;
//	TIM_TimeBaseInitStruture.TIM_Prescaler=7200-1;//改了这里  ，但是改回来了
////	TIM_TimeBaseInitStruture.TIM_Period=50-1;
////	TIM_TimeBaseInitStruture.TIM_Prescaler=7200-1;
//	TIM_TimeBaseInitStruture.TIM_RepetitionCounter=0;
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruture);
//	
//	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
//	NVIC_InitTypeDef NVIC_InitTypeStruture;
//	NVIC_InitTypeStruture.NVIC_IRQChannel=TIM4_IRQn;
//	NVIC_InitTypeStruture.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitTypeStruture.NVIC_IRQChannelPreemptionPriority=2;
//	NVIC_InitTypeStruture.NVIC_IRQChannelSubPriority=1;
//	NVIC_Init(&NVIC_InitTypeStruture);
//	TIM_Cmd(TIM4,ENABLE);
}
