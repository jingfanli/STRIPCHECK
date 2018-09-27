#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

typedef enum
{
	MAD1,
	MAD2,
	MAD3,
	MAD4,
	MAD5,
	MAD6,
	MAD7,
	MAD8,
	MAD9,
	MAD10,
	MAD11,
	MAD12,
	MAD13,
	MAD14,
	MAD_COUNT
	
}ADC_CHANNEL;

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
 
#endif 
