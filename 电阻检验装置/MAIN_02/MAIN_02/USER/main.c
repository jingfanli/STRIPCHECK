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
����������ذ����02
����:���
΢̩ҽ�����޹�˾

************************************************/

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define Receive_TASK_PRIO		2
//�����ջ��С	
#define Receive_STK_SIZE 		512  
//������
TaskHandle_t ReceiveTask_Handler;
//������
void Receive_task(void *pvParameters);

//�������ȼ�
#define Send_TASK_PRIO		4
//�����ջ��С	
#define Send_STK_SIZE 		512  
//������
TaskHandle_t SendTask_Handler;
//������
void Send_task(void *pvParameters);


//�������ȼ�
#define Check_TASK_PRIO		4
//�����ջ��С	
#define Check_STK_SIZE 		512  
//������
TaskHandle_t CheckTask_Handler;
//������
void Check_task(void *pvParameters);




int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	RCC_Configuration();
	NVIC_Configuration();
	iic2_init();
	delay_init();	    					 
	uart_init(115200);					
	

	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,           
                (const char*    )"start_task",         
                (uint16_t       )START_STK_SIZE,       
                (void*          )NULL,                 
                (UBaseType_t    )START_TASK_PRIO,       
                (TaskHandle_t*  )&StartTask_Handler);   
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����Receive����
    xTaskCreate((TaskFunction_t )Receive_task,             
                (const char*    )"Receive_task",           
                (uint16_t       )Receive_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )Receive_TASK_PRIO,        
                (TaskHandle_t*  )&ReceiveTask_Handler);   
    //����Send����
    xTaskCreate((TaskFunction_t )Send_task,     
                (const char*    )"Send_task",   
                (uint16_t       )Send_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Send_TASK_PRIO,
                (TaskHandle_t*  )&SendTask_Handler); 

	    //����Check����
    xTaskCreate((TaskFunction_t )Check_task,     
                (const char*    )"Check_task",   
                (uint16_t       )Check_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )Check_TASK_PRIO,
                (TaskHandle_t*  )&CheckTask_Handler); 

    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//task1������
void Receive_task(void *pvParameters)
{

	while(1)
	{

        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}

//task2������
void Send_task(void *pvParameters)
{

	while(1)
	{
		
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}


void Check_task(void *pvParameters)
{    

	Check_init();
	
	while(1)
	{
		checkalldata();
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
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
  //ADͨ��GPIO����

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
  /* ����ö�����ͱ��� HSEStartUpStatus */
  ErrorStatus HSEStartUpStatus;
  /* ��λϵͳʱ������*/
  RCC_DeInit();
  /* ���� HSE*/
  RCC_HSEConfig(RCC_HSE_ON);
  /* �ȴ� HSE �����ȶ�*/
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  /* �ж� HSE ���Ƿ���ɹ���������� if()�ڲ� */
  if(HSEStartUpStatus == SUCCESS)
  {
  /* ѡ�� HCLK��AHB��ʱ��ԴΪ SYSCLK 1 ��Ƶ */
  RCC_HCLKConfig(RCC_SYSCLK_Div1);
  /* ѡ�� PCLK2 ʱ��ԴΪ HCLK��AHB�� 1 ��Ƶ */
  RCC_PCLK2Config(RCC_HCLK_Div1);
  /* ѡ�� PCLK1 ʱ��ԴΪ HCLK��AHB�� 2 ��Ƶ */RCC_PCLK1Config(RCC_HCLK_Div2);
  /* ���� FLASH ��ʱ������Ϊ 2 */
  FLASH_SetLatency(FLASH_Latency_2);
  /* ʹ�� FLASH Ԥȡ���� */
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
  /* ѡ�����໷��PLL��ʱ��ԴΪ HSE 1 ��Ƶ�� ��Ƶ��Ϊ 9���� PLL ���Ƶ��Ϊ 8MHz
  * 9 = 72MHz */
  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
  /* ʹ�� PLL */
  RCC_PLLCmd(ENABLE);
  /* �ȴ� PLL ����ȶ� */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  /* ѡ�� SYSCLK ʱ��ԴΪ PLL */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  /* �ȴ� PLL ��Ϊ SYSCLK ʱ��Դ */
  while(RCC_GetSYSCLKSource() != 0x08);
  }
  /* �� APB2 �����ϵ� GPIOA ʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB
  2Periph_USART1, ENABLE);
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1|RCC_APB1Periph_I2C2,ENABLE);

}


