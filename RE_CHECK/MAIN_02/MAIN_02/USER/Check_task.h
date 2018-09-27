#ifndef __CHECK_TASK_H
#define  __CHECK_TASK_H
#include "Bsp_gpio.h"
#include "sys.h"


typedef enum
{
	RE3AND8=0,
	RE2AND6=2,
	RE_COUNT
}RE_POS;

typedef enum
{
	RE_NUM1,
	RE_NUM2,
	RE_NUM3,
	RE_NUM4,
	RE_NUM5,
	RE_NUM6,
	RE_NUM7,
	RE_NUM8,
	RE_CHANNELCOUNT
}RE_CHANNEL;



typedef struct
{
 u8 RES_TEST;
 u16 RES_UR;
 u16 RES_LR;
 u16 RES_UL;
 u16 RES_LL;
}Test_point;

u8 Check_init(void);
u8 checkalldata(void);
u8 StartallchannelADCS(void);
u8 Adcchannel_clear(void);
u8 Adcvalue_INORDER(u8 num,u8 res_pos);
void gpio_wake_test(void);
void Check_Error(void);

#endif

