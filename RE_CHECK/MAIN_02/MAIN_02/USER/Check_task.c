#include "Check_task.h"
#include "bsp_adc.h"
#include "bsp_dma.h"

//#include "adc.h"
//#include "Dma.h"


#define LED_B_GPIOPORT       GPIOB
#define LED_B_PGIOPIN        GPIO_Pin_12
#define LED_B_CLK            RCC_APB2Periph_GPIOB
#define LED_B                0xc1

#define LED_R_GPIOPORT       GPIOB
#define LED_R_PGIOPIN        GPIO_Pin_13
#define LED_R_CLK            RCC_APB2Periph_GPIOB
#define LED_R                0xd1

#define STRIP_PIN1_PORT          GPIOC
#define STRIP_PIN1_PIN 			 GPIO_Pin_7
#define STRIP_PIN1_CLK           RCC_APB2Periph_GPIOC
#define STRIP_PIN1	             0X72

#define STRIP_PIN7_PORT          GPIOC
#define STRIP_PIN7_PIN 			 GPIO_Pin_8
#define STRIP_PIN7_CLK           RCC_APB2Periph_GPIOC
#define STRIP_PIN7				 0X82


#define STRIP_PIN9_PORT          GPIOC
#define STRIP_PIN9_PIN 			 GPIO_Pin_9
#define STRIP_PIN9_CLK           RCC_APB2Periph_GPIOC
#define STRIP_PIN9   			 0X92

#define S0_PORT                  GPIOA
#define S0_PIN 					 GPIO_Pin_15
#define S0_CLK					 RCC_APB2Periph_GPIOA
#define S0					     0xf0


#define S1_PORT                  GPIOC
#define S1_PIN 					 GPIO_Pin_10
#define S1_CLK					 RCC_APB2Periph_GPIOC
#define S1					 	 0xa2
 
#define A0_PORT   				GPIOB
#define A0_PIN 					GPIO_Pin_4
#define A0_CLK					RCC_APB2Periph_GPIOB
#define A0						0X41

#define A1_PORT   				GPIOB
#define A1_PIN 					GPIO_Pin_5
#define A1_CLK					RCC_APB2Periph_GPIOB
#define A1   					0x51

#define A2_PORT   				GPIOB
#define A2_PIN 					GPIO_Pin_6
#define A2_CLK					RCC_APB2Periph_GPIOB
#define A2						0X61

#define P0_PORT  				GPIOB
#define P0_PIN 					GPIO_Pin_7
#define P0_CLK					RCC_APB2Periph_GPIOB
#define p0						0x71

#define P1_PORT  				GPIOB
#define P1_PIN 					GPIO_Pin_8
#define P1_CLK					RCC_APB2Periph_GPIOB
#define P1						0x81

#define AD_RES                  1000
#define Vref_value              1200
#define AD_MAX					4096


u16 DMA1_MEM_LEN;

static u16 RE_buffer[14]={0};
static u8  Re_flag[7]={0};



 u16 Channel_RECbuffer[40]={0};

u16 Channel1_Res[48]={0};
u16 Channel2_Res[48]={0};
u16 Channel3_Res[48]={0};
u16 Channel4_Res[48]={0};
u16 Channel5_Res[48]={0};
u16 Channel6_Res[48]={0};
u16 Channel7_Res[12]={0};

















static u16 RE_PINTABLE[MAD_COUNT]=
{0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x01,0x11,0x02,0x12,0x22,0x32,0x42};


static u8 Check_REsdata(void);

static u8 Check_PINSTAT(void);

static u8 Check_5REdata(void);
static u8 Check_oneunit(void);


static u8 Check_channelswitch(u8 channel);

