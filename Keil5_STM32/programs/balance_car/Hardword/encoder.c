#include "include.h"
/*
	��������ʼ��  �ɵģ�
				//��PA6 PA7��Ϊ�½��ز���Ķ�ʱ������
				//��PB0 PB1 ��Ϊ��ѯ�½���ʱ��һ�����ŵĵ�ƽ״̬��
				PA6-PB0   A�ֵ�C2-C1
				PA7-PB1   B�ֵ�C1-C2
				//A��Ϊ������ʱ��B��Ϊ�ߵ�ƽ->��ת
				//A��Ϊ������ʱ��B��Ϊ�͵�ƽ->��ת
	
	
	
	��PA6-TIM3_CH1 PA7-TIM3_CH2      ��Ϊһ�����ӵı�������ȡ   CC1-CC2
	  PB6��TIM4_CH1 PB7��TIM4_CH2    ��Ϊ��һ�����ӵı�������ȡ CC2-CC1
*/

void Encoder_Init_TIM3(uint32_t ARR,uint16_t PSC )  //�����ʼ���˹��ڱ��������������������
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = ARR;		
	TIM_TimeBaseInitStructure.TIM_Prescaler =PSC;		
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3, ENABLE);
}
void Encoder_Init_TIM4(uint32_t ARR,uint16_t PSC )  //�����ʼ���˹��ڱ��������������������
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = ARR;		
	TIM_TimeBaseInitStructure.TIM_Prescaler =PSC;		
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM4, ENABLE);
}

int16_t Encoder_Get(int8_t TIMX)
{
	int16_t Temp;
	switch(TIMX)
	{
		case 4: Temp=TIM_GetCounter(TIM4),TIM_SetCounter(TIM4,0);break;
		case 3: Temp=TIM_GetCounter(TIM3),TIM_SetCounter(TIM3,0);break;
		default: Temp=0;
	}
//	if(Temp>1000)
//	{
//		Temp = Temp-65535;
//	}
	return  Temp;
}
