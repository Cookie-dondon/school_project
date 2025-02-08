#include "_I2C_.h"
#include "stdlib.h"

I2C_BUS* Pthis = 0;//全局指针
typedef struct I2C_Private
{
	GPIO_TypeDef* GPIOx;
	I2C_TypeDef* I2Cx;
	uint16_t SCL;
	uint16_t SDA;
	uint16_t I2C_Add;
	uint8_t Hard_I2C_EN;
}I2C_Private;

void MyI2C_W_SCL(uint8_t BitValue){//SCL write data
	//GPIO_WriteBit(Pthis->Private->GPIOx, Pthis->Private->SCL, (BitAction)BitValue);
	if (BitValue != Bit_RESET)
    	Pthis->Private->GPIOx->BSRR = Pthis->Private->SCL;
  	else
   		Pthis->Private->GPIOx->BRR = Pthis->Private->SCL;
	Delay_us(SI2C_delay_time);
}

void MyI2C_W_SDA(uint8_t BitValue){//SDA write data
	//GPIO_WriteBit(Pthis->Private->GPIOx, Pthis->Private->SDA, (BitAction)BitValue);
	if (BitValue != Bit_RESET)
    	Pthis->Private->GPIOx->BSRR = Pthis->Private->SDA;
  	else
   		Pthis->Private->GPIOx->BRR = Pthis->Private->SDA;
	Delay_us(SI2C_delay_time);
}
//0�ɹ�
uint8_t MyI2C_R_SDA(){//SDA read data
	uint8_t BitValue = GPIO_ReadInputDataBit(Pthis->Private->GPIOx, Pthis->Private->SDA);
	Delay_us(SI2C_delay_time);
	return BitValue;
}
/////////////////////////////////////////////////I2C_Action////////////////////////////////////////////////
void SI2C_Start(){
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void SI2C_Stop(){
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void SI2C_WriteByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));// for example 1010 1100  -> 1010 1100 -> 1010 1100
		Delay_us(SI2C_delay_time);					//             |              |      		|
		MyI2C_W_SCL(1);
		Delay_us(SI2C_delay_time);
		MyI2C_W_SCL(0);
		Delay_us(SI2C_delay_time);
	}
}

uint8_t SI2C_ReceiveByte(){
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}
//0Ӧ��ACK , 1 is Nack
void SI2C_WriteAck(uint8_t AckBit){
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t SI2C_ReceiveAck(){//receive ask
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}

uint8_t SI2C_ACK_Test(){
	uint8_t Ack;
	SI2C_Start();
	SI2C_WriteByte(Pthis->Private->I2C_Add);
	Ack = SI2C_ReceiveAck();
	SI2C_Stop();
	return Ack;
}

/////////////////////////////////////////////�?件I2C//////////////////////////////////////////////////////////
void HI2C_Rest_Speed(uint32_t Speed){
	I2C_InitTypeDef I2C_InitStructure;
 	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
 	I2C_InitStructure.I2C_ClockSpeed = Speed;		
 	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
 	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
 	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
 	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
 	I2C_Init(Pthis->Private->I2Cx, &I2C_InitStructure);
}

void HI2C_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 1000;									//给定超时计数时间
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)	//�?�?等待指定事件
	{
		Timeout --;										//等待时，计数值自�?
		if (Timeout == 0)								//�?减到0后，等待超时
			break;										
	}
}

void I2C_Write_Reg(uint8_t RegAddress, uint8_t Data){
	
	if(Pthis->Private->Hard_I2C_EN){
		
		I2C_GenerateSTART(Pthis->Private->I2Cx, ENABLE);										
		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_MODE_SELECT);					
		
		I2C_Send7bitAddress(Pthis->Private->I2Cx, Pthis->Private->I2C_Add, I2C_Direction_Transmitter);	
		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	
		
		I2C_SendData(Pthis->Private->I2Cx, RegAddress);											
		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING);			
		
		I2C_SendData(Pthis->Private->I2Cx, Data);												
		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				
		
		I2C_GenerateSTOP(Pthis->Private->I2Cx, ENABLE);	
	}
	else{
		SI2C_Start();
      
		SI2C_WriteByte(Pthis->Private->I2C_Add);
		SI2C_ReceiveAck();
		
		SI2C_WriteByte(RegAddress);
		SI2C_ReceiveAck();

		SI2C_WriteByte(Data);
		SI2C_ReceiveAck();

		SI2C_Stop();
	}
}

