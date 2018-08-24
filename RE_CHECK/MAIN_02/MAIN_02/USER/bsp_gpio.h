#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H
#include "stm32f10x.h"
#include "type.h"


#define  TEST_ENABLE         1

typedef enum
{
  Gpio_Ain,
  Gpio_flout_in,
  Gpio_pullup_in,
  Gpio_pulldown_in,
  Gpio_out_pp,
  Gpio_out_od
	
}Gpio_ioconfig;

typedef enum
{
  Gpio_pinset,
  Gpio_out,
  Gpio_in,
  Gpio_pinread,
  Gpio_pininterrupt,
  Gpio_pincallback
}gpio_setconfig;

typedef void(*gpio_interrupt_callback)(void);

typedef struct
{
	gpio_interrupt_callback fp_gpio_interrupt;
}gpio_callback;





u8 Systemgpio_init(void);

	
u8 Gpio_setconfig
(
	u16 set_mode,
	u16* set_value,
	u16 PINPORT
);

u8 Gpio_interrupt
(uint8 pin);

#if TEST_ENABLE == 1
void  Gpio_test(void);
#endif
#endif
