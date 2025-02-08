#ifndef EXTEND_HEADFIEL__I2C__H_
#define EXTEND_HEADFIEL__I2C__H_
#include "stm32f10x.h"
#include "generic.h"

/*
bilibili 小努班 UID:437280309
@time:2024.12.1
@verson:V1_3
@updata: Fixed an issue when multiple I2C cannot be used.Using object oriented programming thinking,Hide a lot lof API function,softerware I2C have arrived 390K
修复了多个I2C无法使用的问题。使用面向对象程序设计思维.隐藏大量API函数接口。软件I2C速度已经达到390Khz
*/

/*硬件I2C初始化对应引脚for STM32F103
SCL1：PB6
SDA1：PB7

SCL2：PB10
SDA2：PB11
*/


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	#define SI2C_delay_time 0															//设置??件I2C的延时速度
	#define I2C(obj) (Pthis = &obj)																				//Pthis全局指针宏定??
	typedef struct I2C_Private I2C_Private;		
	typedef struct I2C_BUS
	{
		//严禁使用该指针! It is strictly forbidden to use this pointer
		I2C_Private* Private;

		///////////////////////User//////////////////////////////////                   					    //用户API函数接口
		uint8_t (*AckTest)();																					//响应接口,it can check that if our I2C bus is init succese
		void (*Write_Reg)(uint8_t RegAddress, uint8_t Data);													//写寄存器函数,write register by I2C bus
		uint8_t (*Read_Reg)(uint8_t RegAddress);																//读寄存器函数,read register by I2C bus
		void (*Rest_Speed)(uint32_t Speed);																		//硬件I2C重新设置速度,you can reset your Hardware I2C Speed
		
		uint8_t (*Write_Reg_continue)(uint8_t Device_Add,uint8_t RegAddress,uint8_t Count,uint8_t* Data);		//连续写寄存器函数,continue write register by I2C bus，为了适应移植
		uint8_t (*Read_Reg_continue)(uint8_t Device_Add,uint8_t RegAddress,uint8_t Count,uint8_t* Data);		//连续读寄存器函数,continue read register by I2C bus，为了适应移植
	}I2C_BUS;
	extern I2C_BUS* Pthis;
	I2C_BUS Create_SI2C(GPIO_TypeDef* GPIOx,uint16_t SCL,uint16_t SDA,uint8_t Address);							//创建软件I2C对象,create a softwere I2C
	I2C_BUS Create_HI2C(I2C_TypeDef* I2Cx,uint8_t Address);														//创建硬件I2C对象,create a hardware I2C

#ifdef __cplusplus
extern "C"
}
#endif /* __cplusplus */

#endif /* EXTEND_HEADFIEL__I2C__H_ */