uint8_t I2C_Read_Reg(uint8_t RegAddress){
	uint8_t Data;
	if(Pthis->Private->Hard_I2C_EN){
		I2C_GenerateSTART(Pthis->Private->I2Cx, ENABLE);									
		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_MODE_SELECT);					
		
		I2C_Send7bitAddress(Pthis->Private->I2Cx, Pthis->Private->I2C_Add, I2C_Direction_Transmitter);	
		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	
		
		I2C_SendData(Pthis->Private->I2Cx, RegAddress);											
		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				
		
		I2C_GenerateSTART(Pthis->Private->I2Cx, ENABLE);									
		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_MODE_SELECT);					
		
		I2C_Send7bitAddress(Pthis->Private->I2Cx, Pthis->Private->I2C_Add, I2C_Direction_Receiver);		
		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);		
		
		I2C_AcknowledgeConfig(Pthis->Private->I2Cx, DISABLE);									
		I2C_GenerateSTOP(Pthis->Private->I2Cx, ENABLE);											
		
		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED);				
		Data = I2C_ReceiveData(Pthis->Private->I2Cx);											
		
		I2C_AcknowledgeConfig(Pthis->Private->I2Cx, ENABLE);		

		return Data;
	}else{
		SI2C_Start();
		SI2C_WriteByte(Pthis->Private->I2C_Add);
		SI2C_ReceiveAck();
		SI2C_WriteByte(RegAddress);
		SI2C_ReceiveAck();

		SI2C_Start();
		SI2C_WriteByte(Pthis->Private->I2C_Add | 0x01);//|0x01将最低位�?1，代表�?�操�?
		SI2C_ReceiveAck();
		Data = SI2C_ReceiveByte();
		SI2C_WriteAck(1);//�?读一次非应答结束
		SI2C_Stop();

		return Data;
	}
}

// uint8_t I2C_Write_Reg_continue(uint8_t Device_Add,uint8_t RegAddress,uint8_t Count,uint8_t* Data){
// 	int i = 0;
// 	if(Pthis->Private->Hard_I2C_EN){
// 		I2C_GenerateSTART(Pthis->Private->I2Cx, ENABLE);										
// 		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_MODE_SELECT);					
		
// 		I2C_Send7bitAddress(Pthis->Private->I2Cx, Device_Add<<1, I2C_Direction_Transmitter);	
// 		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	
		
// 		I2C_SendData(Pthis->Private->I2Cx, RegAddress);											
// 		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING);			
// 		for(;i<Count;i++){
// 			I2C_SendData(Pthis->Private->I2Cx, Data[i]);												
// 			HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED);	
// 		}	
// 		I2C_GenerateSTOP(Pthis->Private->I2Cx, ENABLE);
// 		return 0;
// 	}
// 	else{
// 		SI2C_Start();
// 		SI2C_WriteByte((Device_Add<<1)|0);
// 		if(SI2C_ReceiveAck()){
// 			printf("I2C��ʼ����ַʧ��\n");
// 			SI2C_Stop();
// 			return 1;
// 		}
// 		SI2C_WriteByte(RegAddress);
// 		SI2C_ReceiveAck();
// 		for(;i<Count;i++){
// 			SI2C_WriteByte(Data[i]);
// 			if(SI2C_ReceiveAck()){
// 				printf("I2Cû�н���Ӧ��\n");
// 				SI2C_Stop();
// 				return 1;
// 			}
// 		}
// 		SI2C_Stop();
// 		return 0;
// 	}
// }

// uint8_t I2C_Read_Reg_continue(uint8_t Device_Add,uint8_t RegAddress,uint8_t Count,uint8_t* Data){
// 	int i = 0;
// 	if(Pthis->Private->Hard_I2C_EN){
// 		I2C_GenerateSTART(Pthis->Private->I2Cx, ENABLE);									
// 		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_MODE_SELECT);					
		
// 		I2C_Send7bitAddress(Pthis->Private->I2Cx, Device_Add<<1, I2C_Direction_Transmitter);	
// 		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	
		
// 		I2C_SendData(Pthis->Private->I2Cx, RegAddress);											
// 		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				
		
// 		I2C_GenerateSTART(Pthis->Private->I2Cx, ENABLE);									
// 		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_MODE_SELECT);					
		
// 		I2C_Send7bitAddress(Pthis->Private->I2Cx, Device_Add<<1, I2C_Direction_Receiver);		
// 		HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);		
		
