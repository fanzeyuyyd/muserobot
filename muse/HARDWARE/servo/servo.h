#ifndef _servo_H
#define _servo_H
#endif

#include "sys.h"

extern int l1,l2,l3,l4,r1,r2,r3,r4;


#define L1 465
#define L2 615
#define L3 440
#define L4 712
#define R1 540
#define R2 506
#define R3 663
#define R4 377


#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD
#define	SlaveAddress	0x80	//IIC写入时的地址字节数据，+1为读取


  //void Adafruit_PWMServoDriver(uint8_t addr = 0x40);
   void servoreset(void);
	 void servobegin(void);
 
  void setPWMFreq(float freq);
  void setPWM(uint8_t num, uint16_t on, uint16_t off);
  //void setPin(uint8_t num, uint16_t val, bool invert=false);


  

  uint8_t read8(uint8_t addr);
  void write8(uint8_t addr, uint8_t d);
void Write_servo_IIC(unsigned char addr,unsigned char data);
unsigned char Readservo_IIC(unsigned char REG_Address);
//unsigned char Readservo_one(unsigned char ReadAddr);
int GetData(unsigned char REG_Address);
void leftleg(uint8_t number,int ang);
void rightleg(uint8_t number,int ang);
void W_z(int ang);
void W_x(int ang);	
void W_y(int ang);
void refresh(void);
void MPU_lInit(void);
void MPU_rInit(void);
void get_datal(void);
void get_datar(void);


