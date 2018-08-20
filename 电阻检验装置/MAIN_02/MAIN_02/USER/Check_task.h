#ifndef __CHECK_TASK_H
#define  __CHECK_TASK_H

typedef enum
{
	
	
}Point_value;

typedef struct
{
 u16 RES_TEST;
 u16 RES_UR;
 u16 RES_LR;
 u16 RES_UL;
 u16 RES_LL;

 u8  Test_point;
 
 
	
}Check_data;

u8 Check_init();
u8 checkalldata();
#endif