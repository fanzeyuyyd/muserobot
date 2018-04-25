#ifndef __MUSE_H
#define __MUSE_H				

#include "sys.h"
			


typedef struct
{
  	u8 flag;       //0:无任务状态   1：任务执行中
		short last;
	  short cur;
	  short aim;
	  short min;
	  short max;
	  short  v;
	  short  a; 
} Servo;

void muse_init(void);
void servo_init(Servo* servo,short min,short max);
void explan(u8 *buf);
void servo_refresh(Servo* servo);
void set_servo(Servo* servo,short value,short v);
void stop_servo(Servo* servo);
void quick_move(Servo* servo);
#endif

