#include "Check_task.h"
#include "adc.h"
#include "Dma.h"
#include "Bsp_gpio.h"

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

static u8 Check_gpio_init();
static u8 Check_oneunit
(
u16 channel,
u16* 
)


u8 Check_init()
{
  Adc_Init();
  Check_gpio_init();
}


u8 checkalldata()
{
	
}


static u8 Check_oneunit()
{
	
}


