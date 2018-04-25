#include "muse.h"
#include "sys.h"
#include "sdio_sdcard.h"  
#include "adc.h"
#include "usart.h"
#include "hx711.h"



extern int l1,l2,l3,l4,r1,r2,r3,r4;  
extern u8 tex[362];
extern u16 initvalue_l1,initvalue_l2,initvalue_l3,initvalue_l4,initvalue_r1,initvalue_r2,initvalue_r3,initvalue_r4;
extern u16 f_l1,f_l2,f_l3,f_l4,f_r1,f_r2,f_r3,f_r4;
extern int f1,f2,f3,f4;
extern Servo servol1,servol2,servol3,servol4,servor1,servor2,servor3,servor4;
extern u8 status,lstatus,rstatus,last_status;


void servo_init(Servo* servo,short min,short max)
{
		servo->a = 0;
	  servo->v = 0;
	  servo->aim = 0;
	  servo->cur = 0;
	  servo->flag = 0;
	  servo->last = 0;
	  servo->min = min;
	  servo->max = max;
}

void muse_init()
{
		servo_init(&servol1,-42,90);
	  servo_init(&servol2,-160,206);
	  servo_init(&servol3,-15,210);
	  servo_init(&servol4,-128,72);
	  servo_init(&servor1,-42,90);
	  servo_init(&servor2,-160,206);
	  servo_init(&servor3,-15,210);
	  servo_init(&servor4,-128,72);  
	  last_status = 0;
	  status = 0;
}


void explan(u8 *buf)
{
     if(buf[0]!='*')
     {
       //  printf("error:*"); 
         return; 
     } 
//		 if(buf[25]!='!')
//     {
//         printf("error:!"); 
//         return; 
//     } 
   
          l1 = 0+(buf[1]-48)*100;
          l1 = l1+(buf[2]-48)*10;
          l1 = l1+(buf[3]-48);
       
          l2 = 0+(buf[4]-48)*100;
          l2= l2+(buf[5]-48)*10;
          l2 = l2+(buf[6]-48);
    
          l3 = 0+(buf[7]-48)*100;
          l3 = l3+(buf[8]-48)*10;
          l3 = l3+(buf[9]-48);
       
          l4 = 0+(buf[10]-48)*100;
          l4 = l4+(buf[11]-48)*10;
          l4 = l4+(buf[12]-48);
    
          r1 = 0+(buf[13]-48)*100;
          r1 = r1+(buf[14]-48)*10;
          r1 = r1+(buf[15]-48);
       
          r2 = 0+(buf[16]-48)*100;
          r2 = r2+(buf[17]-48)*10;
          r2 = r2+(buf[18]-48);
    
          r3 = 0+(buf[19]-48)*100;
          r3 = r3+(buf[20]-48)*10;
          r3 = r3+(buf[21]-48);
       
          r4 = 0+(buf[22]-48)*100;
          r4 = r4+(buf[23]-48)*10;
          r4 = r4+(buf[24]-48);
}

void servo_refresh(Servo* servo)
{
	  short v;	  
		if(servo->cur / 10!= servo->aim)
		{
			  v = (10*servo->aim - servo->cur)/5+5;
				if(v > servo->v)
				{
						v = servo->v;
				}else if(v < -servo->v)
				{
						v = -servo->v;
				}
			  servo->cur += v;
				if(servo->cur < servo->min*10)
				{
						servo->cur = 10*servo->min;
				}else if(servo->cur > 10*servo->max)
				{
						servo->cur = 10*servo->max;
				}
		}			
}


void set_servo(Servo* servo,short value,short v)
{
		servo->aim = value;
	  servo->v = v;
}

void stop_servo(Servo* servo)
{
		servo->aim = servo->cur/10;
	  servo->v = 0;
	  servo->a = 0;
	  servo->last = servo->cur/10;
}

void quick_move(Servo* servo)//,short aim)
{
	  //servo->aim = aim;
		servo->cur = servo->aim*10;
	  servo->a = 0;
	  servo->v = 0;
	  
}

