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
int p1,p2,p3,p4;  //压力传感器初始值
int l1,l2,l3,l4,r1,r2,r3,r4;
unsigned int f1,f2,f3,f4;   //压力传感器压力值
float pitch1,roll1,yaw1,pitch2,roll2,yaw2; 		//欧拉角
short gyrox1,gyroy1,gyroz1,gyrox2,gyroy2,gyroz2;
int down,down2;
u8 status,lstatus,rstatus,last_status;  //0：悬空  1：左腿落地  2：右腿落地  3：双腿落地   
//last_status: 0:站立  1：落下了右腿  2：落了左腿  3：抬起了左腿  4：抬起了右腿
Servo servol1,servol2,servol3,servol4,servor1,servor2,servor3,servor4;


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	servobegin(); 
	setPWMFreq(60);	
	hx711_Init();	
	presen_init();
	uart_init(38400);		//初始化串口波特率为38400
	 
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
	LED_Init();					//初始化LED  		
	MPU_lInit();//初始化MPU6050
	MPU_rInit();		
  TIM3_Init(200-1,8400-1);//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数200次为20ms 
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


