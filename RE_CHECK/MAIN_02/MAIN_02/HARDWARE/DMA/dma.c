#include "dma.h"

DMA_InitTypeDef DMA_InitStructure;

u16 DMA1_MEM_LEN;//±£´æDMAÃ¿´ÎÊı¾İ´«ËÍµÄ³¤¶È 	    
//DMA1µÄ¸÷Í¨µÀÅäÖÃ
//ÕâÀïµÄ´«ÊäĞÎÊ½ÊÇ¹Ì¶¨µÄ,ÕâµãÒª¸ù¾İ²»Í¬µÄÇé¿öÀ´ĞŞ¸Ä
//´Ó´æ´¢Æ÷->ÍâÉèÄ£Ê½/8Î»Êı¾İ¿í¶È/´æ´¢Æ÷ÔöÁ¿Ä£Ê½
//DMA_CHx:DMAÍ¨µÀCHx
//cpar:ÍâÉèµØÖ·
//cmar:´æ´¢Æ÷µØÖ·
//cndtr:Êı¾İ´«ÊäÁ¿ 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 csour,u32 cdest,u16 cnlenth)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//Ê¹ÄÜDMA´«Êä
	
    DMA_DeInit(DMA_CHx);   //½«DMAµÄÍ¨µÀ1¼Ä´æÆ÷ÖØÉèÎªÈ±Ê¡Öµ
	DMA1_MEM_LEN=cnlenth;
	DMA_InitStructure.DMA_PeripheralBaseAddr = csour;  //DMAÍâÉèADC»ùµØÖ·
	DMA_InitStructure.DMA_MemoryBaseAddr = cdest;  //DMAÄÚ´æ»ùµØÖ·
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //Êı¾İ´«Êä·½Ïò£¬´ÓÍâÉèµ½ÄÚ´æ
	DMA_InitStructure.DMA_BufferSize = cnlenth;  //DMAÍ¨µÀµÄDMA»º´æµÄ´óĞ¡
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //ÍâÉèµØÖ·¼Ä´æÆ÷²»±ä
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //ÄÚ´æµØÖ·¼Ä´æÆ÷µİÔö
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //Êı¾İ¿í¶ÈÎª16Î»
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //Êı¾İ¿í¶ÈÎª16Î»
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //¹¤×÷ÔÚÕı³£»º´æÄ£Ê½
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAÍ¨µÀ xÓµÓĞÖĞÓÅÏÈ¼¶ 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAÍ¨µÀxÃ»ÓĞÉèÖÃÎªÄÚ´æµ½ÄÚ´æ´«Êä
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //¸ù¾İDMA_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯DMAµÄÍ¨µÀUSART1_Tx_DMA_ChannelËù±êÊ¶µÄ¼Ä´æÆ÷
	  	
} 
//¿ªÆôÒ»´ÎDMA´«Êä
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //¹Ø±ÕÄÍ¨µÀ      
 	DMA_SetCurrDataCounter(DMA1_Channel4,DMA1_MEM_LEN);//DMAÍ¨µÀµÄDMA»º´æµÄ´óĞ¡
 	DMA_Cmd(DMA_CHx, ENABLE);  //Ê¹ÄÜ 
}	  

 

























