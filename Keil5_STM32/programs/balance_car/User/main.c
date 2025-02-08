#include "include.h"
/*......
2024.12.5 今天完成了速度数据的上传（到上位机）还没进行PID调试，
A轮的代码可能存在一些问题，因为其会使的其速度很高于目标值，已经排除是电机的问题，因为进行了电机交换了。
明天可以解决这个问题，并且将PID调制好。
2024.12.23 今天完成了mpu6050的软件调试，现在还没写对应姿态角的驱动，即前倾就往前开，后仰就往后开....
板子差不多画完原理图了。代码差对应四个方向的stm32电机驱动

2024.12.29 今天焊好了板子，也装上机了，对于遥控内容也测试了，可以正常遥控，现在差车子运动的代码，和与MPU6050联动。

对于电机驱动：直接调用MotorControl（）这个会出现一些问题，两个轮子的代码内的正转无法正常像反转一样稳定，在按着
正转的按钮时，输出的pwm波动太大，怀疑是编码器正转部分的读取错误，要继续排查。

对于联动MPU6050，目前没有有效进展

2025.1.9 解决了轮子不动的原因，因为改了PID,把之前的PID给删了，所以发现PB4脚用不了，看了引脚说明才发现，PB4默认是NJTRST
不能正常用来做IO口，之前可以使用怀疑是在删掉的某些地方上，顺便给PB4给复用了。
2025.1.11 项目基本完成，可以实现手机端移动遥控，速度调节，回传画面，可以保持基本的平衡，有一定的平衡保持力，抗击打。
现在可以增加一个复位主控和测控的按钮。

*/
//位置环调节
int Balence_PD(float roll , float gyrox);
//速度环调节
int Velocity_PI(int speed,int target);
//转向环
int Turn_PD(int encoder_left,int encoder_right,int turn);
void PID_motor(void);
void key_proc(void);

MPU6050 MM;	//创建一个结构体用来储存欧拉角
MPU6050_raw ram_MM;//要获取角速度
float newroll = 0;
float newpitch = 0;
int16_t Encoder_Left,Encoder_Right;
int V=0,V_base=5;//速度 - 前  +后   V_base是用来记录基准速度的，初始速度是5  
int TURN;//左右轮的转向  + 朝向左   -朝向右
KalmanFilter KK1;
KalmanFilter KK2;
int key_temp;//读取到的按键键值
char key_mode; //0-移动模式  1-调数据模式
char key_V_updata=0;//用于速度更改的 因为我这里是电平读取按键，所以在速度加加或减减时会加很多或减很多，所以用标志位 1+ 2-
char face_mode;  // 1-显示

int main(void)
{
	KalmanFilter_Init(&KK2,0,0.01,0.1);
	KalmanFilter_Init(&KK1,0,0.0001,0.1);
	Serial_Init(USART3,9600);//串口初始化
	All_init();
	OLED_ShowString(0,0,"Speed",12);
	OLED_ShowString(0,1,"baseV",12);
	OLED_ShowString(0,2,"Mode",12);
	while(1)
	{   
		
		OLED_ShowNum(40,0,V,5,12,1);
		OLED_ShowNum(40,1,V_base,5,12,1);
		OLED_ShowNum(40,2,key_mode,5,12,0);
		if(key_mode==1)
		{	
			
			OLED_ShowString(0,3,"****************",12);
			OLED_ShowString(85,0,"*****",12);
			OLED_ShowString(85,1,"*****",12);
			OLED_ShowString(85,2,"*****",12);
		}else
		{
			OLED_ShowString(0,3,"                ",12);
			OLED_ShowString(85,0,"     ",12);
			OLED_ShowString(85,1,"     ",12);
			OLED_ShowString(85,2,"     ",12);
		}
		key_proc();
		//MPU6050_Get_Angle(&MM);
		newroll = KalmanFilter_Update(&KK1,MM.roll); 
		newpitch = KalmanFilter_Update(&KK2,MM.pitch);
		printf("%.3f,%.3f,%.3f\n",newroll,newpitch,MM.yaw);
		
	}
}

