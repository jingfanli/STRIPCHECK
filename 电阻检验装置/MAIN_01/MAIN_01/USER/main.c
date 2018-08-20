#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "myiic.h"
/*
************************************************
电阻检验主控板程序
作者:李竞帆
微泰医疗有限公司

************************************************/

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define Receive_TASK_PRIO		2
//任务堆栈大小	
#define Receive_STK_SIZE 		512  
//任务句柄
TaskHandle_t ReceiveTask_Handler;
//任务函数
void Receive_task(void *pvParameters);

//任务优先级
#define Send_TASK_PRIO		3
//任务堆栈大小	
#define Send_STK_SIZE 		512  
//任务句柄
TaskHandle_t SendTask_Handler;
//任务函数
void Send_task(void *pvParameters);






int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				 
	uart_init(115200);					

	
    xTaskCreate((TaskFunction_t )start_task,            
                (const char*    )"start_task",          
                (uint16_t       )START_STK_SIZE,       
                (void*          )NULL,                  
                (UBaseType_t    )START_TASK_PRIO,       
                (TaskHandle_t*  )&StartTask_Handler);                
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建TASK1任务
    xTaskCreate((TaskFunction_t )Receive_task,             
                (const char*    )"Receive_task",           
                (uint16_t       )Receive_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )Receive_TASK_PRIO,        
                (TaskHandle_t*  )&ReceiveTask_Handler);   
    //创建TASK2任务
    xTaskCreate((TaskFunction_t )Send_task,     
                (const char*    )"Send_task",   
                (uint16_t       )Send_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Send_TASK_PRIO,
                (TaskHandle_t*  )&SendTask_Handler); 

    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//task1任务函数
void Receive_task(void *pvParameters)
{

	while(1)
	{

        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}
}

//task2任务函数
void Send_task(void *pvParameters)
{

	while(1)
	{

        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}
}





