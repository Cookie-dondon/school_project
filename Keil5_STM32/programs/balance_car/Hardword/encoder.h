#include "stm32f10x.h" 



//A��Ϊ������ʱ��B��Ϊ�ߵ�ƽ->��ת
//A��Ϊ������ʱ��B��Ϊ�͵�ƽ->��ת

//��PA6 PA7��Ϊ�½��ز���Ķ�ʱ������
//��PB0 PB1 ��Ϊ��ѯ�½���ʱ��һ�����ŵĵ�ƽ״̬��


void Encoder_Init_TIM4(uint32_t ARR,uint16_t PSC );
void Encoder_Init_TIM3(uint32_t ARR,uint16_t PSC );
int16_t Encoder_Get(int8_t TIMX);

