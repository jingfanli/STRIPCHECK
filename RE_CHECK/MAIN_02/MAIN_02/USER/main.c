#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "APP.h"
#include "misc.h"
#include "timers.h"
#include "iic_slave.h"


TimerHandle_t I2c_ReadCallback_Handler;
extern TaskHandle_t StartTask_Handler;
extern TaskHandle_t ReceiveTask_Handler;
extern TaskHandle_t SendTask_Handler;
extern TaskHandle_t CheckTask_Handler;

extern QueueHandle_t Check_Queue;

extern QueueHandle_t Send_Queue;

extern QueueHandle_t Receive_Queue;




void I2c_ReadCallback(TimerHandle_t xTimer);

void NVIC_Configuration(void);
void RCC_Configuration(void);
/*#include "myiic.h"
************************************************
电阻检验主控板程序02
作者:李竞帆
微泰医疗有限公司

************************************************/

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_Configuration();
	NVIC_Configuration();
	IICSLAVE_Init();
	delay_init();	    					 
	//uart_init(115200);					
	//创建开始任务
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
    Check_Queue = xQueueCreate(Check_num,sizeof(u8));    
    Send_Queue = xQueueCreate(Send_num,sizeof(u8)); 
	Receive_Queue = xQueueCreate(Receive_num,sizeof(u8));
    xTaskCreate((TaskFunction_t )Receive_task,             
                (const char*    )"Receive_task",           
                (uint16_t       )Receive_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )Receive_TASK_PRIO,        
                (TaskHandle_t*  )&ReceiveTask_Handler);   
    xTaskCreate((TaskFunction_t )Send_task,     
                (const char*    )"Send_task",   
                (uint16_t       )Send_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Send_TASK_PRIO,
                (TaskHandle_t*  )&SendTask_Handler); 

    xTaskCreate((TaskFunction_t )Check_task,     
                (const char*    )"Check_task",   
                (uint16_t       )Check_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Check_TASK_PRIO,
                (TaskHandle_t*  )&CheckTask_Handler); 
								
	I2c_ReadCallback_Handler = xTimerCreate( (const char * ) "I2c_ReadTimer",
								(const TickType_t) 10,
								(const UBaseType_t) pdFALSE,
								(void *) 1,
								(TimerCallbackFunction_t) I2c_ReadCallback );

    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();            
}



void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =10;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void RCC_Configuration(void)
{
  
  ErrorStatus HSEStartUpStatus;

  RCC_DeInit();

  RCC_HSEConfig(RCC_HSE_ON);

  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
  RCC_HCLKConfig(RCC_SYSCLK_Div1);
  RCC_PCLK2Config(RCC_HCLK_Div1);
  RCC_PCLK1Config(RCC_HCLK_Div2);
  FLASH_SetLatency(FLASH_Latency_2);
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
  RCC_PLLCmd(ENABLE);
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  while(RCC_GetSYSCLKSource() != 0x08);
  }
}
void I2c_ReadCallback(TimerHandle_t xTimer)
{
	//IIC_Timeout();
}


