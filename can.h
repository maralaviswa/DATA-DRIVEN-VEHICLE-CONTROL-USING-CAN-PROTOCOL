#include "type.h"
typedef struct CAN_Frame
{	
	u32 ID;
	struct BitField
	{
		u32 RTR : 1;
		u32 DLC : 4;
	}BFV;	
	s32 Data1,Data2;
	//u8 Data[8];
}CANF;
void Init_CAN1(void);
void CAN1_Tx(CANF txFrame);
void CAN1_Rx(CANF *rxFrame);




