#include <LPC21xx.H>

#include "adc_defines.h"

#include "MACROS.h"
#include"type.h"
#include"delay.h"




void Init_ADC(void)

{

  ADCR = (1<<PDN_BIT)|(0<<CLKS)|(CLKDIV<<8);	

}


f32 Read_ADC(u8 chNo)
{
	u16 adcVal=0;f32 eAR;
	u32 DC,distance;

  //select channel

	WRITEBYTE(ADCR,0,chNo);  

	//start the adc conversion

	SETBIT(ADCR,ADC_START_BIT);  

	//wait for conversion time (10 bit conversion time: 2.44 us)

  delay_us(3);

	//wait for done bit

  while(READBIT(ADDR,DONE_BIT) == 0);

	//stop the adc conversion

  CLRBIT(ADCR,ADC_START_BIT);  

	//collect the ADDR register content into adcVal variable

  adcVal = (ADDR>>6)&0x3FF;

	//eAR=((converted voltage*vref)/2^n-1);
   
	eAR = adcVal*(3.3/1023);

	DC=(eAR*(1024/5));
	distance=(6787/(DC-3))-4;
	if(distance<10)
	distance=10;
	else if(distance>80)
	distance=80;
  return distance;
}



