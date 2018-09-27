#include "Iic_slave.h"

#define I2C_BUFNUM    30



static u8 Start_flag;
static u8 Stop_flag;
static 	uint8 iic_data;
static  uint8 iic_count;

static lib_queue_object iic_Requeue;


static uint8 I2c_buffer[I2C_BUFNUM];

u8 READ_salve_init(void)
{
	u16 value;
	gpio_callback tp_callback;
	//IICSLAVE_Init();
	value=0;
	Gpio_setconfig(Gpio_pininterrupt,&value,IIC_SDAPORT);

	value=1;
	Gpio_setconfig(Gpio_pininterrupt,&value,IIC_SDAPORT);
	tp_callback.fp_gpio_interrupt=IIC_INTERRUPT;
	Gpio_setconfig(Gpio_pincallback,(const uint16 *)&value,IIC_SDAPORT);
	
}

u8 IIC_INTERRUPT(void)
{
	uint16 value;
	gpio_callback tp_callback;
	if(IIC_SCL==1)
		{
			if(IIC_SDA==0)
				{
				Start_flag=1;
				value=2;//falling
				Gpio_setconfig(Gpio_pininterrupt,&value,IIC_SDAPORT);
				tp_callback.fp_gpio_interrupt=IIC_READdata_ISR;
				Gpio_setconfig(Gpio_pincallback,(const uint16 *)&value,IIC_SCLPORT);
				}
		}
	else
		{
			if(IIC_SDA==1)
				{
					Stop_flag=1;
				}
		}

	return 1;

}

u8 IIC_READdata_ISR(void)
{
	uint lenth;

	if(Start_flag==1)
		{
			iic_count=0;
			iic_data=0;
			Start_flag=0;
		}
	if(IIC_SCL==0)
		{
			if(iic_count<8)
				{
			if(IIC_SDA=1)
				{
				iic_data++;
				}
			iic_data<<=1;
			iic_count++;
				}
		}

	

	if(iic_count==8)
		{
			
			iic_count=0;
			lenth=1
			LibQueue_PushHead((lib_queue_object *)&iic_Requeue,(uint8 *) &iic_data,&lenth);
		}
}


u8 IIC_Timeout(void)
{
	uint16 value;

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	value=0;
	Gpio_setconfig(Gpio_pininterrupt,&value,IIC_SCLPORT);
	value=2;
	Gpio_setconfig(Gpio_pininterrupt,&value,IIC_SDAPORT);
	LibQueue_Initialize((lib_queue_object *)&iic_Requeue,(uint8 *) &I2c_buffer,I2C_BUFNUM);
	iic_count=0;
	iic_data=0;
	Start_flag=0;
	Stop_flag=0;

	return 1;
}





u8 Comm_init(void)
{
	u16 value;
	gpio_callback tp_callback;
	Start_flag=0;
	Stop_flag=0;
	iic_data=0;
	iic_count=0;
	IICSLAVE_Init();
	value=2;//falling
	Gpio_setconfig(Gpio_pininterrupt,&value,IIC_SDAPORT);
	tp_callback.fp_gpio_interrupt=IIC_INTERRUPT;
	Gpio_setconfig(Gpio_pincallback,(const uint16 *)&value,IIC_SDAPORT);
	
}


void IICSLAVE_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	LibQueue_Initialize((lib_queue_object *)&iic_Requeue,(uint8 *) &I2c_buffer,I2C_BUFNUM);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE ); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}


