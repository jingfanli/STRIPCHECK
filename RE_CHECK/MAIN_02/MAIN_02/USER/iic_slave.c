#include "Iic_slave.h"
#include "stm32f10x_i2c.h"



/* STM32 I2C ����ģʽ */
#define I2C_Speed              400000  //*

/* �����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ */
  

#define I2Cx_OWN_ADDRESS7      0x50






#define I2C_SEND_BUFNUM    30
#define I2C_RECIVE_BUFNUM  30



static u8 Start_flag;
static u8 Stop_flag;
static 	uint8 Tx_Idx;
static  uint8  Rx_Idx;

static uint8 I2C1_Buffer_Tx[I2C_SEND_BUFNUM];

static uint8 I2C1_Buffer_Rx[I2C_RECIVE_BUFNUM];

static lib_queue_object iic_Requeue;

static void I2C_Mode_Configu(void);




//static uint8 I2c_buffer[I2C_BUFNUM];


void IICSLAVE_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	LibQueue_Initialize((lib_queue_object *)&iic_Requeue,(uint8 *) &I2c_buffer,I2C_BUFNUM);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE ); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	
	I2C_Mode_Configu();	
}



	



static void I2C_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 
  NVIC_InitTypeDef  NVIC_InitStructure;

  /* I2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =0x50; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	
	/* I2C��Ѱַģʽ */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* ͨ������ */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C ��ʼ�� */
  I2C_Init(I2C2, &I2C_InitStructure);
  
	/* ʹ�� I2C */
  I2C_Cmd(I2C2, ENABLE); 
	I2C_ITConfig(I2C2,I2C_IT_EVT,ENABLE);
	I2C_ITConfig(I2C2,I2C_IT_ERR,ENABLE);
	

	
 /* Configure and enable I2Cx event interrupt -------------------------------*/
     NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
 	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
	 
	 /* Configure and enable I2C1 error interrupt -------------------------------*/
	 NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	 NVIC_Init(&NVIC_InitStructure);

	 

	 
}


void I2C2_interrupt(void)     
 
//�¼��жϴ����� 
 
{ 
 
 switch (I2C_GetLastEvent(I2C2))
 
 //��ȡi2c1���ж��¼� 
 
 { 
 
  /* Slave Transmitter ---------------------------------------------------*/  
 
 case I2C_EVENT_SLAVE_BYTE_TRANSMITTED: 
 
  /* ����������Ǹ����Ǵӷ���ģʽ�·������ݵģ�����������������Ҳ���Ǻ����ף��о�������λ�Ĵ��������  �յ�����,׼�������ݷ��Ͱ� */ 
 
  I2C_SendData(I2C2, I2C1_Buffer_Tx[Tx_Idx++]);
 
   break; 
 
 case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:             /* EV3 */    
 
  /* Transmit I2C1 data */
 
  I2C_SendData(I2C2, I2C1_Buffer_Tx[Tx_Idx++]); 
 
   break; 
 
 /* Slave Receiver ------------------------------------------------------*/ 
 
 case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:     /* EV1 */ 
 
  /* ��ַƥ���жϣ����ܴӷ��ͺͽ��ն�Ҫƥ���ַ������ͼ244��243���͵�ַ֮�󶼻���ӦEV1 */ 
 
  break; 
 
 case I2C_EVENT_SLAVE_BYTE_RECEIVED:                /* EV2 */ 
 
  /* Store I2C1 received data */ 
 
  /* ����жϾ�����ӦEV2�жϣ�����ͼ244��ÿ������������һ�����ݾͻ����һ��EV2���ж� */ 
 
  I2C1_Buffer_Rx[Rx_Idx++] = I2C_ReceiveData(I2C1);
 
  /* �ѽ��յ����ж���䵽������ */ 
 
  /* ע�⣺��ַ������������ */
 
   break; 
 
 case I2C_EVENT_SLAVE_STOP_DETECTED:                /* EV4 */
 
   /* Clear I2C1 STOPF flag */ 
 
  /* �����������ֹͣ��ʱ�������һ��ֹͣ�ź� */ 
 
  I2C_Cmd(I2C2, ENABLE); 
 
  /* ��Ҳ��������ΪʲôҪ���������������һ������֮�󣬲���Ӧ������������� �ر�i2c��Ȼ���ڴ��������ݺ���  ��������i2c���ǵ��Ǵ������� */ 
 
  Rx_Idx=0; 
 
 // i2c_event = EVENT_OPCOD_NOTYET_READ; 
 
  break; 
 
 default: 
 
  break;    
 
  } 
 
} 
 
void I2C2_ER_INTERRUPT(void) 
 
{ 
 
 /* Check on I2C1 AF flag and clear it */ 
 
 if (I2C_GetITStatus(I2C1, I2C_IT_AF)) 
 
 { 
 
  /* �������ͼ243������Ǹ�û��Ӧ����жϣ�Ҳ���Ƿ�����һ�����ݺ���жϣ����������㹤�� */  
 
  I2C_ClearITPendingBit(I2C1, I2C_IT_AF); 
 
  Tx_Idx = 0; 
 
  //i2c_event = EVENT_OPCOD_NOTYET_READ; 
 
 } 
 
 /* Check on I2C1 AF flag and clear it */ 
 
 if (I2C_GetITStatus(I2C1, I2C_IT_BERR))   //���������ʼ��ֹͣ����������
 
 { 
 
  I2C_ClearITPendingBit(I2C1, I2C_IT_BERR);
 
  }
 
 }


