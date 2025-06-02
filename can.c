#include <LPC21xx.h>
#include "type.h"
#include "MACROS.h"
#include "delay.h"
#include "can.h"
#include "can_defines.h"

void Init_CAN1(void)
{
    //cfg p0.25 as CAN1_RX pin(RD1)
    PINSEL1|=0x00040000;
    //Reset CAN1 controller
    SETBIT(C1MOD,RM_BIT);		
    //all received messages are accepted
	  CLRBIT(AFMR,AccOFF_BIT);
    SETBIT(AFMR,AccBP_BIT);             
	  //AFMR=((AFMR&~(3<<AccOFF_BIT))|(2<<AccOFF_BIT));
    //Set baud Rate for CAN
    C1BTR=BTR_LVAL;
   //Enable CAN1 controller
    CLRBIT(C1MOD,RM_BIT);					
}

void CAN1_Tx(CANF txFrame)
{		
   // Checking that the TX buffer is empty
   while(READBIT(C1GSR,TBS1_BIT)==0);
	//while(((C1GSR>>TBS1_BIT)&1)==0);
   // Cfg Tx ID
   C1TID1=txFrame.ID;
   // Cfg RTR & DLC	
   C1TFI1=(txFrame.BFV.RTR<<RTR_BIT)|
          (txFrame.BFV.DLC<<DLC_BITS);
    //Check whether D/R Frame to Transmit
   if(txFrame.BFV.RTR!=1)
   {	
     //if D Frame,wr to data transmit buffers
	   C1TDA1= txFrame.Data1; /*bytes 1-4 */
	   C1TDB1= txFrame.Data2; /*bytes 5-8 */
   }
   //Select Tx Buff 1 & Start Xmission
   C1CMR|=1<<STB1_BIT|1<<TR_BIT;
   //wait until tx complete
   while(READBIT(C1GSR,TCS1_BIT)==0); 
}

void CAN1_Rx(CANF *rxFrame)
{
  //wait for CAN frame recv status
  while(READBIT(C1GSR,RBS_BIT)==0);
  //read 11-bit CANid of recvd frame.
  rxFrame->ID=C1RID; 
  //& read & extract data/remote frame status
  rxFrame->BFV.RTR=(C1RFS>>RTR_BIT)&1;
  //& extract data length
  rxFrame->BFV.DLC=(C1RFS>>DLC_BITS)&0x0f;
  //check if recvd frame is data frame,
  if(rxFrame->BFV.RTR==0)
  {	
   //extract data bytes 1-4
   rxFrame->Data1=C1RDA;
   //extract data bytes 5-8
   rxFrame->Data2=C1RDB;
  }
  // Release receive buffer command
  SETBIT(C1CMR,RRB_BIT);    
}


