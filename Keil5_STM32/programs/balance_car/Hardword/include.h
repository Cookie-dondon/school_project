#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "key.h"
#include "CountSensor.h"
#include "Timer.h"
#include "motor.h"
#include "PWM.h"
#include "oled.h"
#include "encoder.h"
#include "uart.h"
#include "stdio.h"
#include <stdio.h>
#include <stdarg.h>
#include "generic.h"
#include "MPU6050.h"
#include "Serial.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "Kalman_filter.h"

extern float newroll;
extern float newpitch;


void All_init(void);