static u8 Check_flag=0;

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
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Pin=LED_B_PGIOPIN;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LED_B_GPIOPORT,&gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Pin=LED_R_PGIOPIN;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LED_R_GPIOPORT,&gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_configinit.GPIO_Pin=STRIP_PIN1_PIN;
	GPIO_Init(STRIP_PIN1_PORT,&gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_configinit.GPIO_Pin=STRIP_PIN7_PIN;
	GPIO_Init(STRIP_PIN7_PORT,&gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_configinit.GPIO_Pin=STRIP_PIN9_PIN;
	GPIO_Init(STRIP_PIN9_PORT,&gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_configinit.GPIO_Pin=S0_PIN;
	GPIO_Init(S0_PORT,&gpio_configinit);


	gpio_configinit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_configinit.GPIO_Pin=S1_PIN;
	GPIO_Init(S1_PORT,&gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_configinit.GPIO_Pin=A0_PIN;
	GPIO_Init(A0_PORT,&gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_configinit.GPIO_Pin=A1_PIN;
	GPIO_Init(A1_PORT,&gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_configinit.GPIO_Pin=A2_PIN;
	GPIO_Init(A2_PORT,&gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_configinit.GPIO_Pin=P0_PIN;
	GPIO_Init(P0_PORT,&gpio_configinit);

	gpio_configinit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_configinit.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_configinit.GPIO_Pin=P1_PIN;
	GPIO_Init(P1_PORT,&gpio_configinit);

	DMA1_MEM_LEN=MAD_COUNT*sizeof(u16);

	
	
}

static u8 Check_gpio_init();

static u8 Check_gpio_init()
{
	uint16 set_value;
	uint8 i;

	set_value=Gpio_flout_in;
	for(i=0;i<MAD_COUNT;i++)
		{
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[i]);
		}
	Gpio_setconfig(Gpio_in,&set_value,STRIP_PIN1);
	Gpio_setconfig(Gpio_in,&set_value,STRIP_PIN7);
	Gpio_setconfig(Gpio_in,&set_value,STRIP_PIN9);

	set_value=Gpio_out_pp;
	Gpio_setconfig(Gpio_out,&set_value,P0_PIN);
	Gpio_setconfig(Gpio_out,&set_value,P1_PIN);

	set_value=0;
	Gpio_setconfig(Gpio_pinset,&set_value,P0_PIN);
	Gpio_setconfig(Gpio_pinset,&set_value,P1_PIN);
	

	
	
}


u8 Check_init()
{
	Systemgpio_init();

	Check_gpio_init();
	
}


u8 checkalldata()
{
	uint8 value;
	value=Check_REsdata();
	
	return value;
}

/*
static u8 Check_oneunit(void)
{
	uint8 err;
	Check_flag=1;
	if(Check_flag!=0)
		{
		//Check_channelswitch(channel);

	err= Check_REsdata();//一个条子上电阻的
	if(err==FUNCTION_FAIL)
		{
			return FUNCTION_FAIL;
		}
	err = Check_PINSTAT();//1 //7 9通断情况
	if(err==FUNCTION_FAIL)
		{
			return FUNCTION_FAIL;
		}
	        return FUNCTION_OK;
		}
	
	return FUNCTION_FAIL;
	
}
*/

void gpio_wake_test(void)	
{
	uint16 value=Gpio_flout_in;
	Gpio_setconfig(Gpio_in,&value,0x61);
	
	value=Gpio_out_pp;
	Gpio_setconfig(Gpio_out,&value,0x60);
	Gpio_setconfig(Gpio_out,&value,0x70);

	value=1;
	Gpio_setconfig(Gpio_out,&value,0x70);

	value = 0x55;
	Gpio_setconfig(Gpio_pinread,&value,0x60);

	
	

	if(value==0)
		{
			Gpio_setconfig(Gpio_out,&value,0x60);
			
		}
	else if(value==1)
		{
			Gpio_setconfig(Gpio_out,&value,0x60);
		}
	else
		{
		value=0;
			Gpio_setconfig(Gpio_out,&value,0x70);
		}
}
/*
static u8 Check_gpio_init()
{
	
}
*/

void Check_Error(void)
{
	uint16 set_value;
	set_value=Gpio_out_pp;
	Gpio_setconfig(Gpio_out,(const uint16 *)&set_value,LED_R);
	Gpio_setconfig(Gpio_out,(const uint16 *)&set_value,LED_B);
	set_value=0;
	Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,LED_B);
	set_value=1;
	Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,LED_R);	
}

void Check_ok(void)
{
	uint16 set_value;
	set_value=Gpio_out_pp;
	Gpio_setconfig(Gpio_out,(const uint16 *)&set_value,LED_R);
	Gpio_setconfig(Gpio_out,(const uint16 *)&set_value,LED_B);
	set_value=1;
	Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,LED_B);
	set_value=0;
	Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,LED_R);
}

void Check_normal(void)
{
	uint16 set_value;
	set_value=Gpio_out_pp;
	Gpio_setconfig(Gpio_out,(const uint16 *)&set_value,LED_R);
	Gpio_setconfig(Gpio_out,(const uint16 *)&set_value,LED_B);
	set_value=0;
	Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,LED_B);
	set_value=0;
	Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,LED_R);

}

