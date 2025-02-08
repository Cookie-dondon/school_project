#include "include.h"
/*......
2024.12.5 ����������ٶ����ݵ��ϴ�������λ������û����PID���ԣ�
A�ֵĴ�����ܴ���һЩ���⣬��Ϊ���ʹ�����ٶȺܸ���Ŀ��ֵ���Ѿ��ų��ǵ�������⣬��Ϊ�����˵�������ˡ�
������Խ��������⣬���ҽ�PID���ƺá�
2024.12.23 ���������mpu6050��������ԣ����ڻ�ûд��Ӧ��̬�ǵ���������ǰ�����ǰ��������������....
���Ӳ�໭��ԭ��ͼ�ˡ�������Ӧ�ĸ������stm32�������

2024.12.29 ���캸���˰��ӣ�Ҳװ�ϻ��ˣ�����ң������Ҳ�����ˣ���������ң�أ����ڲ���˶��Ĵ��룬����MPU6050������

���ڵ��������ֱ�ӵ���MotorControl������������һЩ���⣬�������ӵĴ����ڵ���ת�޷�������תһ���ȶ����ڰ���
��ת�İ�ťʱ�������pwm����̫�󣬻����Ǳ�������ת���ֵĶ�ȡ����Ҫ�����Ų顣

��������MPU6050��Ŀǰû����Ч��չ

2025.1.9 ��������Ӳ�����ԭ����Ϊ����PID,��֮ǰ��PID��ɾ�ˣ����Է���PB4���ò��ˣ���������˵���ŷ��֣�PB4Ĭ����NJTRST
��������������IO�ڣ�֮ǰ����ʹ�û�������ɾ����ĳЩ�ط��ϣ�˳���PB4�������ˡ�
2025.1.11 ��Ŀ������ɣ�����ʵ���ֻ����ƶ�ң�أ��ٶȵ��ڣ��ش����棬���Ա��ֻ�����ƽ�⣬��һ����ƽ�Ᵽ������������
���ڿ�������һ����λ���غͲ�صİ�ť��

*/
//λ�û�����
int Balence_PD(float roll , float gyrox);
//�ٶȻ�����
int Velocity_PI(int speed,int target);
//ת��
int Turn_PD(int encoder_left,int encoder_right,int turn);
void PID_motor(void);
void key_proc(void);

MPU6050 MM;	//����һ���ṹ����������ŷ����
MPU6050_raw ram_MM;//Ҫ��ȡ���ٶ�
float newroll = 0;
float newpitch = 0;
int16_t Encoder_Left,Encoder_Right;
int V=0,V_base=5;//�ٶ� - ǰ  +��   V_base��������¼��׼�ٶȵģ���ʼ�ٶ���5  
int TURN;//�����ֵ�ת��  + ������   -������
KalmanFilter KK1;
KalmanFilter KK2;
int key_temp;//��ȡ���İ�����ֵ
char key_mode; //0-�ƶ�ģʽ  1-������ģʽ
char key_V_updata=0;//�����ٶȸ��ĵ� ��Ϊ�������ǵ�ƽ��ȡ�������������ٶȼӼӻ����ʱ��Ӻܶ����ܶ࣬�����ñ�־λ 1+ 2-
char face_mode;  // 1-��ʾ

int main(void)
{
	KalmanFilter_Init(&KK2,0,0.01,0.1);
	KalmanFilter_Init(&KK1,0,0.0001,0.1);
	Serial_Init(USART3,9600);//���ڳ�ʼ��
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

//λ�û�����
int Balence_PD(float roll , float gyrox)
{
	float Kp=494*0.75,Kd=1.85*0.7 ;// 794  1.75
//	float Kp=0,Kd=0;
	//�����1.45��ƽ����ֵ1.8   1.5
	return (int)(Kp*(1.8-roll) + Kd*(0-gyrox));
}
//�ٶȻ�����
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
//ת��
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
//λ�û��Ŀ���,����mpu6050���ⲿ�жϽ�  ����λ�û����ƣ��ٶȻ�����
void PID_motor(void)
{
	Encoder_Left=Encoder_Get(3);
	Encoder_Right=Encoder_Get(4);
	MPU6050_Get_Angle(&MM);
	MPU6050_Get_Raw(&ram_MM);
	newroll = KalmanFilter_Update(&KK1,MM.roll);
	int16_t gyrox=ram_MM.GyroX;

	int motor_pwmA = Balence_PD(newroll,gyrox)+Velocity_PI((Encoder_Right),-V);  //λ�û� �� �ٶȻ�;
	int motor_pwmB = Balence_PD(newroll,gyrox)+Velocity_PI((Encoder_Left),-V);  //λ�û� �� �ٶȻ�;
	int turn_pwm_left=Turn_PD(Encoder_Left,Encoder_Right,TURN);
	int turn_pwm_right=Turn_PD(Encoder_Left,Encoder_Right,TURN);
	motor_pwmA = motor_pwmA+turn_pwm_left;
	motor_pwmB = motor_pwmB-turn_pwm_right;
	motor_pwmA=motor_pwm_max_min(motor_pwmA);//�������ֵ��Сֵ������
	motor_pwmB=motor_pwm_max_min(motor_pwmB);//�������ֵ��Сֵ������
	
	if(newroll>=50 || newroll<=-50)
	{
		motor_pwmA=0;
		motor_pwmB=0;
	}


	PID_SetPWM(motor_pwmA,motor_pwmB);
	
	
	
}

//pmu6050���ⲿ�ж�����  ��PA8���ⲿ�ж�����
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
		case 1://��
			if(key_mode==0)
			{
				TURN=10;
				V=-3;
			}
		break;
		case 2://��
			if(key_mode==0)
			{
				TURN=-10;
				V=-3;
			}
		break;
		case 3://��
			if(key_mode==0)
			{
				V=-V_base;
			}else if(key_mode==1)
			{
				key_V_updata=2;
			}
		break;
		case 4://��
			if(key_mode==0)
			{
				V=V_base;
			}else if(key_mode==1)
			{
				key_V_updata=1;
			}
		break;
		case 0://�ɿ�����
			V=0;
			TURN=0;
			if(key_mode==1)
			{
				if(key_V_updata==1) V_base++;
				if(key_V_updata==2) V_base--;
				key_V_updata=0;
			}
		break;
		case 5: //���ٶ�ģʽ
			key_mode=1;//������ģʽ
		break;
		case 6: //��ͨģʽ
			key_mode=0;//�ƶ�ģʽ
		break;
	}
}


