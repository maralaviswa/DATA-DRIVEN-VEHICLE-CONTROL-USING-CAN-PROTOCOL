#include<LPC21xx.H>
#include"delay.h"
#include"interrupts.h"


void ini_INTERUPT(void)
{
	
	PINSEL0|=2<<(EXT_INT1_PIN*2);
	PINSEL1|=1<<((16-EXT_INT0_PIN)*2);
	PINSEL0|=  (0x2UL << (EXT_INT2_PIN * 2));
	
	
  VICIntSelect=0;
	
	
	VICIntEnable|=1<<EXT_INT0 ;
	VICVectCntl0=((1<<5)| EXT_INT0);	
	VICVectAddr0=(unsigned)interupt_0;

	VICIntEnable|=1<<EXT_INT1 ;
	VICVectCntl1=((1<<5)| EXT_INT1);
	VICVectAddr1=(unsigned)interupt_1;
	
	VICIntEnable|=1<<EXT_INTO2;
	VICVectCntl2=((1<<5)|EXT_INTO2);
	VICVectAddr2=(unsigned)interupt_2;
		
	EXTMODE=(1<<0)|(1<<1)|(1<<2);
	EXTPOLAR=0;
}


