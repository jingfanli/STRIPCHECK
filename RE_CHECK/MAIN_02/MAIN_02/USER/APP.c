#include "APP.H"
#include "Receive_task.h"
#include "Send_task.h"
#include "Check_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


TaskHandle_t StartTask_Handler;
TaskHandle_t ReceiveTask_Handler;
TaskHandle_t SendTask_Handler;
TaskHandle_t CheckTask_Handler;





QueueHandle_t Check_Queue;
QueueHandle_t Send_Queue;
QueueHandle_t Receive_Queue;


//task1ÈÎÎñº¯Êı
void Receive_task(void *pvParameters)
{
	uint8 Receiv_start=0;
    BaseType_t mesg_flag;

	//TEST_LIB();
	//Recevie_init();
	while(1)
	{
	 mesg_flag = xQueueReceive(Check_Queue,&Receiv_start,portMAX_DELAY);
	   if(mesg_flag==pdTRUE)
	   	{
	      //Recevie_data();
	   	}
	   else
	   	{
          vTaskDelay(200);       //ÑÓÊ±1s£¬Ò²¾ÍÊÇ1000¸öÊ±ÖÓ½ÚÅ
       }	
	}
}

//task2ÈÎÎñº¯Êı
void Send_task(void *pvParameters)
{
	send_init();

	while(1)
	{
		
		//Send_data();
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
			mesg_flag=xQueueSend(Check_Queue,&check_state,(TickType_t)0);
			if(mesg_flag!=pdTRUE)
				{
					//Check_Error();
				}
			check_state=0;
		}
	
    vTaskDelay(200);                          	
	}
}



