#include "timer.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!

#define UPDOWN 400
#define LEGDOWN 100
#define LEGUP 100

extern Servo servol1,servol2,servol3,servol4,servor1,servor2,servor3,servor4;
extern unsigned int f1,f2,f3,f4;   //ѹ��������ѹ��ֵ
extern u8 status,lstatus,rstatus,last_status;
extern int down,down2;

void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	  
	  get_datal();
  	get_datar();	 
	  f1 = readfoot_1();
		f2 = readfoot_2();
	  f3 = readfoot_3();
		f4 = readfoot_4();
	  
	  /////////////////////////////////////////////////////////////
	  if(f1+f3 >= 10)///�õ��㲿״̬
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
		{   ////////////////////////////////////////���downֵ
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
				down2 = 100*pitch1 + gyroy1/2;//ǰ��downֵ
				//printf("%d\r\n",down2);
				////////////////////////////////////////
				if(status == 3)  //˫�����״̬
				{
					  if(last_status == 1&&down > UPDOWN)
						{
								set_servo(&servol3,50,LEGUP);						
							 	quick_move(&servol3);
							  stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servor1,0,30);	
                set_servo(&servor2,0,30);								
							  last_status = 3;//̧��������							  
						}else if(last_status == 2&&down < -UPDOWN)
						{
								set_servo(&servor3,50,LEGUP);
							  quick_move(&servor3);
							  stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servol1,0,30);	
							  set_servo(&servol2,0,30);	
							  last_status = 4;//̧��������
						}else if(last_status == 0&&down < -UPDOWN)
						{
								set_servo(&servor3,50,LEGUP);
							  quick_move(&servor3);
							  stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servol1,0,30);	
							  set_servo(&servol2,0,30);	
							  last_status = 4;//̧��������
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
				
				}else if(status == 1)//��������������״̬
				{
						if(last_status == 2&&down < -UPDOWN)
						{
								set_servo(&servor3,50,LEGUP);
							  quick_move(&servor3);
						  	stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servol1,0,30);	
							  set_servo(&servol2,0,30);	
							  last_status = 4;//����̧��
						}else if(last_status == 0&&down < -UPDOWN)
						{
								set_servo(&servor3,50,LEGUP);
							  quick_move(&servor3);
						  	stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servol1,0,30);	
							  set_servo(&servol2,0,30);	
							  last_status = 4;//����̧��
						}else//�����Ѿ�̧��
						{
								if(last_status == 4 && gyrox1 > 0)//down > -380
								{
										set_servo(&servor3,0,LEGDOWN);
									  //printf("r:  %f  %d\r\n",roll1,gyrox1);
									  last_status = 1;//��������
									  if(down > -500)
										{
												set_servo(&servor1,(down+500)/100,LEGUP);
											  quick_move(&servor1);											  
										}
										if(down2 > -100)//ǰ��
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
									  last_status = 1;//��������
									  if(down > -500)
										{
												set_servo(&servor1,(down+500)/100,LEGUP);
											  quick_move(&servor1);											  
										}
										if(down2 > -100)//ǰ��
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
				}else if(status == 2)//��������������״̬
				{
						if(last_status == 1&&down > UPDOWN)
						{
								set_servo(&servol3,50,LEGUP);
							  quick_move(&servol3);
						  	stop_servo(&servol1);
							  stop_servo(&servor1);
							  set_servo(&servor1,0,30);
                set_servo(&servor2,0,30);
							  last_status = 3;//����̧��
						}else//�����Ѿ�̧��
						{
								if(last_status == 3 && gyrox2 > 0)//down < 380
								{
										set_servo(&servol3,0,LEGDOWN);
								  	//quick_move(&servol3);
									  //printf("l:  %f  %d\r\n",roll2,gyrox2);
									  last_status = 2;//��������
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
									  last_status = 2;//��������
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
	  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ	 
	  
}


