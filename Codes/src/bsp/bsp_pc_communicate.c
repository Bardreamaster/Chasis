#include "bsp/bsp_PC_communicate.h"
#include "rtthread.h"
#include "mathlib/math_crc.h"

static PC_communicate_buffer_t PC_communicate_buffer;
static PC_communicate_frame_t PC_communicate_frame;
static uint8_t Buffer_flag = 0; 

void bsp_PC_communicate_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    DMA_InitTypeDef   DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    RCC_APB2PeriphResetCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB1Periph_UART4, DISABLE);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_UART4);

    /* -------------- INIT BUFFER ---------------------------------------*/
    {
        PC_communicate_buffer.buffer_ptr_order=0;
        PC_communicate_buffer.p_buffer = &(PC_communicate_buffer.buffer[0][PC_communicate_buffer.buffer_ptr_order]);
    }

    /* -------------- Configure GPIO ---------------------------------------*/
     {
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
                GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
                GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
                GPIO_Init(GPIOA, &GPIO_InitStructure);

                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
                GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
                GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
                GPIO_Init(GPIOA, &GPIO_InitStructure);

                USART_DeInit(UART4);

                USART_InitStructure.USART_BaudRate = 115200;
                USART_InitStructure.USART_WordLength = USART_WordLength_8b;
                USART_InitStructure.USART_StopBits = USART_StopBits_1;
                USART_InitStructure.USART_Parity = USART_Parity_Odd;
                USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
                USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
                USART_Init(UART4, &USART_InitStructure);

                USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);

                USART_ClearFlag(UART4, USART_FLAG_IDLE);

                USART_Cmd(UART4, ENABLE);
        }

    /* -------------- Configure NVIC ---------------------------------------*/
   {
                NVIC_InitTypeDef NVIC_InitStructure;
                NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
                NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART4_NVIC;
                NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStructure);
    }

    /* -------------- Configure DMA -----------------------------------------*/
   {
                DMA_InitTypeDef DMA_InitStructure;
                DMA_DeInit(DMA1_Stream2);

                DMA_InitStructure.DMA_Channel = DMA_Channel_4;
                DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (UART4->DR);
                DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)PC_communicate_buffer.buffer[0];
                DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
                DMA_InitStructure.DMA_BufferSize = 36;
                DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
                DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
                DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
                DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
                DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
                DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
                DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
                DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
                DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
                DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
                DMA_Init(DMA1_Stream2, &DMA_InitStructure);
                DMA_DoubleBufferModeConfig(DMA1_Stream2, (uint32_t)PC_communicate_buffer.buffer[1], DMA_Memory_1);
                DMA_DoubleBufferModeCmd(DMA1_Stream2, ENABLE);

                DMA_Cmd(DMA1_Stream2, DISABLE); //Add a disable
                DMA_Cmd(DMA1_Stream2, ENABLE);
   }
}

void DMA1_Stream2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream2,DMA_IT_TCIF2)==SET)
	{
        pc_communicate_hook();
		DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA1_Stream2, DMA_IT_TCIF2);
	}
}

void pc_communicate_hook()
{   
    for(uint8_t i=0;i<(72-PC_communicate_buffer.buffer_ptr_order);)
    {
        
    }
}

void p_buffer_return_to_zero(uint8_t p_buffer_order)
{
    PC_communicate_buffer.buffer_ptr_order=0;
    PC_communicate_buffer.p_buffer = &(PC_communicate_buffer.buffer[p_buffer_order][PC_communicate_buffer.buffer_ptr_order]);
}

inline void p_buffer_move()
{
    if(PC_communicate_buffer.buffer_ptr_order<BUFFER_SIZE)
    {
        PC_communicate_buffer.p_buffer++;
        PC_communicate_buffer.buffer_ptr_order++;
    }
    else
    {
        PC_communicate_buffer.buffer_ptr_order = 0;
        Buffer_flag=(Buffer_flag+1)%2;
        PC_communicate_buffer.p_buffer=&PC_communicate_buffer.buffer[Buffer_flag][PC_communicate_buffer.buffer_ptr_order];
    }
}