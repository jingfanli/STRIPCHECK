#ifndef __APP_H
#define __APP_H

#include "freertos.h"
#include "task.h"
#include "queue.h"



#include "Bsp_gpio.h"

#define START_TASK_PRIO		1
	
#define START_STK_SIZE 		128  

extern TaskHandle_t StartTask_Handler;

void start_task(void *pvParameters);


#define Receive_TASK_PRIO		2

#define Receive_STK_SIZE 		512  

extern TaskHandle_t ReceiveTask_Handler;

void Receive_task(void *pvParameters);


#define Send_TASK_PRIO		4
	
#define Send_STK_SIZE 		512  

extern TaskHandle_t SendTask_Handler;

void Send_task(void *pvParameters);



#define Check_TASK_PRIO		4

#define Check_STK_SIZE 		512  

extern TaskHandle_t CheckTask_Handler;

void Check_task(void *pvParameters);


#define Check_num         5       
#define Send_num					5
#define Receive_num				5

extern QueueHandle_t Check_Queue;

extern QueueHandle_t Send_Queue;

extern QueueHandle_t Receive_Queue;

#endif
