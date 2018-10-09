#ifndef _IIC_SLAVE_H
#define _IIC_SLAVE_H
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "bsp_iic.h"
#include "bsp_gpio.h"
#include "lib_queue.h"

#define I2C_SEND_BUFNUM    50
#define I2C_RECIVE_BUFNUM  30



void IICSLAVE_Init(void);

void I2C2_interrupt(void);

void I2C2_ER_INTERRUPT(void);


#endif
