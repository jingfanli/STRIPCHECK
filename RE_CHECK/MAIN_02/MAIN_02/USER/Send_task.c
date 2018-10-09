#include "bsp_iic.h"
#include "bsp_gpio.h"
#include "lib_queue.h"
#include "lib_string.h"
#include "Send_task.h"
#include "lib_checksum.h"






void Frame_init(frame_object *tp_Frame)
{
	tp_Frame->checksum=0xffff;
	tp_Frame->ADDERESS0=0xaa;
	tp_Frame->ADDERESS1=0;
	tp_Frame->LEN=0;
	tp_Frame->CODE1=0;
	tp_Frame->CODE2=0;
	tp_Frame->CONTROLCODE=0;
	tp_Frame->ETX=0;
	
}


uint8 Frame_Pack
(
	frame_object *tp_Frame,
	const uint8 *u8p_Source,
	uint8 channel,
	uint8 tp_Length
)
{

	uint8 i;
	uint8 len;
	uint8 control;
	uint8 code1;
	uint8 code2;
	uint8 length;

	length=tp_Length;

	for(i=0;i<length;i++)
		{
			*(tp_Frame->DATA+i) = *(u8p_Source+i);
		}
	tp_Frame->checksum=LibChecksum_GetChecksum16Bit(tp_Frame->DATA,length);

	tp_Frame->ADDERESS0=0xaa;
	tp_Frame->ADDERESS1=channel;
	tp_Frame->LEN=length+2;
	tp_Frame->CODE1=0x52;
	tp_Frame->CODE2=0x14;
	tp_Frame->CONTROLCODE=Send_control+0x80;
	tp_Frame->ETX=0x16;


	return FUNCTION_OK;

	
}




void TEST_LIB(void)
{
	lib_queue_object t_Queue;
	uint8 u8p_buffer[19]={0};
	uint8 u8p_data1[5]={1,2,3,4,5};
	uint8 u8p_data2[5]={6,7,8,9,10};
	uint8 u8p_data3[5]={11,12,13,14,15};
	uint8 u8p_data4[5]={16,17,18,19,20};


	uint8 u8p_data5[10]={0};
	uint length;
	
	LibQueue_Initialize((lib_queue_object*) &t_Queue,(uint8 *)&u8p_buffer,17);
	length=sizeof(u8p_data1);

	LibQueue_PushHead(&t_Queue,(const uint8 *)&u8p_data1,&length);
	length=sizeof(u8p_data2);

	LibQueue_PushHead(&t_Queue,(const uint8 *)&u8p_data2,&length);
	length=sizeof(u8p_data3);

	LibQueue_PushHead(&t_Queue,(const uint8 *)&u8p_data3,&length);
	length=sizeof(u8p_data4);

	LibQueue_PushHead(&t_Queue,(const uint8 *)&u8p_data4,&length);

	length=sizeof(u8p_data5);
	LibQueue_PopHead(&t_Queue,( uint8 *)&u8p_data5,&length);

	
}

u8 send_init(void)
{}

