#include "stm32f10x.h"

#define Motor_AIN1_GPIO_Pin    GPIO_Pin_4
#define Motor_AIN2_GPIO_Pin    GPIO_Pin_5
#define Motor_AIN1_GPIO_PORT   GPIOB
#define Motor_AIN2_GPIO_PORT   GPIOB

#define Motor_BIN1_GPIO_Pin    GPIO_Pin_4
#define Motor_BIN2_GPIO_Pin    GPIO_Pin_5
#define Motor_BIN1_GPIO_PORT   GPIOA
#define Motor_BIN2_GPIO_PORT   GPIOA

//��ʱ��2ͨ��3
#define MOTOR_PWMA_GPIO_PIN    GPIO_Pin_2
#define MOTOR_PWMA_GPIO_PORT   GPIOA

//��ʱ��2ͨ��4
#define MOTOR_PWMB_GPIO_PIN    GPIO_Pin_3
#define MOTOR_PWMB_GPIO_PORT   GPIOA

#define PS2_SPEEDPULSE_ZERO 1000
#define PS2_SPEEDPULSE_MAX 1600
#define AUTO_SPEED_MAX 800
#define PS2_SPEED_MAX 500
#define TIRE_SPACE 200//��̥��ȷ������
#define WHEELBASE 185//���

extern int16_t _sMotorA_Speed_M;
extern int16_t _sMotorB_Speed_M;


void motor_init(void);
void motor_test(void);

void motorA_Forward(void);//  ���A��ת
void motorA_Backward(void);// ���A��ת
void motorB_Forward(void);//  ���B��ת
void motorB_Backward(void);// ���B��ת

void motorASetSpeed(int8_t _direction,int16_t _speed); //���A��ת��ռ�ձȵ��ں���
void motorBSetSpeed(int8_t _direction,int16_t _speed); //���B��ת��ռ�ձȵ��ں���
void MotorControl(uint8_t _cAngularDirection, uint16_t _cAngularSpeed, uint8_t _cMotionDirection, uint16_t _sMotionSpeed);
void motor_one(void);
void motor_two(uint16_t key_temp);
void PID_SetPWM(int16_t _sMotorA, int16_t _sMotorB);
int motor_pwm_max_min(int motor_pwm);//����ʹ��PWM�������һ����Χֵ,������С�����ֵ������






