#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"  
#include "myiic.h"
#include "Servo.h"
#include "adc.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "malloc.h" 
#include "sdio_sdcard.h"  
#include "muse.h"
#include "hx711.h"
#include "timer.h"

u8 len,t;
u16 adc0;
u16 initvalue_l1,initvalue_l2,initvalue_l3,initvalue_l4,initvalue_r1,initvalue_r2,initvalue_r3,initvalue_r4;
int p1,p2,p3,p4;  //ѹ����������ʼֵ
int l1,l2,l3,l4,r1,r2,r3,r4;
unsigned int f1,f2,f3,f4;   //ѹ��������ѹ��ֵ
float pitch1,roll1,yaw1,pitch2,roll2,yaw2; 		//ŷ����
short gyrox1,gyroy1,gyroz1,gyrox2,gyroy2,gyroz2;
int down,down2;
u8 status,lstatus,rstatus,last_status;  //0������  1���������  2���������  3��˫�����   
//last_status: 0:վ��  1������������  2����������  3��̧��������  4��̧��������
Servo servol1,servol2,servol3,servol4,servor1,servor2,servor3,servor4;


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	servobegin(); 
	setPWMFreq(60);	
	hx711_Init();	
	presen_init();
	uart_init(38400);		//��ʼ�����ڲ�����Ϊ38400
	 
	l1 = 0;
  l2 = 0;
  l3 = 0;
  l4 = 0;
  r1 = 0;
  r2 = 0;
  r3 = 0;
  r4 = 0;
	muse_init();
	refresh();
	delay_ms(1000);		
	LED_Init();					//��ʼ��LED  		
	MPU_lInit();//��ʼ��MPU6050
	MPU_rInit();		
  TIM3_Init(200-1,8400-1);//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����200��Ϊ20ms 
	while(1)
	{		
		 
		
		
		
		
			for(int n = 0;n < 100;n++)
		{
				n--;
		}
		
			//set_servo(&servol1,-30);
			//set_servo(&servor1,30);
			
		 
		  //printf("!\r\n");
	} 
}