//位置环调节
int Balence_PD(float roll , float gyrox)
{
	float Kp=494*0.75,Kd=1.85*0.7 ;// 794  1.75
//	float Kp=0,Kd=0;
	//这里的1.45是平衡中值1.8   1.5
	return (int)(Kp*(1.8-roll) + Kd*(0-gyrox));
}
//速度环调节
int Velocity_PI(int speed,int target)
{
	static float Kp=88,Ki=0.38;  //78 -0.5
	static int I=0,error=0;
	error = (speed-target);	
	I += error;
	if(I>4210)  I=4210;
	if(I<-4210) I=-4210;
	
	return (int)(Kp*error+Ki*I);
}
//转向环
int Turn_PD(int encoder_left,int encoder_right,int turn)
{
	static float Kp=88,Ki=+0.38;  //78 -0.5
	static int I=0,error=0;
	error = encoder_left-encoder_right+turn;	
	I += error;
	if(I>4210)  I=4210;
	if(I<-4210) I=-4210;
	
	return (int)(Kp*error+Ki*I);
	
}
//位置环的控制,不用mpu6050的外部中断脚  包含位置环控制，速度环控制
void PID_motor(void)
{
	Encoder_Left=Encoder_Get(3);
	Encoder_Right=Encoder_Get(4);
	MPU6050_Get_Angle(&MM);
	MPU6050_Get_Raw(&ram_MM);
	newroll = KalmanFilter_Update(&KK1,MM.roll);
	int16_t gyrox=ram_MM.GyroX;

	int motor_pwmA = Balence_PD(newroll,gyrox)+Velocity_PI((Encoder_Right),-V);  //位置环 加 速度环;
	int motor_pwmB = Balence_PD(newroll,gyrox)+Velocity_PI((Encoder_Left),-V);  //位置环 加 速度环;
	int turn_pwm_left=Turn_PD(Encoder_Left,Encoder_Right,TURN);
	int turn_pwm_right=Turn_PD(Encoder_Left,Encoder_Right,TURN);
	motor_pwmA = motor_pwmA+turn_pwm_left;
	motor_pwmB = motor_pwmB-turn_pwm_right;
	motor_pwmA=motor_pwm_max_min(motor_pwmA);//进行最大值最小值的限制
	motor_pwmB=motor_pwm_max_min(motor_pwmB);//进行最大值最小值的限制
	
	if(newroll>=50 || newroll<=-50)
	{
		motor_pwmA=0;
		motor_pwmB=0;
	}


	PID_SetPWM(motor_pwmA,motor_pwmB);
	
	
	
}

//pmu6050的外部中断引脚  用PA8做外部中断引脚
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line8)==SET)
	{
		PID_motor();
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}

void key_proc(void)
{
	key_temp=key_read();
	switch(key_temp)
	{
		case 1://左
			if(key_mode==0)
			{
				TURN=10;
				V=-3;
			}
		break;
		case 2://右
			if(key_mode==0)
			{
				TURN=-10;
				V=-3;
			}
		break;
		case 3://下
			if(key_mode==0)
			{
				V=-V_base;
			}else if(key_mode==1)
			{
				key_V_updata=2;
			}
		break;
		case 4://上
			if(key_mode==0)
			{
				V=V_base;
			}else if(key_mode==1)
			{
				key_V_updata=1;
			}
		break;
		case 0://松开按键
			V=0;
			TURN=0;
			if(key_mode==1)
			{
				if(key_V_updata==1) V_base++;
				if(key_V_updata==2) V_base--;
				key_V_updata=0;
			}
		break;
		case 5: //调速度模式
			key_mode=1;//调数据模式
		break;
		case 6: //普通模式
			key_mode=0;//移动模式
		break;
	}
}


