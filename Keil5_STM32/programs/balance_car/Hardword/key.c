#include "key.h"
#include "stm32f10x.h"                  // Device header


uint16_t key_value=0,key_down=0,key_up=0,key_old=0;//按键的专属变量

void key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//按键的
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//按键的
	GPIO_InitTypeDef GPIO_InitTypeStruture;
	
	GPIO_InitTypeStruture.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitTypeStruture.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitTypeStruture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitTypeStruture);
	
	GPIO_InitTypeStruture.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitTypeStruture);
}
uint16_t key_read(void)
{
	uint16_t key_temp=0;
	
	if(key_left  == Bit_SET)key_temp=1; // 左	
	if(key_right == Bit_SET)key_temp=2;//  右
	if(key_below == Bit_SET)key_temp=3;//  下
	if(key_above == Bit_SET)key_temp=4;//  上
	if(key_above == Bit_SET && key_below  == Bit_SET) key_temp =5;//进入调速模式
	if(key_left  == Bit_SET && key_right  == Bit_SET) key_temp =6;//进入普通模式
	return key_temp;
}
uint16_t key_return(void)
{
	key_value = key_read();
	key_down=key_value&(key_value ^ key_old);//下降沿
	key_up=~key_value&(key_value ^ key_old);//上升沿
	key_old=key_value;
	return key_down;
}


