#include "Iic_slave.h"
#include "stm32f10x_i2c.h"



/* STM32 I2C 快速模式 */
#define I2C_Speed              400000  //*

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
  

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

  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =0x50; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	
	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* 通信速率 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C 初始化 */
  I2C_Init(I2C2, &I2C_InitStructure);
  
	/* 使能 I2C */
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
 
//事件中断处理函数 
 
{ 
 
 switch (I2C_GetLastEvent(I2C2))
 
 //获取i2c1的中断事件 
 
 { 
 
  /* Slave Transmitter ---------------------------------------------------*/  
 
 case I2C_EVENT_SLAVE_BYTE_TRANSMITTED: 
 
  /* 这个和下面那个都是从发送模式下发送数据的，具体两个的区别我也不是很明白，感觉就是移位寄存器空与非  空的区别,准备好数据发送吧 */ 
 
  I2C_SendData(I2C2, I2C1_Buffer_Tx[Tx_Idx++]);
 
   break; 
 
 case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:             /* EV3 */    
 
  /* Transmit I2C1 data */
 
  I2C_SendData(I2C2, I2C1_Buffer_Tx[Tx_Idx++]); 
 
   break; 
 
 /* Slave Receiver ------------------------------------------------------*/ 
 
 case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:     /* EV1 */ 
 
  /* 地址匹配中断，不管从发送和接收都要匹配地址，如下图244、243发送地址之后都会响应EV1 */ 
 
  break; 
 
 case I2C_EVENT_SLAVE_BYTE_RECEIVED:                /* EV2 */ 
 
  /* Store I2C1 received data */ 
 
  /* 这个中断就是响应EV2中断，如下图244，每次主机发送完一个数据就会产生一个EV2的中断 */ 
 
  I2C1_Buffer_Rx[Rx_Idx++] = I2C_ReceiveData(I2C1);
 
  /* 把接收到的中断填充到数组中 */ 
 
  /* 注意：地址不会填充进来的 */
 
   break; 
 
 case I2C_EVENT_SLAVE_STOP_DETECTED:                /* EV4 */
 
   /* Clear I2C1 STOPF flag */ 
 
  /* 这个就是正常停止的时候产生的一个停止信号 */ 
 
  I2C_Cmd(I2C2, ENABLE); 
 
  /* 我也不清楚这个为什么要这样，如果接收完一串数据之后，不响应主机的情况可以 关闭i2c，然后在处理完数据后再  从新配置i2c，记得是从新配置 */ 
 
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
 
  /* 这个就是图243中最后那个没有应答的中断，也就是发送了一串数据后的中断，可以做清零工作 */  
 
  I2C_ClearITPendingBit(I2C1, I2C_IT_AF); 
 
  Tx_Idx = 0; 
 
  //i2c_event = EVENT_OPCOD_NOTYET_READ; 
 
 } 
 
 /* Check on I2C1 AF flag and clear it */ 
 
 if (I2C_GetITStatus(I2C1, I2C_IT_BERR))   //这个就是起始和停止条件出错了
 
 { 
 
  I2C_ClearITPendingBit(I2C1, I2C_IT_BERR);
 
  }
 
 }


