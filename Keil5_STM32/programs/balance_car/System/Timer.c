#include "Timer.h"


void Timer_int(void)//��������һ��50ms�Ķ�ʱ����ÿ��50ms�Ͷ�ȡһ�±������ļ���ֵ �Ӷ�ͨ���ж϶�ȡ�ٶ�
{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
//	TIM_InternalClockConfig(TIM4);
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruture;
//	TIM_TimeBaseInitStruture.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInitStruture.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruture.TIM_Period=500-1;
//	TIM_TimeBaseInitStruture.TIM_Prescaler=7200-1;//��������  �����ǸĻ�����
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
