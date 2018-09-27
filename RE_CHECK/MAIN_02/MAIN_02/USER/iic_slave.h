#ifndef _IIC_SLAVE_H
#define _IIC_SLAVE_H
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "bsp_iic.h"
#include "bsp_gpio.h"
#include "lib_queue.h"



u8 READ_salve_init(void);

u8 IIC_INTERRUPT(void);

u8 IIC_READdata_ISR(void);

u8 IIC_Timeout(void);

void I2c_ReadCallback(TimerHandle_t xTimer);

void IICSLAVE_Init(void);

u8 Comm_init(void);


#endif
