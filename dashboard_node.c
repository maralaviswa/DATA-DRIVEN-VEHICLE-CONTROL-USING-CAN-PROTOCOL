#include"delay.h"
#include"can.h"
#include"lcd.h"
#include"interrupts.h"
#include"can_defines.h"
#include"DS18B20.h"
#include"Ds18B20_defines.h"
#include<lpc21xx.h>
#include<stdio.h>

#define LED 11
			
#define NODE1 100
#define NODE2 200
#define NODE3 300


u8 arr[][9]={{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F},
           {0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F},{0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F},
		   {0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F},{0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x1F},
		   {0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F},{0x00,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F},
		   {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}};

s32 type=-1,node3=0,data=-1;

void interupt_0(void) __irq
{
	type++;
	data=0;
	delay_ms(50);
	EXTINT=1<<0;
	VICVectAddr=0;

}
void interupt_1(void) __irq
{
    data=1;
	type--;
	delay_ms(50);
	EXTINT=1<<1;
	VICVectAddr=0;
}
void interupt_2(void) __irq
{
	 node3^=1;
	 delay_ms(50);
	 EXTINT=1<<2;
	 VICVectAddr=0;
}


 CANF txFrame={0} ,rxFrame={0};

 int CAN1_IsRxBufferNotEmpty(void)
{
    // C1GSR Bit 0 (RBS) = 1 ? Receive buffer contains a message
    if (C1GSR & (1 << 0))
    {
        return 1; // Data is available
    }
    else
    {
        return 0; // Buffer is empty
    }
}


 void setFrame(u32 id, u32 data)
{
    txFrame.ID = id;              // Set CAN ID for Node 1
    txFrame.BFV.DLC = 4;          // Data length (4 bytes)
    txFrame.Data1 = data;
    CAN1_Tx(txFrame);             // Transmit the frame
}


void check_rec(void)
{
   CAN1_Rx(&rxFrame);
   if(rxFrame.ID==NODE1 || rxFrame.ID==NODE3)
   {	
		/*if(type>=8)
		{
		   cmd_LCD(0xC0);
		  // str_LCD("                  ");
		   str_LCD("WINDOW OPE");
		} 
		else if(type==1)
		{
		  cmd_LCD(0xC0);
		  str_LCD("WINDOW CLO");
		} */
	    cmd_LCD(0xC0);
		str_LCD("WINDOW");
		BuildCGRAM(arr[type],8);
		cmd_LCD(8);
   }
   else if(rxFrame.ID==NODE2 && rxFrame.Data1==500)
   {
       cmd_LCD(0x8B);
	   str_LCD("FOR");
	   cmd_LCD(0xCB);
	   str_LCD("        ");
   }
   else if(rxFrame.ID==NODE2 && rxFrame.Data1>=0)
   {
       cmd_LCD(0x8B);
	   str_LCD("REV");
	   cmd_LCD(0xCB);
	   U32LCD(rxFrame.Data1);
	   if(rxFrame.Data1<20)
	   IOSET0=1<<LED;
	   else
	   IOCLR0=1<<LED;
   }
}


signed char buf[16];
 
void sensor_data()
{
	  f32 temp;
	  ds18b20_start_convert();
					delay_ms(750); // Wait for conversion
					
					temp = ds18b20_read_temp();
					if(temp <= -999.0f) 
					{
							return;
					} 
					else 
					{
							
							// Fixed format to ensure proper display
							int whole = (int)temp;
							int decimal = (int)((temp - whole) * 100);
							if(decimal < 0) decimal = -decimal;
							sprintf((char*)buf, "Tem:%d.%01dC", whole, decimal);
					} 
}
 

int main()
{
   init_LCD();
   IODIR0|=1<<LED;
   cmd_LCD(0x80);
   str_LCD("CAN PROJECT");delay_s(1);cmd_LCD(0x01); 
   cmd_LCD(0x80);
   str_LCD("LCD INITILISED");delay_s(1);cmd_LCD(0x01);
   ini_INTERUPT();
   cmd_LCD(0x80);
   str_LCD("INTERR ENABLE");delay_s(1);cmd_LCD(0x01);
   Init_CAN1();
   cmd_LCD(0x80);
   str_LCD("CAN INITILISED");delay_s(1);cmd_LCD(0x01);
   while(1)
   {
            	sensor_data();
				cmd_LCD(0x80);
				str_LCD(buf);

	               if(data==0)
				   {
                         if(type==1)
		    	          setFrame(NODE1,1);
					     else if(type==2)
					     setFrame(NODE1,3);
						 	else if(type==3)
					     setFrame(NODE1,5);
						 	else if(type==4)
					     setFrame(NODE1,15);
						 	else if(type==5)
					     setFrame(NODE1,31);
						 	else if(type==6)
					     setFrame(NODE1,63);
						 	else if(type==7)
					     setFrame(NODE1,127);
						    else if(type==8)
						 setFrame(NODE1,255);
						 else if(type>8)
						   type=8;
							if(CAN1_IsRxBufferNotEmpty())
			                 check_rec();
							 else
							 {
									 //cmd_LCD(0x01);
									 /*cmd_LCD(0x80);
									 str_LCD("WINDOWNODE DISC");
									 delay_s(1);cmd_LCD(0x01);*/
							 }
				   }
		           else if(data==1)
				    { 
				     	if(type==1)
		    	         setFrame(NODE3,0);
					     else if(type==2)
					     setFrame(NODE3,3);
						 	else if(type==3)
					     setFrame(NODE3,5);
						 	else if(type==4)
					     setFrame(NODE3,15);
						 	else if(type==5)
					     setFrame(NODE3,31);
						 	else if(type==6)
					     setFrame(NODE3,63);
						 	else if(type==7)
					     setFrame(NODE3,127);
						    else if(type==8)
						 setFrame(NODE3,255);
						 else if(type<=0)
						   type=0;

						   	if(CAN1_IsRxBufferNotEmpty())
			                check_rec();
							else
							{
									 //cmd_LCD(0x01);
									 //cmd_LCD(0x80);
								//	 str_LCD("WINDOWNODE DISC");
								//	 delay_s(1);cmd_LCD(0x01);
							}
				    }
		           if(node3==0)
				    {
		               setFrame(NODE2,0XC3);
					   	if(CAN1_IsRxBufferNotEmpty())
			            check_rec(); 
						else
						{
								   //  cmd_LCD(0x01);
									 /*cmd_LCD(0x80);
									 str_LCD("DIRE NODE DISC");
									 delay_s(1);cmd_LCD(0x01);*/
						}
				    }      						
		           else if(node3==1)
				    {
						setFrame(NODE2,0XD4);
							if(CAN1_IsRxBufferNotEmpty())
			                check_rec();
							else
							{
									//cmd_LCD(0x01);
									 /*cmd_LCD(0x80);
									 str_LCD("DIRE NODE DISC");
									 delay_s(1);cmd_LCD(0x01); */
							}
					}

   }
}

