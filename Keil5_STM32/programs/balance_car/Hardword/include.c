#include "include.h"

void All_init(void)
{
	
	
	OLED_Init();
	OLED_Clear();
	motor_init();
	PWM_init();
	Encoder_Init_TIM3(65536-1,1-1);
	Encoder_Init_TIM4(65536-1,1-1);
	key_Init();
	MPU6050_CountSensor_Init();
	MPU6050_init(GPIOA,GPIO_Pin_11,GPIO_Pin_12);//SCL SDA
}

