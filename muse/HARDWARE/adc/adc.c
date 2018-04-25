
#include "myiic.h"
#include "math.h"
#include "delay.h"
#include "adc.h"

typedef enum
{
  GAIN_TWOTHIRDS    = ADS1015_REG_CONFIG_PGA_6_144V,
 GAIN_ONE          = ADS1015_REG_CONFIG_PGA_4_096V,
  GAIN_TWO          = ADS1015_REG_CONFIG_PGA_2_048V,
  GAIN_FOUR         = ADS1015_REG_CONFIG_PGA_1_024V,
  GAIN_EIGHT        = ADS1015_REG_CONFIG_PGA_0_512V,
  GAIN_SIXTEEN      = ADS1015_REG_CONFIG_PGA_0_256V
} adsGain_t;



 void      setGain(adsGain_t gain);
  adsGain_t getGain(void);

   uint8_t   m_i2cAddress;
   uint8_t   m_conversionDelay;
   uint8_t   m_bitShift;
   adsGain_t m_gain;

static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) 
{
//  Wire.beginTransmission(i2cAddress);
//  i2cwrite((uint8_t)reg);
//  i2cwrite((uint8_t)(value>>8));
//  i2cwrite((uint8_t)(value & 0xFF));
//  Wire.endTransmission();
	
	IIC_Start();
	IIC_Send_Byte(i2cAddress);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)(value>>8));
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)(value & 0xFF));
	IIC_Wait_Ack();
	IIC_Stop();
}

/**************************************************************************/
/*!
    @brief  Writes 16-bits to the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg) 
{
//  Wire.beginTransmission(i2cAddress);
//  i2cwrite(ADS1015_REG_POINTER_CONVERT);
//  Wire.endTransmission();
//  Wire.requestFrom(i2cAddress, (uint8_t)2);
//  return ((i2cread() << 8) | i2cread()); 

  char H;
	char L;
	IIC_Start();
	IIC_Send_Byte(i2cAddress);
	IIC_Wait_Ack();
	IIC_Send_Byte(ADS1015_REG_POINTER_CONVERT); 
	IIC_Wait_Ack();
	IIC_Start();                   //起始信号
	IIC_Send_Byte(i2cAddress+1); //发送设备地址+读信号
  IIC_Wait_Ack(); 
  H=IIC_Read_Byte(1);
	L=IIC_Read_Byte(0);// ((IIC_Read_Byte(0)<<8)|IIC_Read_Byte(0))
  return(H<<8)+L;



	
}

/**************************************************************************/
/*!
    @brief  Instantiates a new ADS1015 class w/appropriate properties
*/
/**************************************************************************/

/**************************************************************************/
/*!
    @brief  Instantiates a new ADS1115 class w/appropriate properties
*/
/**************************************************************************/
void adc1115(uint8_t i2cAddress)
{
   m_i2cAddress = i2cAddress;
   m_conversionDelay = ADS1115_CONVERSIONDELAY;
   m_bitShift = 0;
   m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
}

/**************************************************************************/
/*!
    @brief  Sets up the HW (reads coefficients values, etc.)
*/
/**************************************************************************/

/**************************************************************************/
/*!
    @brief  Sets the gain and input voltage range
*/
/**************************************************************************/
void setGain(adsGain_t gain)
{
  m_gain = gain;
}

/**************************************************************************/
/*!
    @brief  Gets a gain and input voltage range
*/
/**************************************************************************/
adsGain_t getGain()
{
  return m_gain;
}

/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel
*/
/**************************************************************************/
uint16_t readADC_SingleEnded(uint8_t channel) 
{
	uint16_t config;
  if (channel < 3)
  {
    m_i2cAddress=0x90;
  }
	if(channel>3)
	{
	  m_i2cAddress=0x92;
	}
	if(channel>7)
	{
	 m_i2cAddress=0x94;
	}
  
  // Start with default values
   config = (ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE);   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= m_gain;

  // Set single-ended input channel
	while(channel>3)
	{
	channel-=4;
	}
  switch (channel)
  {
    case (0):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  delay_ms(m_conversionDelay);

  // Read the conversion results
  // Shift 12-bit results right 4 bits for the ADS1015
  return readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) ;
}

/**************************************************************************/
 

