#include "Receive_task.h"
#include "Freertos.h"







#define com_lenth             150
#define Receive_lenth         150

extern u8 Com_buffer[com_lenth]={0};
static u8 Receive_buffer[Receive_lenth]={0};


static u8 Function_Receive_Com(void);
static u8 Function_Handle_que
(
	u8*datasource,
 	u8*datatdest,
	u16 lenth
 );


 


