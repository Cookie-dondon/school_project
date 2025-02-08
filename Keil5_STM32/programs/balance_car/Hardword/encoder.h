#include "stm32f10x.h" 



//A项为上升沿时，B项为高电平->正转
//A项为上升沿时，B项为低电平->反转

//用PA6 PA7作为下降沿捕获的定时器引脚
//用PB0 PB1 作为查询下降沿时另一个引脚的电平状态。


void Encoder_Init_TIM4(uint32_t ARR,uint16_t PSC );
void Encoder_Init_TIM3(uint32_t ARR,uint16_t PSC );
int16_t Encoder_Get(int8_t TIMX);