// 		I2C_AcknowledgeConfig(Pthis->Private->I2Cx, DISABLE);									
// 		I2C_GenerateSTOP(Pthis->Private->I2Cx, ENABLE);											
// 		for(;i<Count;i++){
// 			HI2C_WaitEvent(Pthis->Private->I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED);				
// 			Data[i] = I2C_ReceiveData(Pthis->Private->I2Cx);											
// 		}
// 		I2C_AcknowledgeConfig(Pthis->Private->I2Cx, ENABLE);
// 		return 0;		
// 	}else{
// 		SI2C_Start();
// 		SI2C_WriteByte(Device_Add<<1);
// 		if(SI2C_ReceiveAck()){
// 			printf("I2C��ַ����\n");
// 			SI2C_Stop();
// 			return 1;
// 		}
// 		SI2C_WriteByte(RegAddress);
// 		SI2C_ReceiveAck();

// 		SI2C_Start();
// 		SI2C_WriteByte((Device_Add<<1) | 0x01);//|0x01将最低位�?1，代表�?�操�?
// 		SI2C_ReceiveAck();
// 		for(;i<Count;i++){
// 			Data[i] = SI2C_ReceiveByte();
// 			SI2C_WriteAck(0);
// 		}
// 		SI2C_WriteAck(1);
// 		SI2C_Stop();
// 		return 0;
// 	}
// }

/// @brief ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param GPIOx 
/// @param SCl 
/// @param SDA 
/// @return 
I2C_BUS Create_SI2C(GPIO_TypeDef* GPIOx,uint16_t SCL,uint16_t SDA,uint8_t Address){
	//创建一�?对象
	struct I2C_BUS this;
	this.Private = malloc(sizeof(I2C_Private));//为�?�有成员开辟内�?
	this.Private->I2C_Add = Address;
	this.Private->GPIOx = GPIOx;
	this.Private->SCL = SCL;
	this.Private->SDA = SDA;
	this.Private->I2Cx = 0;
	this.Private->Hard_I2C_EN = 0;

	this.AckTest = SI2C_ACK_Test;
	this.Write_Reg = I2C_Write_Reg;
	this.Read_Reg = I2C_Read_Reg;
	this.Rest_Speed = 0;
	// this.Read_Reg_continue = I2C_Read_Reg_continue;
	// this.Write_Reg_continue = I2C_Write_Reg_continue;
	this.Read_Reg_continue = 0;
	this.Write_Reg_continue = 0;

	if(GPIOx==GPIOA)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	else if(GPIOx==GPIOB)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	else if(GPIOx==GPIOC)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	else if(GPIOx==GPIOD)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	else if(GPIOx==GPIOE)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	else if(GPIOx==GPIOF)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	else if(GPIOx==GPIOG)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	
	GPIO_InitTypeDef GPIO_Init_Struct;
	GPIO_Init_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_Struct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init_Struct.GPIO_Pin = SCL|SDA;
	GPIO_Init(GPIOx,&GPIO_Init_Struct);

	GPIO_WriteBit(GPIOx, SCL, (BitAction)1);
	GPIO_WriteBit(GPIOx, SDA, (BitAction)1);

	return this;
}

I2C_BUS Create_HI2C(I2C_TypeDef* I2Cx,uint8_t Address){
	//创建一�?对象
	struct I2C_BUS this;
	this.Private = malloc(sizeof(I2C_Private));//为�?�有成员开辟内�?
	this.Private->I2C_Add = Address;
	this.Private->GPIOx = 0;
	this.Private->SCL = 0;
	this.Private->SDA = 0;
	this.Private->I2Cx = I2Cx;
	this.Private->Hard_I2C_EN = 1;

	this.AckTest = SI2C_ACK_Test;
	this.Write_Reg = I2C_Write_Reg;
	this.Read_Reg = I2C_Read_Reg;
	this.Rest_Speed = HI2C_Rest_Speed;
	// this.Read_Reg_continue = I2C_Read_Reg_continue;
	// this.Write_Reg_continue = I2C_Write_Reg_continue;
	this.Read_Reg_continue = 0;
	this.Write_Reg_continue = 0;

	if(I2Cx==I2C1)RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	else RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//!!!!!! OD mode!!!!!!!!
	if(I2Cx==I2C1)GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	else GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//Mode
	I2C_InitStructure.I2C_ClockSpeed = 200000;//时钟速度,speed
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//时钟占空比，clock_duty	
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;	//应答，选择使能
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//应答地址，选择7位，从机模式下才有效
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;//�?�?地址，从机模式下才有�?
	I2C_Init(I2Cx, &I2C_InitStructure);

	I2C_Cmd(I2Cx, ENABLE);

	return this;
}
