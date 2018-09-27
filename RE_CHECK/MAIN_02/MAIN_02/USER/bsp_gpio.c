#include "Bsp_gpio.h"
#include "exti.h"
#include "stm32f10x_gpio.h"



#define  m_gpiocallbacknum   16
#define GPIO_PIN_COUNT 16
#define Pinmask    0xf0
#define Portmask   0x0f


  


static gpio_callback m_gpiocallback[m_gpiocallbacknum] = {0};

static uint8 Gpio_InterruptPort[GPIO_PIN_COUNT] = {0}; 

static uint8 EtiIRQchannel[]=
{
EXTI0_IRQn,EXTI1_IRQn,EXTI2_IRQn,EXTI3_IRQn,EXTI4_IRQn	
};


static uint8 GPIO_PORTSOURCETABLE[]=
{GPIO_PortSourceGPIOA,GPIO_PortSourceGPIOB,GPIO_PortSourceGPIOC,GPIO_PortSourceGPIOD};

static uint8 GPIO_PINSOURCETABLE[]=
{GPIO_PinSource0,GPIO_PinSource1,GPIO_PinSource2,GPIO_PinSource3,GPIO_PinSource4,
GPIO_PinSource5,GPIO_PinSource6,GPIO_PinSource7,GPIO_PinSource8,GPIO_PinSource9,
GPIO_PinSource10,GPIO_PinSource11,GPIO_PinSource12,GPIO_PinSource13,GPIO_PinSource14,
GPIO_PinSource15
};

static uint32 RCC_APB2Periph_GPIOTABLE[]=
{((uint32)0x00000004),((uint32)0x00000008), ((uint32)0x00000010),((uint32)0x00000020)};

static GPIO_TypeDef* Gpio_table[]=
{GPIOA,GPIOB,GPIOC,GPIOD};

static uint16 GPIO_PINTABL[]=
{(1 << 0), (1 << 1), (1 << 2), (1 << 3), (1 << 4), (1 << 5), (1 << 6), (1 << 7),(1<<8),
(1<<9),(1<<10),(1<<11),(1<<12),(1<<13),(1<<14),(1<<15)};

static u8 Gpio_readport
(
	uint16 port,
  uint16 pin,
  u16 * value
);
static u8 Gpio_readpin
(
	uint16 port,
	uint16 pin,
	u16 * value

);

static u8 Gpio_writepin
(
	uint16 port,
	uint16 pin,
	u16* value

);

static u8 Gpio_writeport
(
		uint16 port,
	uint16 pin,
	  u16* value

);

static u8 Gpio_clearport
(
  uint16 port,
  uint16 pin,
  u16* value
);


static u8 Gpio_clearpin
(
	uint16 port,
	uint16 pin,
 	u16 * value
);

static u8 Gpio_setcallback
(
	uint16 port,
	uint16 pin,
	const uint16* value

);

static u8 Gpio_setinterrupt
(
	uint16 port,
  	uint16 pin,
	uint16 Svalue
);



