#ifndef __SEND_TASK_H
#define __SEND_TASK_H
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


typedef struct
{
	uint8 HEADER1;
	uint8 ADDERESS0;
	uint8 ADDERESS1;
	uint8 HEADER2;
	uint8 CONTROLCODE;
	uint8 LEN;
	uint8 CODE1;
	uint8 CODE2;
	uint8* DATA;
	uint8 checksum;
	uint8 ETX;
	

} frame_object;



u8 send_init(void);

u8 send_process(void);

uint8 Frame_Pack
(
	frame_object *tp_Frame,
	const uint8 *u8p_Source,
	uint8 channel,
	uint8 tp_Length
);

void Frame_init(frame_object *tp_Frame);




#endif
