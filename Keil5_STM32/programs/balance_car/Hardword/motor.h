#include "stm32f10x.h"

#define Motor_AIN1_GPIO_Pin    GPIO_Pin_4
#define Motor_AIN2_GPIO_Pin    GPIO_Pin_5
#define Motor_AIN1_GPIO_PORT   GPIOB
#define Motor_AIN2_GPIO_PORT   GPIOB

#define Motor_BIN1_GPIO_Pin    GPIO_Pin_4
#define Motor_BIN2_GPIO_Pin    GPIO_Pin_5
#define Motor_BIN1_GPIO_PORT   GPIOA
#define Motor_BIN2_GPIO_PORT   GPIOA

//定时器2通道3
#define MOTOR_PWMA_GPIO_PIN    GPIO_Pin_2
#define MOTOR_PWMA_GPIO_PORT   GPIOA

//定时器2通道4
#define MOTOR_PWMB_GPIO_PIN    GPIO_Pin_3
#define MOTOR_PWMB_GPIO_PORT   GPIOA

#define PS2_SPEEDPULSE_ZERO 1000
#define PS2_SPEEDPULSE_MAX 1600
#define AUTO_SPEED_MAX 800
#define PS2_SPEED_MAX 500
#define TIRE_SPACE 200//轮胎宽度方向距离
#define WHEELBASE 185//轴距

extern int16_t _sMotorA_Speed_M;
extern int16_t _sMotorB_Speed_M;


void motor_init(void);
void motor_test(void);

void motorA_Forward(void);//  电机A正转
void motorA_Backward(void);// 电机A反转
void motorB_Forward(void);//  电机B正转
void motorB_Backward(void);// 电机B反转

void motorASetSpeed(int8_t _direction,int16_t _speed); //电机A的转向、占空比调节函数
void motorBSetSpeed(int8_t _direction,int16_t _speed); //电机B的转向、占空比调节函数
void MotorControl(uint8_t _cAngularDirection, uint16_t _cAngularSpeed, uint8_t _cMotionDirection, uint16_t _sMotionSpeed);
void motor_one(void);
void motor_two(uint16_t key_temp);
void PID_SetPWM(int16_t _sMotorA, int16_t _sMotorB);
int motor_pwm_max_min(int motor_pwm);//用来使得PWM的输出有一个范围值,就是最小跟最大值的限制






