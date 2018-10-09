#include "APP.H"
#include "Receive_task.h"
#include "Send_task.h"
#include "Check_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "lib_queue.h"
#include "Iic_slave.h"
#include "Check_task.h"


uint8 I2C1_Buffer_Tx[I2C_SEND_BUFNUM];

TaskHandle_t StartTask_Handler;
TaskHandle_t ReceiveTask_Handler;
TaskHandle_t SendTask_Handler;
TaskHandle_t CheckTask_Handler;



QueueHandle_t Check_Queue;
QueueHandle_t Send_Queue;
QueueHandle_t Receive_Queue;


extern u16 Channel1_Res[48];
extern u16 Channel2_Res[48];
extern u16 Channel3_Res[48];
extern u16 Channel4_Res[48];
extern u16 Channel5_Res[48];
extern u16 Channel6_Res[48];
extern u16 Channel7_Res[12];






//task1ÈÎÎñº¯Êý
void Receive_task(void *pvParameters)
{
	uint8 Receiv_start=0;
    BaseType_t mesg_flag;

	//Recevie_init();
	while(1)
	{
	 mesg_flag = xQueueReceive(Receive_Queue,&Receiv_start,portMAX_DELAY);
	   if(mesg_flag==pdTRUE)
	   	{
	   		if(Receiv_start==1)
	   			{
	   				
	   			}
	   	}
	   else
	   	{
          vTaskDelay(200);       //ÑÓÊ±1s£¬Ò²¾ÍÊÇ1000¸öÊ±ÖÓ½ÚÅ
       }	
	}
}

//task2ÈÎÎñº¯Êý
void Send_task(void *pvParameters)
{
	uint8 err;
	uint8 value;
	uint16 Send_buffer[40];
	frame_object send_frame;
	send_init();

	while(1)
	{
		err=xQueueReceive(Send_Queue,&value,portMAX_DELAY);
		if(err==pdTRUE)
			{
				Frame_init(&send_frame);
				
				switch (value)
					{
						case 1: LibQueue_Memcpy((uint8*)Send_buffer,(uint8 *)Channel1_Res,sizeof(Channel1_Res));
							break;
						case 2:
								LibQueue_Memcpy((uint8*)Send_buffer,(uint8*)Channel1_Res,sizeof(Channel1_Res));
							break;
						case 3: 
								LibQueue_Memcpy((uint8*)Send_buffer,(uint8*)Channel1_Res,sizeof(Channel1_Res));
							break;
						case 4:
								LibQueue_Memcpy((uint8*)Send_buffer,(uint8*)Channel1_Res,sizeof(Channel1_Res));
							break;
						case 5:
								LibQueue_Memcpy((uint8*)Send_buffer,(uint8*)Channel1_Res,sizeof(Channel1_Res));
							break;
						case 6:
								LibQueue_Memcpy((uint8*)Send_buffer,(uint8*)Channel1_Res,sizeof(Channel1_Res));
							break;
						case 7:
								LibQueue_Memcpy((uint8*)Send_buffer,(uint8*)Channel1_Res,sizeof(Channel1_Res));
							break;

						default:
							LibQueue_Memcpy((uint8 *)Send_buffer,(void*)0,sizeof(Channel1_Res));
							break;
						
					}
				if(Send_buffer!=NULL)
					{
				Frame_Pack(&send_frame,(const uint8 *)Send_buffer,value,sizeof(Send_buffer));

				LibQueue_Memcpy(I2C1_Buffer_Tx,(uint8 *)&send_frame,sizeof(send_frame));
					}
				 
				
			}
		else 
			{
			}
		
        vTaskDelay(200);                           //ÑÓÊ±1s£¬Ò²¾ÍÊÇ1000¸öÊ±ÖÓ½ÚÅÄ	
	}
}


void Check_task(void *pvParameters)
{    

	uint8 check_state=0;
	BaseType_t mesg_flag;
	Check_init();
	//Adc_Init();
	while(1)
	{
	check_state=checkalldata();
	if(check_state!=0)
		{
			mesg_flag=xQueueSend(Send_Queue,&check_state,portMAX_DELAY);
			if(mesg_flag!=pdTRUE)
				{
					//Check_Error();
				}
			check_state=0;
		}
	
    vTaskDelay(200);                          	
	}
}



