#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "adc.h"
#include "Receive_task.h"
#include "Send_task.h"
#include "Task_control.h"
#include "Check_task.h"

555
/*#include "myiic.h"
************************************************
电阻检验主控板程序02
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
#define Send_TASK_PRIO		4
//任务堆栈大小	
#define Send_STK_SIZE 		512  
//任务句柄
TaskHandle_t SendTask_Handler;
//任务函数
void Send_task(void *pvParameters);


//任务优先级
#define Check_TASK_PRIO		4
//任务堆栈大小	
#define Check_STK_SIZE 		512  
//任务句柄
TaskHandle_t CheckTask_Handler;
//任务函数
void Check_task(void *pvParameters);




int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	RCC_Configuration();
	NVIC_Configuration();
	iic2_init();
	delay_init();	    					 
	uart_init(115200);					
	

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
    //创建Receive任务
    xTaskCreate((TaskFunction_t )Receive_task,             
                (const char*    )"Receive_task",           
                (uint16_t       )Receive_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )Receive_TASK_PRIO,        
                (TaskHandle_t*  )&ReceiveTask_Handler);   
    //创建Send任务
    xTaskCreate((TaskFunction_t )Send_task,     
                (const char*    )"Send_task",   
                (uint16_t       )Send_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Send_TASK_PRIO,
                (TaskHandle_t*  )&SendTask_Handler); 

	    //创建Check任务
    xTaskCreate((TaskFunction_t )Check_task,     
                (const char*    )"Check_task",   
                (uint16_t       )Check_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Check_TASK_PRIO,
                (TaskHandle_t*  )&CheckTask_Handler); 

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


void Check_task(void *pvParameters)
{    

	Check_init();
	
	while(1)
	{
		checkalldata();
        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =10;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA , &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB , &GPIO_InitStructure);

  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC , &GPIO_InitStructure);
  //AD通道GPIO配置

  GPIO_InitStructure.GPIO_Pin = ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA , &GPIO_InitStructure);

  

  
}

u8 iic2_init(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x55;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 200000;
	I2C_Init(I2C2,&I2C_InitStructure);
	I2C_ITConfig(I2C2,I2C_IT_EVT|I2C_IT_BUF,ENABLE);
	I2C_Cmd(I2C2,ENABLE);
}
void RCC_Configuration(void)
{
  /* 定义枚举类型变量 HSEStartUpStatus */
  ErrorStatus HSEStartUpStatus;
  /* 复位系统时钟设置*/
  RCC_DeInit();
  /* 开启 HSE*/
  RCC_HSEConfig(RCC_HSE_ON);
  /* 等待 HSE 起振并稳定*/
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  /* 判断 HSE 起是否振成功，是则进入 if()内部 */
  if(HSEStartUpStatus == SUCCESS)
  {
  /* 选择 HCLK（AHB）时钟源为 SYSCLK 1 分频 */
  RCC_HCLKConfig(RCC_SYSCLK_Div1);
  /* 选择 PCLK2 时钟源为 HCLK（AHB） 1 分频 */
  RCC_PCLK2Config(RCC_HCLK_Div1);
  /* 选择 PCLK1 时钟源为 HCLK（AHB） 2 分频 */RCC_PCLK1Config(RCC_HCLK_Div2);
  /* 设置 FLASH 延时周期数为 2 */
  FLASH_SetLatency(FLASH_Latency_2);
  /* 使能 FLASH 预取缓存 */
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
  /* 选择锁相环（PLL）时钟源为 HSE 1 分频， 倍频数为 9，则 PLL 输出频率为 8MHz
  * 9 = 72MHz */
  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
  /* 使能 PLL */
  RCC_PLLCmd(ENABLE);
  /* 等待 PLL 输出稳定 */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  /* 选择 SYSCLK 时钟源为 PLL */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  /* 等待 PLL 成为 SYSCLK 时钟源 */
  while(RCC_GetSYSCLKSource() != 0x08);
  }
  /* 打开 APB2 总线上的 GPIOA 时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB
  2Periph_USART1, ENABLE);
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1|RCC_APB1Periph_I2C2,ENABLE);

}