static u8 Check_REsdata(void)
{
	uint8 value;

	value=Check_5REdata();
	if(value!=FUNCTION_OK)
		{
			return value;
		}

		return FUNCTION_OK;
}


static u8 Check_channelswitch(u8 channel)
{

	uint16 set_value;
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	set_value=Gpio_out_pp;
	Gpio_setconfig(Gpio_out,(const uint16 *)&set_value,A2);
	Gpio_setconfig(Gpio_out,(const uint16 *)&set_value,A1);
	
	
	Gpio_setconfig(Gpio_out,(const uint16 *)&set_value,A0);

	set_value=0;
	Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A0);
	Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A1);
	Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A2);
	
	switch(channel)
		{
			case 0:
				set_value=0;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A1);
	            Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A2);
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A0);
				break;
		
			case 1:
				set_value=1;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A0);
				set_value=0;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A1);
	            Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A2);
			break;
			case 2:
				set_value=1;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A1);
				set_value=0;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A0);
	            Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A2);
			break;
			case 3:
				set_value=1;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A1);
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A0);
				set_value=0;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A1);
	            
			break;
			case 4:
				set_value=1;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A2);
				set_value=0;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A0);
	            Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A1);
			break;
			case 5:
				set_value=1;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A2);
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A0);
				set_value=0;
				
	            Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A1);
			break;
			case 6:
				set_value=1;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A2);
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A1);
				set_value=0;
				
	            Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A0);
			break;
			case 7:
				set_value=1;
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A2);
				Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A1);
	            Gpio_setconfig(Gpio_pinset,(const uint16 *)&set_value,A0);
			break;
			default:
				return FUNCTION_FAIL;
				
		}

	return FUNCTION_OK;
}

static u8 Check_5REdata(void)
{
	uint16 set_value;
	uint8 i;
	uint8 stat;
	uint8 err;

	for(i=0;i<8;i++)
	{
	Check_channelswitch(i);
	set_value=Gpio_out_pp;
	Gpio_setconfig(Gpio_out,&set_value,p0);
	Gpio_setconfig(Gpio_out,&set_value,P1);

	Gpio_setconfig(Gpio_out,&set_value,S0);
	Gpio_setconfig(Gpio_out,&set_value,S1);
	
    //设置4,10脚为推挽输出
	set_value=0;
	Gpio_setconfig(Gpio_pinset,&set_value,p0);//导通4脚
	set_value=1;
	Gpio_setconfig(Gpio_pinset,&set_value,P1);//关断10脚相当于悬空状态
	
	set_value=Gpio_flout_in;
	Gpio_setconfig(Gpio_out,&set_value,STRIP_PIN1);
	Gpio_setconfig(Gpio_out,&set_value,STRIP_PIN7);
	Gpio_setconfig(Gpio_out,&set_value,STRIP_PIN9);




	//将4脚设置为电源，其余引脚全部设置为浮空。

	set_value=Gpio_Ain;
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD1]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD2]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD3]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD4]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD5]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD6]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD7]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD8]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD9]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD10]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD11]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD12]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD13]);
	Gpio_setconfig(Gpio_in,&set_value,RE_PINTABLE[MAD14]);
	set_value=1;
	Gpio_setconfig(Gpio_pinset,&set_value,S0);
	set_value=0;
	Gpio_setconfig(Gpio_pinset,&set_value,S1);
	StartallchannelADCS();
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)!=FUNCTION_OK);
	ADC_Cmd(ADC1, DISABLE);
	DMA_Cmd(DMA1_Channel1, DISABLE );
	Adcvalue_INORDER(i,RE3AND8);
	Adcchannel_clear();
	//选择3和8的ad值	

	set_value=0;
	Gpio_setconfig(Gpio_pinset,&set_value,S0);
	set_value=1;
	Gpio_setconfig(Gpio_pinset,&set_value,S1);
	StartallchannelADCS();
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)!=FUNCTION_OK);
	ADC_Cmd(ADC1, DISABLE);
	DMA_Cmd(DMA1_Channel1, DISABLE );
	Adcvalue_INORDER(i,RE2AND6);
	Adcchannel_clear();
	//选择2和6的ad值
	set_value=1;
	Gpio_setconfig(Gpio_pinset,&set_value,S0);
	set_value=0;
	Gpio_setconfig(Gpio_pinset,&set_value,S1);
	Gpio_setconfig(Gpio_pinset,&set_value,p0);//关断4脚
	set_value=0;
	Gpio_setconfig(Gpio_pinset,&set_value,P1);//导通10脚

	StartallchannelADCS();
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)!=FUNCTION_OK);
	ADC_Cmd(ADC1, DISABLE);
	DMA_Cmd(DMA1_Channel1, DISABLE );
	
	switch(i)
		{ 
			case 0:
				Channel1_Res[i*5+4] = RE_buffer[i*2+1];
				break;
			
			case 1:
				Channel2_Res[i*5+4] = RE_buffer[i*2+1];
				break;
			case 2:
				Channel3_Res[i*5+4] = RE_buffer[i*2+1];
				break;
			case 3:
				Channel4_Res[i*5+4] = RE_buffer[i*2+1];
				break;
			case 4:
				Channel5_Res[i*5+4] = RE_buffer[i*2+1];
				break;
			case 5:
				Channel6_Res[i*5+4] = RE_buffer[i*2+1];
				break;
			case 6:
				Channel7_Res[i*5+4] = RE_buffer[i*2+1];
				break;
			//case 7:
			//	Channel7_Res[i*5+4] = RE_buffer[i*2+1];
				//break;
			default:
				break;
		}
	//将base的电阻值放入

		Adcchannel_clear();
		
	
		}
	
	
	
		return FUNCTION_OK;
	
}
static u8 Check_REBASE(void)
{
	
}


