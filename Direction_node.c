#include"can.h"
#include"adc_defines.h"
#include"can_defines.h"
#include<LPC21xx.h>



#define NODE2 200 
CANF txFrame,rxFrame;


f32 Read_ADC(u8 chNo);
void Init_ADC(void);

 void setFrame(u32 id, u32 data)
{
    txFrame.ID = id;              // Set CAN ID for Node 1
    txFrame.BFV.DLC = 4;          // Data length (4 bytes)
    txFrame.Data1 = data;
    CAN1_Tx(txFrame);             // Transmit the frame
}


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

	u32 distance;

int main()
{
   Init_ADC();
   Init_CAN1();
     while(1)
      {
	          distance=Read_ADC(CH0);
			  if(CAN1_IsRxBufferNotEmpty())
			  {	 
			     CAN1_Rx(&rxFrame);

				 if(rxFrame.ID==NODE2 && rxFrame.Data1==0XC3)
				 {
					 setFrame(NODE2,500);
				 }
				 else if(rxFrame.ID==NODE2 && rxFrame.Data1==0XD4)
				 {
					  setFrame(NODE2,distance);
				 } 	                                                         
			  }
     }
}

