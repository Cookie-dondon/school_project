#include "include.h"


void PID_SetPWM(int16_t _sMotorA, int16_t _sMotorB);



//���ų�ʼ��
void motor_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //��PB4�ض������ͨIO��
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

void motor_test(void) //������ԣ�����
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
A�����ת
����Pin_0Ϊ�ߵ�ƽ ��Pin_1Ϊ�͵�ƽ ��ʱΪ��ת
********************************** */
void motorA_Forward(void)
{
	GPIO_WriteBit(Motor_AIN1_GPIO_PORT,Motor_AIN1_GPIO_Pin,Bit_SET);
	GPIO_WriteBit(Motor_AIN2_GPIO_PORT,Motor_AIN2_GPIO_Pin,Bit_RESET);
}
/* **********************************
motorA_Backward
A�����ת
����Pin_0Ϊ�͵�ƽ ��Pin_1Ϊ�ߵ�ƽ ��ʱΪ��ת
********************************** */
void motorA_Backward(void)
{
	GPIO_WriteBit(Motor_AIN1_GPIO_PORT,Motor_AIN1_GPIO_Pin,Bit_RESET);
	GPIO_WriteBit(Motor_AIN2_GPIO_PORT,Motor_AIN2_GPIO_Pin,Bit_SET);
}
/* **********************************
motorB_Forward
B�����ת
����Pin_4Ϊ�ߵ�ƽ ��Pin_5Ϊ�͵�ƽ ��ʱΪ��ת
********************************** */
void motorB_Forward(void)
{
	GPIO_WriteBit(Motor_BIN1_GPIO_PORT,Motor_BIN1_GPIO_Pin,Bit_SET);
	GPIO_WriteBit(Motor_BIN2_GPIO_PORT,Motor_BIN2_GPIO_Pin,Bit_RESET);
}
/* **********************************
motorB_Backward
B�����ת
����Pin_4Ϊ�͵�ƽ ��Pin_5Ϊ�ߵ�ƽ ��ʱΪ��ת
********************************** */
void motorB_Backward(void)
{
	GPIO_WriteBit(Motor_BIN1_GPIO_PORT,Motor_BIN1_GPIO_Pin,Bit_RESET);
	GPIO_WriteBit(Motor_BIN2_GPIO_PORT,Motor_BIN2_GPIO_Pin,Bit_SET);
}
/* *********************************** 
��������motorASetSpeed
����˵����A�����ռ�ձ����ü��˶�����
����˵����_direction :�˶����� 00-��  01-��   _speed ��ռ�ձ� ��Χ 0~2000
����ֵ����
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
��������motorBSetSpeed
����˵����B�����ռ�ձ����ü��˶�����
����˵����_direction :�˶����� 00-��  01-��   _speed ��ռ�ձ� ��Χ 0~2000
����ֵ����
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
*@brief		:������õ�PWM��������
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
int motor_pwm_max_min(int motor_pwm)//����ʹ��PWM�������һ����Χֵ,������С�����ֵ������
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







