#include "hx711.h"
#include "delay.h"
#include "usart.h"


extern int p1,p2,p3,p4;

//hx711初始化函数，一次性初始化四个模块
void hx711_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6; //KEY0 KEY1 KEY2对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOE3
	
}


//读取模块1的数值
unsigned long readCount_1(void)
{
    unsigned long Count;
    unsigned char i;
    ADSK1=0; 
    Count=0;
    while(ADDO1);
	
		
    for (i=0;i<24;i++)
    {
        ADSK1=1;
        Count = Count<<1; 
			  
        ADSK1 = 0;
       if(ADDO1) Count++;
    }
    ADSK1=1;
    Count=Count^0x800000;//?25????????,????
    ADSK1=0;
		Count = Count/1000;
    return Count;  
}


//读取模块2的数值
unsigned long readCount_2(void)
{
    unsigned long Count;
    unsigned char i;
    ADSK2=0; 
    Count=0;
    while(ADDO2);
		
		
    for (i=0;i<24;i++)
    {
        ADSK2=1;
        Count = Count<<1; 
			  
        ADSK2 = 0;
       if(ADDO2) Count++;
    }
    ADSK2=1;
    Count=Count^0x800000;//?25????????,????
    ADSK2=0;
		Count = Count/1000;
    return Count; 
}


//读取模块3的数值
unsigned long readCount_3(void)
{
    unsigned long Count;
    unsigned char i;
    ADSK3=0; 
    Count=0;
    while(ADDO3);
		
    for (i=0;i<24;i++)
    {
        ADSK3=1;
        Count = Count<<1; 
			  
        ADSK3 = 0;
       if(ADDO3) Count++;
    }
    ADSK3=1;
    Count=Count^0x800000;//?25????????,????
    ADSK3=0;
		Count = Count/1000;
    return Count; 
}


//读取模块4的数值
unsigned long readCount_4(void)
{
    unsigned long Count;
    unsigned char i;
    ADSK4=0; 
    Count=0;
    while(ADDO4);
    for (i=0;i<24;i++)
    {
        ADSK4=1;
        Count = Count<<1; 
			  
        ADSK4 = 0;
       if(ADDO4) Count++;
    }
    ADSK4=1;
    Count=Count^0x800000;//?25????????,????
    ADSK4=0;
		Count = Count/1000;
    return Count; 
}

unsigned int readfoot_1(void)
{
		unsigned int pressure;
		pressure = readCount_1() - p1;
	  if(pressure > 1000)
		{
				return 0;
		}
		pressure = pressure/11;
		if(pressure <= 0)
		{
				return 0;
		}
		return pressure;
}

unsigned int readfoot_2(void)
{
		unsigned int pressure;
		pressure = readCount_2() - p2;
	  if(pressure > 1000)
		{
				return 0;
		}
		pressure = pressure/13;
		if(pressure <= 0)
		{
				return 0;
		}
		return pressure;
}

unsigned int readfoot_3(void)
{
		unsigned int pressure;
		pressure = readCount_3() - p3;
	  if(pressure > 1000)
		{
				return 0;
		}
		pressure = pressure/12;\
		if(pressure <= 0)
		{
				return 0;
		}
		return pressure;
}

unsigned int readfoot_4(void)
{
		unsigned int pressure;
		pressure = readCount_4() - p4;
	  if(pressure > 1000)
		{
				return 0;
		}
		pressure = pressure/13;
		if(pressure <= 0)
		{
				return 0;
		}
		return pressure;
}

void presen_init()
{
	  p1 = readCount_1();
		p2 = readCount_2();
		p3 = readCount_3();
		p4 = readCount_4();
}

