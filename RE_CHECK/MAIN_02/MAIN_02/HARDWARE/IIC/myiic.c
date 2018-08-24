#include "myiic.h"
#include "delay.h"
#include "stm32f10x_i2c.h"


static u8 Start_flag;

static u8 Stop_flag;
	 
//��ʼ��IIC
void IIC1_Init(void)
{		
  GPIO_InitTypeDef GPIO_InitStructure;
	Start_flag=0;
	Stop_flag=0;
	
	//RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ�� 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
 
	IIC_SCL=1;
	IIC_SDA=1;

}
//����IIC��ʼ�ź�
void IIC1_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC1_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC1_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC1_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC1_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC1_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

/*
u16 IIC2_Write
(
	u16 ui_Address,
	const u8 *u8p_Data,
	u16 ui_Length
)

{
	u16 ui_PageLength;
	u16 m_u16_Timeout;

	while (ui_Length > 0)
	{
		m_u16_Timeout = 0;


		while ((I2C2->SR2 & I2C_SR2_BUSY) != 0)
		{
			m_u16_Timeout++;

			if (m_u16_Timeout > IIC2_TIME_OUT)
			{
				return FUNCTION_FAIL;
			}
		}


		I2C2->CR1 |= I2C_CR1_START;
		m_u16_Timeout = 0;


		while (((I2C2->SR1 & I2C_SR1_SB) != I2C_SR1_SB) ||
			((I2C2->SR2 & (I2C_SR2_MSL | I2C_SR2_BUSY)) != 
			(I2C_SR2_MSL | I2C_SR2_BUSY)))
		{
			m_u16_Timeout++;

			if (m_u16_Timeout > IIC2_TIME_OUT)
			{
				return FUNCTION_FAIL;
			}
		}


		I2C2->DR = EEPROM_DEVICE_ADDRESS & ~(0x01);
		m_u16_Timeout = 0;

	
		while (((I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_TXE)) != 
			(I2C_SR1_ADDR | I2C_SR1_TXE)) ||
			((I2C1->SR3 & (I2C_SR3_MSL | I2C_SR3_BUSY | I2C_SR3_TRA)) != 
			(I2C_SR3_MSL | I2C_SR3_BUSY | I2C_SR3_TRA)))
		{
			m_u16_Timeout++;

			if (m_u16_Timeout > IIC2_TIME_OUT)
			{
				return FUNCTION_FAIL;
			}
		}


		I2C1->DR = (uint8)ui_Address;
		m_u16_Timeout = 0;

	
		while (((I2C1->SR1 & (I2C_SR1_BTF | I2C_SR1_TXE)) != 
			(I2C_SR1_BTF | I2C_SR1_TXE)) ||
			((I2C1->SR3 & (I2C_SR3_MSL | I2C_SR3_BUSY | I2C_SR3_TRA)) != 
			(I2C_SR3_MSL | I2C_SR3_BUSY | I2C_SR3_TRA)))
		{
			m_u16_Timeout++;

			if (m_u16_Timeout > EEPROM_TIME_OUT)
			{
				return FUNCTION_FAIL;
			}
		}

		ui_Length -= ui_PageLength;
		ui_Address += ui_PageLength;

		while (ui_PageLength > 0)
		{
		
			I2C1->DR = *u8p_Data;
			m_u16_Timeout = 0;

			while (((I2C1->SR1 & (I2C_SR1_BTF | I2C_SR1_TXE)) != 
				(I2C_SR1_BTF | I2C_SR1_TXE)) ||
				((I2C1->SR3 & (I2C_SR3_MSL | I2C_SR3_BUSY | I2C_SR3_TRA)) != 
				(I2C_SR3_MSL | I2C_SR3_BUSY | I2C_SR3_TRA)))
			{
				m_u16_Timeout++;

				if (m_u16_Timeout > EEPROM_TIME_OUT)
				{
					return FUNCTION_FAIL;
				}
			}

			ui_PageLength--;
			u8p_Data++;
		}


		I2C1->CR2 |= I2C_CR2_STOP;

		if (ui_Length > EEPROM_PAGE_SIZE)
		{
			ui_PageLength = EEPROM_PAGE_SIZE;
		}
		else
		{
			ui_PageLength = ui_Length;
		}

		if (DrvEEPROM_PollWriteDone() != FUNCTION_OK)
		{
			return FUNCTION_FAIL;
		}
	}

	return FUNCTION_OK;
}

*/





