u8 StartallchannelADCS(void)
{

	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	DMA1_MEM_LEN=MAD_COUNT;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);	 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)&RE_buffer[0],DMA1_MEM_LEN);
	MYDMA_Enable(DMA1_Channel1);
	ADC_DeInit(ADC1); 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	ADC_InitStructure.ADC_NbrOfChannel = 14;	
	ADC_Init(ADC1, &ADC_InitStructure);	 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 7, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 8, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 9, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 10, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 11, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 12, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 13, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 14, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1, ENABLE);		
	ADC_ResetCalibration(ADC1);	  
	while(ADC_GetResetCalibrationStatus(ADC1));	
	ADC_StartCalibration(ADC1);	 
	while(ADC_GetCalibrationStatus(ADC1));	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	

	

	return FUNCTION_OK;
	
}


u8 Adcchannel_clear(void)
{
	uint8 i;

	for(i=0;i<14;i++)
		{
			RE_buffer[i]=0;
		}

	return FUNCTION_OK;
}

u8 Adcvalue_INORDER(u8 num,u8 res_pos)
{
	uint8 i;
	i=0;

	Channel1_Res[num*5+res_pos]=RE_buffer[i];
	i++;
	Channel1_Res[num*5+res_pos+1]=RE_buffer[i];
	i++;
	Channel2_Res[num*5+res_pos]=RE_buffer[i];
	i++;
	Channel2_Res[num*5+res_pos+1]=RE_buffer[i];
	i++;
	Channel3_Res[num*5+res_pos]=RE_buffer[i];
	i++;
	Channel3_Res[num*5+res_pos+1]=RE_buffer[i];
	i++;
	Channel4_Res[num*5+res_pos]=RE_buffer[i];
	i++;
	Channel4_Res[num*5+res_pos+1]=RE_buffer[i];
	i++;
	Channel5_Res[num*5+res_pos]=RE_buffer[i];
	i++;
	Channel5_Res[num*5+res_pos+1]=RE_buffer[i];
	i++;
	Channel6_Res[num*5+res_pos]=RE_buffer[i];
	i++;
	Channel6_Res[num*5+res_pos+1]=RE_buffer[i];
	i++;
	Channel7_Res[num*5+res_pos]=RE_buffer[i];
	i++;
	Channel7_Res[num*5+res_pos+1]=RE_buffer[i];

	return FUNCTION_OK;	
}

/*
u8 RECACUTE_CHECK(void)
{
	u8 value;
	u8 i;
	u8 j;
	u16 VrefAD;
	u16 Advalue;
	u16 Revalue;
	

	i=0;

	VrefAD=Get_Adc_Average(ADC_Channel_17,10);

	for(j=0;j++;j<40)
	{
	Channel_RECbuffer[j]=Channel1_Res[j];
	
	}
	
	Advalue=Channel1_Res[i]；
	Revalue=(u16)((u32)(Vref_value*Advalue)/VrefAD);
	Revalue=(4096)*(AD_RES+)
	
	
	Channel1_Res[i]=Revalue;
	i++;

	Advalue=Channel1_Res[i]；
	Revalue=(u16)((u32)(Vref_value*Advalue)/VrefAD);
	Channel1_Res[i]=Revalue;
	

	
	
}
*/
