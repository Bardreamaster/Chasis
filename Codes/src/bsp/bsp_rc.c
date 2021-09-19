#include "bsp/bsp_rc.h"

#include "rtthread.h"

#define SBUS_RX_BUF_NUM 36u
#define NULL 0

#define RC_FRAME_LENGTH 18u

static void SBUS_TO_RC(volatile const uint8_t *sbus_buf, RC_raw_t *rc_raw);

static RC_raw_t rc_raw;
static uint8_t SBUS_rx_buf[2][SBUS_RX_BUF_NUM];

RC_raw_t *get_remote_control_raw(void)
{
        return &rc_raw;
}

void bsp_remote_control_init(void)
{
        bsp_rc_init(SBUS_rx_buf[0], SBUS_rx_buf[1], SBUS_RX_BUF_NUM);
}

void bsp_rc_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
        /* -------------- Enable Module Clock Source ----------------------------*/
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

        RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART6, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART6, DISABLE);

        GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6); //PB7  usart1 rx
                                                                  /* -------------- Configure GPIO ---------------------------------------*/
        {
                GPIO_InitTypeDef GPIO_InitStructure;
                USART_InitTypeDef USART_InitStructure;
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
                GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
                GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
                GPIO_Init(GPIOC, &GPIO_InitStructure);

                USART_DeInit(USART6);

                USART_InitStructure.USART_BaudRate = 100000;
                USART_InitStructure.USART_WordLength = USART_WordLength_8b;
                USART_InitStructure.USART_StopBits = USART_StopBits_1;
                USART_InitStructure.USART_Parity = USART_Parity_Even;
                USART_InitStructure.USART_Mode = USART_Mode_Rx;
                USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
                USART_Init(USART6, &USART_InitStructure);

                USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);

                USART_ClearFlag(USART6, USART_FLAG_IDLE);
                USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);

                USART_Cmd(USART6, ENABLE);
        }

        /* -------------- Configure NVIC ---------------------------------------*/
        {
                NVIC_InitTypeDef NVIC_InitStructure;
                NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
                NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RC_NVIC;
                NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStructure);
        }

        //DMA2 stream1 ch5  or (DMA2 stream2 ch5)    !!!!!!! P308 of the datasheet
        /* -------------- Configure DMA -----------------------------------------*/
        {
                DMA_InitTypeDef DMA_InitStructure;
                DMA_DeInit(DMA2_Stream1);

                DMA_InitStructure.DMA_Channel = DMA_Channel_5;
                DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART6->DR);
                DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
                DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
                DMA_InitStructure.DMA_BufferSize = dma_buf_num;
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
                DMA_Init(DMA2_Stream1, &DMA_InitStructure);
                DMA_DoubleBufferModeConfig(DMA2_Stream1, (uint32_t)rx2_buf, DMA_Memory_0);
                DMA_DoubleBufferModeCmd(DMA2_Stream1, ENABLE);
                DMA_Cmd(DMA2_Stream1, DISABLE); //Add a disable
                DMA_Cmd(DMA2_Stream1, ENABLE);
        }
}

void bsp_rc_disable(void)
{
        USART_Cmd(USART6, DISABLE);
}

void bsp_rc_enable(void)
{
        USART_Cmd(USART6, ENABLE);
}

void bsp_rc_restart(uint16_t dma_buf_num)
{
        USART_Cmd(USART6, DISABLE);
        DMA_Cmd(DMA2_Stream1, DISABLE);
        DMA_SetCurrDataCounter(DMA2_Stream1, dma_buf_num);

        USART_ClearFlag(USART6, USART_FLAG_IDLE);

        DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF2);
        DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF2);
        DMA_Cmd(DMA2_Stream1, ENABLE);
        USART_Cmd(USART6, ENABLE);
}

void USART6_IRQHandler(void)
{
        rt_interrupt_enter();
        if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
        {
                USART_ReceiveData(USART6);
        }
        else if (USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
        {
                static uint16_t this_time_rx_len = 0;
                USART_ReceiveData(USART6);

                if (DMA_GetCurrentMemoryTarget(DMA2_Stream1) == 0)
                {
                        DMA_Cmd(DMA2_Stream1, DISABLE);
                        this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream1);
                        DMA_SetCurrDataCounter(DMA2_Stream1, SBUS_RX_BUF_NUM);
                        DMA2_Stream1->CR |= DMA_SxCR_CT;

                        DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
                        DMA_Cmd(DMA2_Stream1, ENABLE);
                        if (this_time_rx_len == RC_FRAME_LENGTH)
                        {
                                SBUS_TO_RC(SBUS_rx_buf[0], &rc_raw);
                        }
                }
                else
                {
                        DMA_Cmd(DMA2_Stream1, DISABLE);
                        this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream1);
                        DMA_SetCurrDataCounter(DMA2_Stream1, SBUS_RX_BUF_NUM);
                        DMA2_Stream2->CR &= ~(DMA_SxCR_CT);

                        DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
                        DMA_Cmd(DMA2_Stream1, ENABLE);
                        if (this_time_rx_len == RC_FRAME_LENGTH)
                        {
                                SBUS_TO_RC(SBUS_rx_buf[1], &rc_raw);
                        }
                }
        }
        rt_interrupt_leave();
}

static void SBUS_TO_RC(volatile const uint8_t *sbus_buf, RC_raw_t *rc_raw)
{
        if (sbus_buf == NULL || rc_raw == NULL)
        {
                return;
        }

        rc_raw->rc.ch[0] = (sbus_buf[0] | (sbus_buf[1] << 8)) & 0x07ff;        //!< Channel 0
        rc_raw->rc.ch[1] = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff; //!< Channel 1
        rc_raw->rc.ch[2] = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) |          //!< Channel 2
                            (sbus_buf[4] << 10)) &
                           0x07ff;
        rc_raw->rc.ch[3] = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff; //!< Channel 3
        rc_raw->rc.s[0] = ((sbus_buf[5] >> 4) & 0x0003);                       //!< Switch left
        rc_raw->rc.s[1] = ((sbus_buf[5] >> 4) & 0x000C) >> 2;                  //!< Switch right
        rc_raw->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    //!< Mouse X axis
        rc_raw->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    //!< Mouse Y axis
        rc_raw->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  //!< Mouse Z axis
        rc_raw->mouse.press_l = sbus_buf[12];                                  //!< Mouse Left Is Press ?
        rc_raw->mouse.press_r = sbus_buf[13];                                  //!< Mouse Right Is Press ?
        rc_raw->key.v = sbus_buf[14] | (sbus_buf[15] << 8);                    //!< KeyBoard value
        rc_raw->rc.ch[4] = sbus_buf[16] | (sbus_buf[17] << 8);                 //NULL

        rc_raw->rc.ch[0] -= RC_CH_VALUE_OFFSET;
        rc_raw->rc.ch[1] -= RC_CH_VALUE_OFFSET;
        rc_raw->rc.ch[2] -= RC_CH_VALUE_OFFSET;
        rc_raw->rc.ch[3] -= RC_CH_VALUE_OFFSET;
        rc_raw->rc.ch[4] -= RC_CH_VALUE_OFFSET;
}
