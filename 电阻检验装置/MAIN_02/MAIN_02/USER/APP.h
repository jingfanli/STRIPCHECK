#ifndef __APP_H
#define __APP_H

#define START_TASK_PRIO		1
	
#define START_STK_SIZE 		128  

TaskHandle_t StartTask_Handler;

void start_task(void *pvParameters);


#define Receive_TASK_PRIO		2

#define Receive_STK_SIZE 		512  

TaskHandle_t ReceiveTask_Handler;

void Receive_task(void *pvParameters);


#define Send_TASK_PRIO		4
	
#define Send_STK_SIZE 		512  

TaskHandle_t SendTask_Handler;

void Send_task(void *pvParameters);



#define Check_TASK_PRIO		4

#define Check_STK_SIZE 		512  

TaskHandle_t CheckTask_Handler;

void Check_task(void *pvParameters);


#define Check_num                
#define Send_num
#define Receive_num

QueueHandle_t Check_Queue;

QueueHandle_t Send_Queue;

QueueHandle_t Receive_Queue;

#endif