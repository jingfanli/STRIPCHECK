#include "Bsp_gpio.h"
#include "exti.h"


#define LED_B_GPIOPORT       GPIOB
#define LED_B_PGIOPIN        GPIO_Pin_12
#define LED_B_CLK            RCC_APB2Periph_GPIOB

#define LED_R_GPIOPORT       GPIOB
#define LED_R_PGIOPIN        GPIO_Pin_13
#define LED_R_CLK            RCC_APB2Periph_GPIOB

#define STRIP_PIN1_PORT          GPIOC
#define STRIP_PIN1_PIN 			 GPIO_Pin_7
#define STRIP_PIN1_CLK           RCC_APB2Periph_GPIOC

#define STRIP_PIN7_PORT          GPIOC
#define STRIP_PIN7_PIN 			 GPIO_Pin_8
#define STRIP_PIN7_CLK           RCC_APB2Periph_GPIOC

#define STRIP_PIN9_PORT          GPIOC
#define STRIP_PIN9_PIN 			 GPIO_Pin_9
#define STRIP_PIN9_CLK           RCC_APB2Periph_GPIOC

#define S0_PORT                  GPIOA
#define S0_PIN 					 GPIO_Pin_15
#define S0_CLK					 RCC_APB2Periph_GPIOA

#define S1_PORT                  GPIOC
#define S1_PIN 					 GPIO_Pin_10
#define S1_CLK					 RCC_APB2Periph_GPIOC
 
#define A0_PORT   				GPIOB
#define A0_PIN 					GPIO_Pin_4
#define A0_CLK					RCC_APB2Periph_GPIOB

#define A1_PORT   				GPIOB
#define A1_PIN 					GPIO_Pin_5
#define A1_CLK					RCC_APB2Periph_GPIOB

#define A2_PORT   				GPIOB
#define A2_PIN 					GPIO_Pin_6
#define A2_CLK					RCC_APB2Periph_GPIOB

#define P0_PORT  				GPIOB
#define P0_PIN 					GPIO_Pin_7
#define P0_CLK					RCC_APB2Periph_GPIOB

#define P1_PORT  				GPIOB
#define P1_PIN 					GPIO_Pin_8
#define P1_CLK					RCC_APB2Periph_GPIOB


static u8 Gpio_readport
(
	u32* Gpiobase,
	u16* value
);
static u8 Gpio_readpin
(
	u16* pinnum,
	u32* Gpiobase,
	u16* value
);

static u8 Gpio_writepin
(
	u16* pinnum,
	u32* Gpiobase,
	u16* value
);

static u8 Gpio_writeport
(
	u32* Gpiobase,
	u16* value
);

static u8 Gpio_setcallback
(
	const u8* value,
	u8* pinnum
);

static u8 Gpio_setinterrupt
(
	u8* pinnum,
	u32 gpiobase
);


u8 Systemgpio_init()
{
	GPIO_InitTypeDef gpio_configinit;
	RCC_APB2PeriphClockCmd
	(
		LED_B_CLK|LED_R_CLK|STRIP_PIN1_CLK|
		STRIP_PIN7_CLK|STRIP_PIN9_CLK|
		S0_CLK|S1_CLK|A0_CLK|A1_CLK|A2_CLK|P0_CLK|P1_CLK,
		ENABLE
	);
	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Pin=LED_B_PGIOPIN;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LED_B_GPIOPORT,gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Pin=LED_R_PGIOPIN;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LED_R_GPIOPORT,gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_configinit.GPIO_Pin=STRIP_PIN1_PIN;
	GPIO_Init(STRIP_PIN1_PORT,gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_configinit.GPIO_Pin=STRIP_PIN7_PIN;
	GPIO_Init(STRIP_PIN7_PIN,gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_configinit.GPIO_Pin=STRIP_PIN9_PIN;
	GPIO_Init(STRIP_PIN9_PIN,gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_configinit.GPIO_Pin=S0_PIN;
	GPIO_Init(S0_PORT,gpio_configinit);


	gpio_configinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_configinit.GPIO_Pin=S1_PIN;
	GPIO_Init(S1_PORT,gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_configinit.GPIO_Pin=A0_PIN;
	GPIO_Init(A0_PORT,gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_configinit.GPIO_Pin=A1_PIN;
	GPIO_Init(A1_PORT,gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_configinit.GPIO_Pin=A2_PIN;
	GPIO_Init(A2_PORT,gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_configinit.GPIO_Pin=P0_PIN;
	GPIO_Init(P0_PORT,gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_configinit.GPIO_Pin=P1_PIN;
	GPIO_Init(P1_PORT,gpio_configinit);

	
}

u8 Gpio_setconfig
(
	u16* set_mode,
	u16* set_value,
	u32 gpiobase,
	u32 RCC_APB2BASE
)
{
	uint16 mode;
	mode=*set_mode;
	RCC_APB2PeriphClockCmd
	(
		RCC_APB2BASE,
		ENABLE
	);
	switch (mode)
		{
			case Gpio_pinset:

				break;
			case Gpio_pullset:

				break;

			case Gpio_pinread:

				break;

			case Gpio_pininterrupt:

				break;

			case Gpio_pincallback

				break;
				
			default:
				return FUNCTION_FAIL;
		}
	return FUNCTION_OK
}


static u8 Gpio_readport
(
u32 * Gpiobase,
u16 * value
)
{
	uint16 gpio_value;
	gpio_value = (GPIO_TypeDef*)Gpiobase->IDR;

	*value=gpio_value;

	return FUNCTION_OK;
	
}

static u8 Gpio_readpin
(
 u16 * pinnum,
 u32 * Gpiobase,
 u16 * value
)
{
	uint16 gpio_value;
	uint16 pinnums;

	pinnums = *pinnum;
	gpio_value = (GPIO_TypeDef*)Gpiobase->IDR;
	gpio_value = gpio_value & pinnums;
	
	* value	= gpio_value;
	return FUNCTION_OK;

}

static u8 Gpio_writeport
(
	u32* Gpiobase,
	u16* value
)
{
	(GPIO_TypeDef*)Gpiobase->ODR = *value;
	return FUNCTION_OK;
}

static u8 Gpio_writepin
(
 u16 * pinnum,
 u32 * Gpiobase,
 u16 * value
)
{
	uint16 gpio_value;
	uint16 pinnums;
	gpio_value = *value;
	pinnums = *pinnum;
	gpio_value= gpio_value&pinnums;
	(GPIO_TypeDef*)Gpiobase->ODR = gpio_value;
	return FUNCTION_OK;
}


static u8 Gpio_setinterrupt
(
	u8* pinnum,
	u32 gpiobase
)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	
	
}

