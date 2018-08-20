/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
static u16 TrCount=0;
static u8 Flag_RcvOK=0;
static u8 Rx_Idx=0;

static u8 Buffer_Rx[30]={0};

static u16 BUFFER_Tx[270]={0};

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
//void SVC_Handler(void)
//{
//}
 
void DebugMon_Handler(void)
{
}




void I2C1_EV_IRQHandler(void)
{
  __IO uint16_t SR1Register =0;
  __IO uint16_t SR2Register =0;


  SR1Register = I2C1->SR1;           // 通过读取 SR1/2 获取 IIC 状态
  SR2Register = I2C1->SR2;


  // 从机发送
  // 判断IIC是从机模式 - 最低位(MSL = 0)
  if((SR2Register & 0x0001) != 0x0001)
  {
    // ADDR：根据状态判断获取从机IIC地址成功
    if((SR1Register & 0x0002) == 0x0002)
    {
      // 清除标志，准备接收数据
      SR1Register = 0;
      SR2Register = 0;
 TrCount= 0; 
    }


//TxE：根据状态标志可以发送数据
if((SR1Register & 0x0080) == 0x0080)
{
 SR1Register = 0;
      SR2Register = 0;
 I2C1->DR =TrCount ++;
}

//STOPF：监测停止标志
if((SR1Register & 0x0010) == 0x0010)
{
 I2C1->CR1 |= CR1_PE_Set;
      SR1Register = 0;
      SR2Register = 0;
 TrCount= 5; 
}

//TIME_OUT
if((SR1Register & 0x4000) == 0x4000)
{
 I2C1->CR1 |= CR1_PE_Set;
 SR1Register = 0;
      SR2Register = 0;
}
  }  
  
  // IIC从机接收
  // 判断IIC是从机模式 - 最低位(MSL = 0)
  if((SR2Register &0x0001) != 0x0001)
  {
    // 收到主机发送的地址：(ADDR = 1: EV1) 
    if((SR1Register & 0x0002) == 0x0002)
    {
      // 清除标志，准备接受数据
      SR1Register = 0;
      SR2Register = 0;
      Rx_Idx = 0;
    }
    
    // 接收数据 (RXNE = 1: EV2)
    if((SR1Register & 0x0040) == 0x0040)
    {
      Buffer_Rx[Rx_Idx++] = I2C1->DR;
      SR1Register = 0;
      SR2Register = 0;
    }
    
    // 监测停止条件 (STOPF =1: EV4) 
    if(( SR1Register & 0x0010) == 0x0010)
    {
      I2C1->CR1 |= CR1_PE_Set;
      SR1Register = 0;
      SR2Register = 0;
      Flag_RcvOK = 1;                         
    }
  } 
}
  
//void PendSV_Handler(void)
//{
//}
// 
//void SysTick_Handler(void)
//{
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
