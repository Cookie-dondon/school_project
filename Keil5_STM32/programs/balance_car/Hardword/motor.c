#include "include.h"


void PID_SetPWM(int16_t _sMotorA, int16_t _sMotorB);



//引脚初始化
void motor_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //把PB4重定义成普通IO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStruture;
	
	GPIO_InitStruture.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruture.GPIO_Pin=Motor_AIN1_GPIO_Pin|Motor_AIN2_GPIO_Pin;
	GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(Motor_AIN1_GPIO_PORT,&GPIO_InitStruture);
	
	GPIO_InitStruture.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruture.GPIO_Pin=Motor_BIN1_GPIO_Pin|Motor_BIN2_GPIO_Pin;
	GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(Motor_BIN1_GPIO_PORT,&GPIO_InitStruture);
}

void motor_test(void) //电机测试，开环
{
		SET_PWMA(1000);
		SET_PWMB(1000);
		GPIO_WriteBit(Motor_AIN1_GPIO_PORT,Motor_AIN1_GPIO_Pin,Bit_SET);
		GPIO_WriteBit(Motor_AIN2_GPIO_PORT,Motor_AIN2_GPIO_Pin,Bit_RESET);
	
		GPIO_WriteBit(Motor_BIN1_GPIO_PORT,Motor_BIN1_GPIO_Pin,Bit_SET);
		GPIO_WriteBit(Motor_BIN2_GPIO_PORT,Motor_BIN2_GPIO_Pin,Bit_RESET);
}
/* **********************************
motorA_Forward
A电机正转
定义Pin_0为高电平 ，Pin_1为低电平 此时为正转
********************************** */
void motorA_Forward(void)
{
	GPIO_WriteBit(Motor_AIN1_GPIO_PORT,Motor_AIN1_GPIO_Pin,Bit_SET);
	GPIO_WriteBit(Motor_AIN2_GPIO_PORT,Motor_AIN2_GPIO_Pin,Bit_RESET);
}
/* **********************************
motorA_Backward
A电机反转
定义Pin_0为低电平 ，Pin_1为高电平 此时为反转
********************************** */
void motorA_Backward(void)
{
	GPIO_WriteBit(Motor_AIN1_GPIO_PORT,Motor_AIN1_GPIO_Pin,Bit_RESET);
	GPIO_WriteBit(Motor_AIN2_GPIO_PORT,Motor_AIN2_GPIO_Pin,Bit_SET);
}
/* **********************************
motorB_Forward
B电机正转
定义Pin_4为高电平 ，Pin_5为低电平 此时为正转
********************************** */
void motorB_Forward(void)
{
	GPIO_WriteBit(Motor_BIN1_GPIO_PORT,Motor_BIN1_GPIO_Pin,Bit_SET);
	GPIO_WriteBit(Motor_BIN2_GPIO_PORT,Motor_BIN2_GPIO_Pin,Bit_RESET);
}
/* **********************************
motorB_Backward
B电机反转
定义Pin_4为低电平 ，Pin_5为高电平 此时为反转
********************************** */
void motorB_Backward(void)
{
	GPIO_WriteBit(Motor_BIN1_GPIO_PORT,Motor_BIN1_GPIO_Pin,Bit_RESET);
	GPIO_WriteBit(Motor_BIN2_GPIO_PORT,Motor_BIN2_GPIO_Pin,Bit_SET);
}
/* *********************************** 
函数名：motorASetSpeed
功能说明：A电机的占空比设置及运动方向
参数说明：_direction :运动方向 00-正  01-反   _speed ：占空比 范围 0~2000
返回值：无
************************************** */
void motorASetSpeed(int8_t _direction,int16_t _speed)
{
	if(_direction==0)
	{
		motorA_Forward();
		SET_PWMA((_speed));
		
	}else if(_direction==1)
	{
		motorA_Backward();
		SET_PWMA((_speed));
	}
}
/* *********************************** 
函数名：motorBSetSpeed
功能说明：B电机的占空比设置及运动方向
参数说明：_direction :运动方向 00-正  01-反   _speed ：占空比 范围 0~2000
返回值：无
************************************** */
void motorBSetSpeed(int8_t _direction,int16_t _speed)
{
	if(_direction==0)
	{
		motorB_Forward();
		SET_PWMB((_speed));
		
	}else if(_direction==1)
	{
		motorB_Backward();
		SET_PWMB((_speed));
	}
}
/***********************************************************
*@fuction	:PID_SetPWM
*@brief		:将计算好的PWM输出给电机
*@param		:int16_t _sMotorX
*@return	:void
*@author	:Hades_Czq
*@date		:2023-07-07
***********************************************************/

void PID_SetPWM(int16_t _sMotorA, int16_t _sMotorB)
{
    if(_sMotorA > 0)
    {
        motorASetSpeed(0, _sMotorA);
    }
    if(_sMotorA <= 0)
    {
        motorASetSpeed(1, -1 * _sMotorA);
    }

    if(_sMotorB > 0)
    {
        motorBSetSpeed(0, _sMotorB);
    }
    if(_sMotorB <= 0)
    {
        motorBSetSpeed(1, -1 * _sMotorB);
    }

}
int motor_pwm_max_min(int motor_pwm)//用来使得PWM的输出有一个范围值,就是最小跟最大值的限制
{
	if(motor_pwm>0)
	{
		if(motor_pwm>1600)motor_pwm=1600;
	}
	else
	{
		if(motor_pwm<-1600)motor_pwm=-1600;
	}
	return motor_pwm;
}







