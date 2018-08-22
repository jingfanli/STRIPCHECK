#include "Bsp_gpio.h"
#include "exti.h"


#define GPIO_PIN_COUNT 16
#define Pinmask    0xf0
#define Portmask   0x0f

#define Gpio_out_pp      0b0011
#define Gpio_out_od      0b0111
#define Gpio_AIN         0b0000
#define Gpio_IN_flout    0b0100
#define Gpio_IN_pullup	 0b1000

GPIO_PortSourceGPIOA
GPIO_PinSource0

static uint8 Gpio_InterruptPort[GPIO_PIN_COUNT] = {0}; 


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
{(1 << 0), (1 << 1), (1 << 2), (1 << 3), (1 << 4), (1 << 5), (1 << 6), (1 << 7),(1<<8,),
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
	u16* value

);

static u8 Gpio_setinterrupt
(
	uint16 port,
  	uint16 pin,
	FunctionalState NewState
);



u8 Gpio_setconfig
(
	u16* set_mode,
	u16* set_value,
	u16* PINPORT
)
{
	uint16 port;
	uint16 pin;
	uint16 mode;
	uint16 value;
	pin=(*PINPORT)&Pinmask;
	port=(*PINPORT)&Portmask;
	if(pin>15||port>3)
		return FUNCTION_FAIL;
	
	mode=*set_mode;
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
				return Gpio_writepin(port,pin,&value);
				else
				return Gpio_clearpin(port,pin,&value);
				break;

			case Gpio_out:
				GPIO_InitTypeDef GPIO_INITSTRUCT;
				switch (*set_value)
					{
						case Gpio_out_pp:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_Out_PP;
							GPIO_INITSTRUCT.GPIO_Pin=pin;
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);
							

							break;
						case Gpio_out_od:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_Out_OD;
							GPIO_INITSTRUCT.GPIO_Pin=pin;
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);

							break;
						case default:
							return FUNCTION_FAIL;
					}

				break;
			case Gpio_in:
				switch (* set_value)
					{
						case Gpio_Ain:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_AIN;
							GPIO_INITSTRUCT.GPIO_Pin=pin;
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);
							break;

						case Gpio_flout_in:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_IN_FLOATING;
							GPIO_INITSTRUCT.GPIO_Pin=pin;
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);

							break;


						case Gpio_pullup_in:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_IPU;
							GPIO_INITSTRUCT.GPIO_Pin=pin;
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);
							break;

						case Gpio_pulldown_in:
							GPIO_INITSTRUCT.GPIO_Mode = GPIO_Mode_IPD;
							GPIO_INITSTRUCT.GPIO_Pin=pin;
							GPIO_Init(Gpio_table[port],&GPIO_INITSTRUCT);
							break;

						case default:
							return FUNCTION_FAIL;
					}

				break;
			case Gpio_pinread:
				return Gpio_readpin(port,pin,set_value);
				break;

			case Gpio_pininterrupt:
				if(*set_value!=0)
				Gpio_InterruptPort[pin] = port;
				Gpio_setinterrupt(port,pin,ENABLE);
			
				else 
				Gpio_InterruptPort[pin] = 0;
				Gpio_setinterrupt(port,pin,DISABLE);
			
				break;

			case Gpio_pincallback
				Gpio_setcallback(port,pin,value);
				break;
				
			default:
				return FUNCTION_FAIL;
		}
	return FUNCTION_OK
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
	 Gpio_table[port]->BSRR | = 0xffff;
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
	 Gpio_table[port]->BSRR |= GPIO_PINTABL[pin];
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
	Gpio_table[port]->BRR &= ~GPIO_PINTABL[pin];
	else
		return FUNCTION_FAIL;
	return FUNCTION_OK;
}

static u8 Gpio_setinterrupt
(
	uint16 port,
  	uint16 pin,
	FunctionalState NewState
)
{


	NVIC_InitTypeDef NVIC_initstruct;
	EXTI_InitTypeDef EXTI_INITSTRUCT;
	NVIC_initstruct.NVIC_IRQChannel = EXTI0_IRQn|EXTI1_IRQn|EXTI2_IRQn|
									  EXTI3_IRQn|EXTI4_IRQn|EXTI9_5_IRQn|
									  EXTI9_5_IRQn|EXTI15_10_IRQn;
	
	NVIC_initstruct.NVIC_IRQChannelPreemptionPriority = 6;

	NVIC_initstruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_initstruct);

	GPIO_EXTILineConfig(GPIO_PORTSOURCETABLE[port],GPIO_PINSOURCETABLE[pin]);

	EXTI_INITSTRUCT.EXTI_Line = GPIO_PINTABL[pin];

	EXTI_INITSTRUCT.EXTI_Mode = EXTI_Mode_Interrupt;

	EXTI_INITSTRUCT.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_INITSTRUCT.EXTI_LineCmd = NewState;

	EXTI_Init(&EXTI_INITSTRUCT);

	return FUNCTION_OK;
	
	
	
}

static u8 Gpio_setcallback
(
	uint16 port,
	uint16 pin,
	u16* value

)
{
	if(port<4&&pin<16)
		{
			m_gpiocallback[pin] =(gpio_callback*)value;
		}
	else 
		return FUNCTION_FAIL;

	return FUNCTION_OK;
}


u8 Gpio_interrupt(uint8 pin)
{

	
	 EXTI->IMR &= ~Gpio_InterruptPort[pin];
	if((m_gpiocallback[pin]->fp_gpio_interrupt)!=0);
		{
		m_gpiocallback[pin]->fp_gpio_interrupt();

		//EXTI_ClearITPendingBit(GPIO_PINSOURCETABLE[pin]);
		}
	
	EXTI->IMR |= Gpio_InterruptPort[pin];
	
}

