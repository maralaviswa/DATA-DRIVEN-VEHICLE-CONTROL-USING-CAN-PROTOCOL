#include "delay.h"
#include "can.h"
#include<lpc21xx.h>
#include"MACROS.h"

#define LEDS 0
#define NODE1 100
#define NODE3 300

CANF txFrame,rxFrame;

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
  ))#  }
}

 int type;
int main()
{
IODIR0=(255<<LEDS);
Init_CAN1();
         while(1)
         {
				  if(CAN1_IsRxBufferNotEmpty())
				  {	 
				     CAN1_Rx(&rxFrame);
					 type=rxFrame.Data1;
				
					 if(rxFrame.ID==NODE1)
					 {
						 WRITEBYTE(IOPIN0,LEDS,~type);
						 setFrame(NODE1,rxFrame.ID+=100);
					 }
					 else if(rxFrame.ID==NODE3)
					 {
						  WRITEBYTE(IOPIN0,LEDS,~type);
						  setFrame(NODE3,rxFrame.ID+=100);
					 } 	                                                         
				  }
         }
}

