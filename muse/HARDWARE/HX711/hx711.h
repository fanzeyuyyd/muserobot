#ifndef __HX711_H
#define __HX711_H
#include "sys.h"

#define ADSK1  PAout(4)
#define ADDO1  PAin(6)  
#define ADSK2  PAout(3)
#define ADDO2  PAin(1)  
#define ADSK3  PAout(0)
#define ADDO3  PAin(2)  
#define ADSK4  PAout(7)
#define ADDO4  PAin(5)  


void hx711_Init(void);
unsigned long readCount_1(void);
unsigned long readCount_2(void);
unsigned long readCount_3(void);
unsigned long readCount_4(void);

unsigned int readfoot_1(void);
unsigned int readfoot_2(void);
unsigned int readfoot_3(void);
unsigned int readfoot_4(void);
void presen_init(void);

#endif

