#include "include.h"



//PA2-A�ֵ�pwm PA3B�ֵ�pwm
void PWM_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Struture;
	GPIO_Struture.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Struture.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Struture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Struture);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruture;
	TIM_TimeBaseInitStruture.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruture.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruture.TIM_Period=2000;
	TIM_TimeBaseInitStruture.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStruture.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruture);
	
	TIM_OCInitTypeDef TIM_OCInitStruture;
	TIM_OCStructInit(&TIM_OCInitStruture);    //���ṹ�帳��ʼֵ
	TIM_OCInitStruture.TIM_OCMode=TIM_OCMode_PWM1;  //����Ƚϵ�ģʽ
	TIM_OCInitStruture.TIM_OCPolarity=TIM_OCPolarity_High; // ����ļ���
	TIM_OCInitStruture.TIM_OutputState=TIM_OutputState_Enable;   //�������ʹ��
	TIM_OCInitStruture.TIM_Pulse=0;    //����CCR
	TIM_OC3Init(TIM2,&TIM_OCInitStruture);
	TIM_OC4Init(TIM2,&TIM_OCInitStruture);
	TIM_Cmd(TIM2,ENABLE);
}
void SET_PWMA(uint16_t num)//����ռ�ձ�
{
	TIM_SetCompare3(TIM2,num);
}
void SET_PWMB(uint16_t num)//����ռ�ձ�
{
	TIM_SetCompare4(TIM2,num);
}
