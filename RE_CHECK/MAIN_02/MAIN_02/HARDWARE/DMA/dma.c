#include "bsp_dma.h"

DMA_InitTypeDef DMA_InitStructure;

extern  u16 DMA1_MEM_LEN;    

void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 csour,u32 cdest,u16 cnlenth)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	
    DMA_DeInit(DMA_CHx);  
	DMA1_MEM_LEN=cnlenth;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32)csour;  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32)cdest;  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
	DMA_InitStructure.DMA_BufferSize = cnlenth;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA_CHx, &DMA_InitStructure); 
	  	
} 
//开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE ); 
 	DMA_SetCurrDataCounter(DMA1_Channel4,DMA1_MEM_LEN);
 	DMA_Cmd(DMA_CHx, ENABLE); 
}	  

 

























