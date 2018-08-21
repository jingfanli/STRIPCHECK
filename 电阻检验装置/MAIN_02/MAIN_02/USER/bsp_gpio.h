#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "stm32f10x_gpio.h"
#include "type.h"

#define m_gpiocallbacknum   16;



typedef enum
{
  Gpio_pinset,
  Gpio_pullset,
  Gpio_pinread,
  Gpio_pininterrupt,
  Gpio_pincallback
}gpio_setconfig;

typedef void(*gpio_interrupt_callback)(void);

typedef struct
{
	gpio_interrupt_callback fp_gpio_interrupt;
}gpio_callback;


static gpio_callback m_gpiocallback[m_gpiocallbacknum]={0};


u8 Systemgpio_init(void);

	
u8 Gpio_setconfig
(
	u16* set_mode,
	u16* set_value
	u32* gpiobase
);





#endif