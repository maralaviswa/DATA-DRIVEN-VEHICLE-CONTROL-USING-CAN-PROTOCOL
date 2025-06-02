#include<lpc21xx.h>
#include"lcd.h"
#include"MACROS.h"
#include"lcd_defines.h"

void init_LCD(void)
{
	WRITEBYTE(IODIR0,LCD,0x7FF);
	delay_us(15);
	cmd_LCD(0x30);
	delay_ms(5);
	cmd_LCD(0x30);
	delay_us(100);
	cmd_LCD(0x30);
	cmd_LCD(0x38);
	cmd_LCD(0x0F);
	cmd_LCD(0x01);
	cmd_LCD(0x06);
}
void cmd_LCD(u32 cmd)
{
	SCLRBIT(IOCLR0,LCD_RS);
	write_LCD(cmd);
}

void data_LCD(u32 data)
{
	SSETBIT(IOSET0,LCD_RS);
	write_LCD(data);
}
void write_LCD(u32 data)
{
	//SCLRBIT(IOCLR0,LCD_RW);
	WRITEBYTE(IOPIN0,LCD,data);
	SSETBIT(IOSET0,LCD_EN);
	delay_us(1);
	SCLRBIT(IOCLR0,LCD_EN);
	delay_ms(2);
}

 void U32LCD(u32 num)
{
	u8 arr[10];
	s8 i=0;
	if(num==0)
		data_LCD('0');
	while(num)
	{
		arr[i++]=(num%10)+48;
		num/=10;
	}
	for(--i;i>=0;i--)
	data_LCD(arr[i]);
}

void S32LCD(s32 num)
{
	if(num<0)
	{
		//display '-' on LCD
		data_LCD('-');		
		//change the sign of the given number
		num=-num;		
	}
	//display updated number after sign change
	U32LCD(num);	
}

void F32LCD(f32 fNum,u32 nDP)
{
	s32 iNum;
	if(fNum<0.0)
	{
		//display '-' on LCD
		data_LCD('-');		
		//change the sign of the given number
		fNum = -fNum;	
	}
	//assign float number to integer variable	
	iNum = fNum;	
	//display integer value on LCD
	U32LCD(iNum);
	//display '.' on LCD
	data_LCD('.');
	//extract digit by digit after decimal point and display it on LCD up to given no.of digits after decimal point
	while(nDP)
	{
		fNum = (fNum-iNum)*10;
		iNum = fNum;
		data_LCD(iNum+0x30);
		nDP--;
	}
}


void str_LCD(s8 * str)
{
	
	while(*str)
	{
		data_LCD(*str++);
	}
}


 void BuildCGRAM(u8 *p,u8 num)

{

	u32 i;

	//Goto CGRAM start

	cmd_LCD(0x40);	

	for(i=0;i<num;i++)

	{

		//write into cgram via data reg

		cmd_LCD(p[i]);		

	}

	//come back to ddram

	cmd_LCD(0x80);

}


