#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "string.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
#include "APP.h"


void NVIC_Configuration(void);
void RCC_Configuration(void);
/*#include "myiic.h"
************************************************
����������ذ����02
����:���
΢̩ҽ�����޹�˾

************************************************/

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	RCC_Configuration();
	NVIC_Configuration();
	//iic2_init();
	delay_init();	    					 
	//uart_init(115200);					
	

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

    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();            
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
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
	//RCC_APB2Periph_USART1, ENABLE);
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1|RCC_APB1Periph_I2C2,ENABLE);

}


