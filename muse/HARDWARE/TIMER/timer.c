#include "timer.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!

#define UPDOWN 400
#define LEGDOWN 100
#define LEGUP 100

extern Servo servol1,servol2,servol3,servol4,servor1,servor2,servor3,servor4;
extern unsigned int f1,f2,f3,f4;   //压力传感器压力值
extern u8 status,lstatus,rstatus,last_status;
extern int down,down2;

void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	  
	  get_datal();
  	get_datar();	 
	  f1 = readfoot_1();
		f2 = readfoot_2();
	  f3 = readfoot_3();
		f4 = readfoot_4();
	  
	  /////////////////////////////////////////////////////////////
	  if(f1+f3 >= 10)///得到足部状态
			{
					lstatus = 1;
				  if(f2+f4 >= 10)
					{
							rstatus = 1;
						  status = 3;
					}else{
							rstatus = 0;
						  status = 1;
					}
			}else{
					lstatus = 0;
				  if(f2+f4 >= 10)
					{
							rstatus = 1;
						  status = 2;
					}else{
							rstatus = 0;
						  status = 0;
					}
			}
			/////////////////////////////////////////////////////////////
	  //printf("%d  %d  %d  %d\r\n",f1,f2,f3,f4);
	  if(status == 0)
		{
				set_servo(&servol1,0,10);
			  set_servo(&servol2,0,10);
				set_servo(&servol3,0,10);
				set_servo(&servol4,0,10);
			  set_servo(&servor1,0,10);
			  set_servo(&servor2,0,10);
				set_servo(&servor3,0,10);
				set_servo(&servor4,0,10);
			  down = 0;
			  status = 0;
			  last_status = 0;
		}else
		{   ////////////////////////////////////////求出down值
				if(status == 3)
				{
						if(roll1 < roll2)
						{
								down = 100*roll1 + 3*gyrox1; 
						}else
						{
								down = -100*roll2 - 3*gyrox2; 
						}
				}else if(status == 1)
				{
						down = 100*roll1 + 3*gyrox1;
				}else if(status == 2)
				{
						down = -100*roll2 - 3*gyrox2; 
				}
				down2 = 100*pitch1 + gyroy1/2;//前后down值
				//printf("%d\r\n",down2);
				////////////////////////////////////////
				if(status == 3)  //双腿落地状态
				{
					  if(last_status == 1&&down > UPDOWN)
						{
								set_servo(&servol3,50,LEGUP);						
							 	quick_move(&servol3);
							  stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servor1,0,30);	
                set_servo(&servor2,0,30);								
							  last_status = 3;//抬起了左腿							  
						}else if(last_status == 2&&down < -UPDOWN)
						{
								set_servo(&servor3,50,LEGUP);
							  quick_move(&servor3);
							  stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servol1,0,30);	
							  set_servo(&servol2,0,30);	
							  last_status = 4;//抬起了右腿
						}else if(last_status == 0&&down < -UPDOWN)
						{
								set_servo(&servor3,50,LEGUP);
							  quick_move(&servor3);
							  stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servol1,0,30);	
							  set_servo(&servol2,0,30);	
							  last_status = 4;//抬起了右腿
						}else
						{
							  //printf("%d\r\n",down);
//								if(last_status == 0)
//								{
//										set_servo(&servol1,-40,25);
//										set_servo(&servor1,40,25);
//								}else if(last_status == 1)
//								{
//										set_servo(&servol1,40,25);
//										set_servo(&servor1,-40,25);
//								}else if(last_status == 2)
//								{
//										set_servo(&servol1,-40,25);
//										set_servo(&servor1,40,25);
//								}
						}						
				
				}else if(status == 1)//左腿落地右腿离地状态
				{
						if(last_status == 2&&down < -UPDOWN)
						{
								set_servo(&servor3,50,LEGUP);
							  quick_move(&servor3);
						  	stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servol1,0,30);	
							  set_servo(&servol2,0,30);	
							  last_status = 4;//右腿抬起
						}else if(last_status == 0&&down < -UPDOWN)
						{
								set_servo(&servor3,50,LEGUP);
							  quick_move(&servor3);
						  	stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servol1,0,30);	
							  set_servo(&servol2,0,30);	
							  last_status = 4;//右腿抬起
						}else//右腿已经抬起
						{
								if(last_status == 4 && gyrox1 > 0)//down > -380
								{
										set_servo(&servor3,0,LEGDOWN);
									  //printf("r:  %f  %d\r\n",roll1,gyrox1);
									  last_status = 1;//右腿落下
									  if(down > -500)
										{
												set_servo(&servor1,(down+500)/100,LEGUP);
											  quick_move(&servor1);											  
										}
										if(down2 > -100)//前倾
										{
										    set_servo(&servor2,(down2+100)/50,LEGUP);//
											  quick_move(&servor2);	
											  printf("%d\r\n",servor2.aim);
										}
										else if(down2 < -100)
										{
										    set_servo(&servor2,(down2+100)/50,LEGUP);
											  quick_move(&servor2);	
											  printf("%d\r\n",servor2.aim);
										}
								}else if(last_status == 4 && roll1 > -2) 
								{
										set_servo(&servor3,0,LEGDOWN);
									  //quick_move(&servor3);
									  //printf("r:  %f  %d\r\n",roll1,gyrox1);
									  last_status = 1;//右腿落下
									  if(down > -500)
										{
												set_servo(&servor1,(down+500)/100,LEGUP);
											  quick_move(&servor1);											  
										}
										if(down2 > -100)//前倾
										{
										    set_servo(&servor2,(down2+100)/50,LEGUP);//(down2+50)/50
											  quick_move(&servor2);	
											  printf("%d\r\n",servor2.aim);
										}
										else if(down2 < -100)
										{
										    set_servo(&servor2,(down2+100)/50,LEGUP);
											  quick_move(&servor2);	
											  printf("%d\r\n",servor2.aim);
										}
								}
								
						}
