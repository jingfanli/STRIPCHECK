#ifndef __CHECK_TASK_H
#define  __CHECK_TASK_H
#include "Bsp_gpio.h"



typedef struct
{
 u16 RES_TEST;
 u16 RES_UR;
 u16 RES_LR;
 u16 RES_UL;
 u16 RES_LL;

 u8  Test_point;
 
 
	
}Check_data;

u8 Check_init(void);
u8 checkalldata(void);
#endif