u8 Gpio_setconfig
(
	u16 set_mode,
	const uint16* set_value,
	u16 PINPORT
)
{
	uint16 port;
	uint16 pin;
	uint16 mode;
	uint16 value;
	GPIO_InitTypeDef GPIO_INITSTRUCT;
	pin=(PINPORT)&Pinmask;
	port=(PINPORT)&Portmask;
	pin=pin>>4;
	if(pin>15||port>3)
		return FUNCTION_FAIL;
	
	mode=set_mode;
	RCC_APB2PeriphClockCmd
	(
		RCC_APB2Periph_GPIOTABLE[port],
		ENABLE
	);
	
	switch (mode)
		{
			case Gpio_pinset:
				value=1;
				if(*set_value!=0)
				return Gpio_writepin(port,pin,(uint16*)&value);
				else
				return Gpio_clearpin(port,pin,(uint16*)&value);

			case Gpio_out:
				
				switch (*set_value)
					{
						case Gpio_out_pp:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_Out_PP;
							GPIO_INITSTRUCT.GPIO_Pin=GPIO_PINTABL[pin];//pin;
							GPIO_INITSTRUCT.GPIO_Speed=GPIO_Speed_50MHz;
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);
							

							break;
						case Gpio_out_od:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_Out_OD;
							GPIO_INITSTRUCT.GPIO_Pin=GPIO_PINTABL[pin];
							GPIO_INITSTRUCT.GPIO_Speed=GPIO_Speed_50MHz;
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);

							break;
						case Gpio_out_AF_pp:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_AF_PP;
							GPIO_INITSTRUCT.GPIO_Pin=GPIO_PINTABL[pin];
							GPIO_INITSTRUCT.GPIO_Speed=GPIO_Speed_50MHz;
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);
							break;
						case Gpio_out_AF_od:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_AF_OD;
							GPIO_INITSTRUCT.GPIO_Pin=GPIO_PINTABL[pin];
							GPIO_INITSTRUCT.GPIO_Speed=GPIO_Speed_50MHz;
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);
							break;

						
						default:
							return FUNCTION_FAIL;
					}

				break;
			case Gpio_in:
				switch (* set_value)
					{
						case Gpio_Ain:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_AIN;
							GPIO_INITSTRUCT.GPIO_Pin=GPIO_PINTABL[pin];
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);
							break;

						case Gpio_flout_in:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_IN_FLOATING;
							GPIO_INITSTRUCT.GPIO_Pin=GPIO_PINTABL[pin];
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);

							break;


						case Gpio_pullup_in:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_IPU;
							GPIO_INITSTRUCT.GPIO_Pin=GPIO_PINTABL[pin];
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);
							break;

						case Gpio_pulldown_in:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_IPD;
							GPIO_INITSTRUCT.GPIO_Pin=GPIO_PINTABL[pin];
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);
							break;

						default:
							return FUNCTION_FAIL;
					}

				break;
			case Gpio_pinread:
				return Gpio_readpin(port,pin,(uint16*)set_value);
				
			case Gpio_pininterrupt:
				if(*set_value!=0)
				{
				Gpio_InterruptPort[pin] = port;
				Gpio_setinterrupt(port,pin, *set_value);
				}
			
				else
				{					
				Gpio_InterruptPort[pin] = 0;
				Gpio_setinterrupt(port,pin,*set_value);
				}
			
				break;

			case Gpio_pincallback:
				Gpio_setcallback(port,pin,set_value);
				break;
				
			default:
				return FUNCTION_FAIL;
		}
	return FUNCTION_OK;
}


static u8 Gpio_readport
(
  uint16 port,
  uint16 pin,
  u16 * value
)
{
	

	uint16 gpio_value;
	gpio_value = Gpio_table[port]->IDR;

	*value=gpio_value;

	return FUNCTION_OK;
	
}

static u8 Gpio_readpin
(
    uint16 port,
  uint16 pin,
  u16 * value
)
{


	uint16 gpio_value;



	gpio_value = Gpio_table[port]->IDR;
	gpio_value = gpio_value &GPIO_PINTABL[pin];
	
	* value	= gpio_value;
	return FUNCTION_OK;

}

static u8 Gpio_writeport
(
	  uint16 port,
  uint16 pin,
	u16* value
)
{

	if(* value==1)
	 Gpio_table[port]->BSRR |= 0xffff;
	else
		return FUNCTION_FAIL;
	return FUNCTION_OK;
}

static u8 Gpio_writepin
(
    uint16 port,
  uint16 pin,
  u16 * value
)
{

	if(* value==1)
	 Gpio_table[port]->BSRR = GPIO_PINTABL[pin];
	else
	return FUNCTION_FAIL;

	return FUNCTION_OK;
}

