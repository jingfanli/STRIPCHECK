#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
   	   		   
//IO方向设置
#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}

//IO操作函数	 
#define IIC_SCL    PCout(12) //SCL
#define IIC_SDA    PCout(11) //SDA	 
#define READ_SDA   PCin(11)  //输入SDA 
#define IIC2_TIME_OUT 20000

//IIC所有操作函数
void IIC1_Init(void);                //初始化IIC的IO口				 
void IIC1_Start(void);				//发送IIC开始信号
void IIC1_Stop(void);	  			//发送IIC停止信号
void IIC1_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC1_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC1_Wait_Ack(void); 				//IIC等待ACK信号
void IIC1_Ack(void);					//IIC发送ACK信号
void IIC1_NAck(void);				//IIC不发送ACK信号

void IIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC1_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















