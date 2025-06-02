#ifndef _ADC_DEFINES_H
#define _ADC_DEFINES_H


//defines for ADCR
#define CH0 (1<<0)
#define CH1 (1<<1)           
#define CH2 (1<<2)          
#define CH3 (1<<3)           

#define FOSC		12000000               
#define CCLK    (5*FOSC)           
#define PCLK    (CCLK/4)           
#define ADCLK   3750000           
#define CLKDIV  ((PCLK/ADCLK)-1)           

#define PDN_BIT       21
#define CLKS	      17 
#define ADC_START_BIT 24 


//defines for ADDR 
#define DONE_BIT      31



#endif