//						if(down < -1000)
//						{
//							  int n = 0;
//							  n = servor1.last+((-(down+1000))/5);
//							  //printf("%d  %d\r\n",servor1.last,n);
//								set_servo(&servor1,n,10);
//								quick_move(&servor1);
//						}
				}else if(status == 2)//右腿落地左腿离地状态
				{
						if(last_status == 1&&down > UPDOWN)
						{
								set_servo(&servol3,50,LEGUP);
							  quick_move(&servol3);
						  	stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servor1,0,30);
                set_servo(&servor2,0,30);
							  last_status = 3;//左腿抬起
						}else//左腿已经抬起
						{
								if(last_status == 3 && gyrox2 > 0)//down < 380
								{
										set_servo(&servol3,0,LEGDOWN);
								  	//quick_move(&servol3);
									  //printf("l:  %f  %d\r\n",roll2,gyrox2);
									  last_status = 2;//左腿落下
									  if(down < 500)
										{
												set_servo(&servol1,(500-down)/100,LEGUP);
											  quick_move(&servol1);											  
										}
										if(down2 > -100)
										{
										    set_servo(&servol2,(down2+100)/50,LEGUP);
											  quick_move(&servol2);	
											  printf("%d\r\n",servol2.aim);
										}
										else if(down2 < -100)
										{
										    set_servo(&servol2,(down2+100)/50,LEGUP);
											  quick_move(&servol2);	
											  printf("%d\r\n",servol2.aim);
										}
										
								}else if(last_status == 3 && roll2 > -2) 
								{
										set_servo(&servol3,0,LEGDOWN);
								  	//quick_move(&servol3);
									  //printf("l:  %f  %d\r\n",roll2,gyrox2);
									  last_status = 2;//左腿落下
									  if(down < 500)
										{
												set_servo(&servol1,(500-down)/100,LEGUP);
											  quick_move(&servol1);											  
										}
										if(down2 > -100)
										{
										    set_servo(&servol2,(down2+100)/50,LEGUP);
											  quick_move(&servol2);	
											  printf("%d\r\n",servol2.aim);
										}
										else if(down2 < -100)
										{
										    set_servo(&servol2,(down2+100)/50,LEGUP);
											  quick_move(&servol2);	
											  printf("%d\r\n",servol2.aim);
										}
								}
								
						}
//						if(down > 10)
//						{
//							  int n = 0;
//							  n = servol1.last+((down-1000)/5);
//						  	//printf("%d  %d\r\n",servor1.last,n);
//								set_servo(&servol1,n,10);
//								quick_move(&servol1);
//						}
				}
				
				
				
				
								
		}
		
		
		
		//printf("%d\r\n",down);
	  servo_refresh(&servol1);
	  servo_refresh(&servol2);
	  servo_refresh(&servol3);
	  servo_refresh(&servol4);
	  servo_refresh(&servor1);
	  servo_refresh(&servor2);
	  servo_refresh(&servor3);
	  servo_refresh(&servor4);

	  refresh();
	  //printf("*");
	  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位	 
	  
}


