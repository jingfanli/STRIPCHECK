#include "APP.H"
#include "Receive_task.h"
#include "Send_task.h"
#include "Check_task.h"

TaskHandle_t StartTask_Handler;
TaskHandle_t ReceiveTask_Handler;
TaskHandle_t SendTask_Handler;
TaskHandle_t CheckTask_Handler;

QueueHandle_t Check_Queue;
QueueHandle_t Send_Queue;
QueueHandle_t Receive_Queue;





//task1������
void Receive_task(void *pvParameters)
{
	
	

	while(1)
	{
	    //Recevie_init();
		//Recevie_data();
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}

//task2������
void Send_task(void *pvParameters)
{
	
	while(1)
	{
		//Send_init();
		//Send_data();
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}


void Check_task(void *pvParameters)
{    

	
	
	while(1)
	{
		//Check_init();
		//checkalldata();
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}

