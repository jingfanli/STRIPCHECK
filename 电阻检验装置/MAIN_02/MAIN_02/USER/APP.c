#include "APP.H"
#include "Receive_task.h"
#include "Send_task.h"
#include "Check_task.h"




//task1任务函数
void Receive_task(void *pvParameters)
{
	
	

	while(1)
	{
	    Recevie_init();
		Recevie_data();
        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}
}

//task2任务函数
void Send_task(void *pvParameters)
{
	
	while(1)
	{
		Send_init();
		Send_data();
        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}
}


void Check_task(void *pvParameters)
{    

	
	
	while(1)
	{
		Check_init();
		checkalldata();
        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}
}

