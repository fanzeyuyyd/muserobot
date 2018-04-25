#include "servo.h"
#include "myiic.h"
#include "math.h"
#include "delay.h"


uint8_t i2caddr=0x40;
extern Servo servol1,servol2,servol3,servol4,servor1,servor2,servor3,servor4;
/**************************************************************
初始化舵机控制板
****************************************************************/
void Write_servo_IIC(unsigned char addr,unsigned char data)
{
		IIC_Start();
		IIC_Send_Byte(SlaveAddress);
		IIC_Wait_Ack();
		IIC_Send_Byte(addr);
		IIC_Wait_Ack();
		IIC_Send_Byte(data);
		IIC_Wait_Ack();
		IIC_Stop();	
}

/********************************************************
从舵机控制板读取数据
********************************************************/

unsigned char Readservo_IIC(unsigned char REG_Address)
{
		u8 REG_data;
		IIC_Start();                   //起始信号
		IIC_Send_Byte(SlaveAddress);//发送设备地址+写信号
		IIC_Wait_Ack();
		IIC_Send_Byte(REG_Address);//发送存储单元地址，从0开始	
		IIC_Wait_Ack();	
		IIC_Start();                   //起始信号
		IIC_Send_Byte(SlaveAddress+1); //发送设备地址+读信号
		IIC_Wait_Ack(); 
		REG_data=IIC_Read_Byte(0);      //读出寄存器数据       
		IIC_Stop();                    //停止信号	
		return REG_data;
}


void servoreset(void)
{
		Write_servo_IIC(PCA9685_MODE1, 0x0);//PCA9685_MODE1
}
	

/*************************************************
舵机控制初始化函数
*************************************************/
void servobegin(void) {
 IIC_Init();
 servoreset();
}

void setPWMFreq(float freq) 
{
	uint8_t prescale;
	uint8_t oldmode;
	uint8_t newmode;
	float prescaleval = 25000000;
  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;
  prescale = floor(prescaleval + 0.5);
  oldmode = Readservo_IIC(PCA9685_MODE1);
	newmode = (oldmode&0x7F) | 0x10; // sleep
  Write_servo_IIC(PCA9685_MODE1, newmode); // go to sleep
  Write_servo_IIC(PCA9685_PRESCALE, prescale); // set the prescaler
  Write_servo_IIC(PCA9685_MODE1, oldmode);
  delay_ms(5);
  Write_servo_IIC(PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
                                          // This is why the beginTransmission below was not working.
  //  Serial.print("Mode now 0x"); Serial.println(read8(PCA9685_MODE1), HEX);
}

void setPWM(uint8_t num, uint16_t on, uint16_t off)
{
	uint8_t onl=0xff;
	uint8_t onh=0xff;
	uint8_t offl=0xff;
	uint8_t offh=0xff;
	onl=on&onl;
	onh=(on>>8)&onh;
	offl=off&offl;
	offh=(off>>8)&offh;
	IIC_Start();
	IIC_Send_Byte(SlaveAddress);
	IIC_Wait_Ack();
	IIC_Send_Byte(LED0_ON_L+4*num);
  IIC_Wait_Ack();
	IIC_Send_Byte(onl);
  IIC_Wait_Ack();
	IIC_Send_Byte(onh);
  IIC_Wait_Ack();
	IIC_Send_Byte(offl);
  IIC_Wait_Ack();
	IIC_Send_Byte(offh);
  IIC_Wait_Ack();
	IIC_Stop();
}

int GetData(unsigned char REG_Address)
{
	char  H,L;
	L=Readservo_IIC(REG_Address);
	H=Readservo_IIC(REG_Address+1);
	return (H<<8)+L;   //合成数据
}	

void LL(uint8_t number,uint8_t ang)
{
switch(number)
{
	case 1:setPWM(15,200,(242*ang)/100+365);break;
	
}
}
void leftleg(uint8_t number,int ang)
{
switch(number)
{
	case 1:setPWM(15,200,(242*(ang-57))/100+365);break;
	case 2:ang = 180-ang;setPWM(14,200,(242*(ang+35))/100+365);break;
	case 3:ang = 180 - ang;setPWM(13,200,(242*(ang-12))/100+365);break;
	case 4:
	if(ang < 70)ang = 70;
	if(ang > 150)ang =150;
	setPWM(12,200,(242*(ang+17))/100+365);break;
}
}
void rightleg(uint8_t number,int ang)
{
switch(number)
{
	case 1:ang = 180-ang;setPWM(0,200,(242*(ang-2))/100+365);break;
	case 2:setPWM(1,200,(242*(ang-60))/100+365);break;
	case 3:setPWM(2,200,(242*(ang-3))/100+365);break;
	case 4:
	if(ang < 70)ang = 70;
	if(ang > 150)ang =150;	
	ang = 180- ang;setPWM(3,200,(242*(ang-50))/100+365);break;

}
}
void W_z(int ang)
{
setPWM(7,200,(242*(ang+9))/100+365);
}
void W_x(int ang)
{
setPWM(6,200,(242*(ang-4))/100+365);
}
void W_y(int ang)
{
setPWM(9,200,(242*(ang-4))/100+365);
}

void refresh()
{	 
		setPWM(15,200,L1+servol1.cur/10);
		setPWM(14,200,L2-servol2.cur/10);
		setPWM(13,200,L3+servol3.cur/10);
		setPWM(12,200,L4+servol4.cur/10);
    setPWM(0,200,R1-servor1.cur/10);
		setPWM(1,200,R2+servor2.cur/10);
		setPWM(2,200,R3-servor3.cur/10);
		setPWM(3,200,R4-servor4.cur/10);
}