static u8 Gpio_clearport
(
  uint16 port,
  uint16 pin,
  u16* value
)
{

	if(* value==1)
	  Gpio_table[port]->BRR &= 0;
	else
		return FUNCTION_FAIL;

	return FUNCTION_OK;
}


static u8 Gpio_clearpin
(
	uint16 port,
	uint16 pin,
 	u16 * value
)
{


	if(* value==1)
	Gpio_table[port]->BRR = GPIO_PINTABL[pin];
	else
		return FUNCTION_FAIL;
	return FUNCTION_OK;
}

static u8 Gpio_setinterrupt
(
	uint16 port,
  	uint16 pin,
	uint16 Svalue
)
{
	uint16 value;


	NVIC_InitTypeDef NVIC_initstruct;
	EXTI_InitTypeDef EXTI_INITSTRUCT;
	GPIO_InitTypeDef GPIO_INITSTRUCT;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	if(pin<5)
		
	NVIC_initstruct.NVIC_IRQChannel = EtiIRQchannel[pin];

	else if(pin<10)

	NVIC_initstruct.NVIC_IRQChannel = EXTI9_5_IRQn;

	else 
	
	NVIC_initstruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	
	NVIC_initstruct.NVIC_IRQChannelPreemptionPriority = 6;

	NVIC_initstruct.NVIC_IRQChannelCmd = ENABLE;
	

	NVIC_Init(&NVIC_initstruct);
	GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_INITSTRUCT.GPIO_Pin=GPIO_PINTABL[pin];

	GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);

	GPIO_EXTILineConfig(GPIO_PORTSOURCETABLE[port],GPIO_PINSOURCETABLE[pin]);

	EXTI_INITSTRUCT.EXTI_Line = GPIO_PINTABL[pin];

	EXTI_INITSTRUCT.EXTI_Mode = EXTI_Mode_Interrupt;

	if(Svalue==1)

	EXTI_INITSTRUCT.EXTI_Trigger = EXTI_Trigger_Rising;
	else if(Svalue == 2)
		EXTI_INITSTRUCT.EXTI_Trigger = EXTI_Trigger_Falling;
	else
		EXTI_INITSTRUCT.EXTI_Trigger = EXTI_Trigger_Rising_Falling;

	if(Svalue==0)

	EXTI_INITSTRUCT.EXTI_LineCmd = DISABLE;
	else
		
	EXTI_INITSTRUCT.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_INITSTRUCT);

	return FUNCTION_OK;
	
	
	
}

static u8 Gpio_setcallback
(
	uint16 port,
	uint16 pin,
	const uint16* value

)
{
	if(port<4&&pin<16)
		{
			m_gpiocallback[pin].fp_gpio_interrupt = ((gpio_callback*)value)->fp_gpio_interrupt;
		}
	else 
		return FUNCTION_FAIL;

	return FUNCTION_OK;
}


u8 Gpio_interrupt(uint8 pin)
{

	
	 EXTI->IMR &= ~Gpio_InterruptPort[pin];
	 if(EXTI_GetITStatus((1)<<pin))
	 	{
	if((m_gpiocallback[pin].fp_gpio_interrupt)!=0);
		{
		m_gpiocallback[pin].fp_gpio_interrupt();

		//EXTI_ClearITPendingBit(GPIO_PINSOURCETABLE[pin]);
		}
	 	}
	
	EXTI->IMR |= Gpio_InterruptPort[pin];
		
		return FUNCTION_OK;
	
}


#if TEST_ENABLE == 1
void Gpio_test(void)
{
	u16  set_value = Gpio_out_pp;
	Gpio_setconfig(Gpio_out,&set_value,0xA0);
	set_value = 1;
	Gpio_setconfig(Gpio_pinset,&set_value,0xA0);
	
	set_value = Gpio_out_pp;
	Gpio_setconfig(Gpio_out,&set_value,0xc1);
	set_value = 0;
	Gpio_setconfig(Gpio_pinset,&set_value,0xc1);		
}

#endif

