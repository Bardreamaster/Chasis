#include "bsp/bsp_can.h"
#include "rtthread.h"

void update_motor_feedback(motor_feedback_t* ptr, CanRxMsg* rx_message)                                                     \
{                                                                                          
    (ptr)->last_ecd = (ptr)->ecd;                                                          
    (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);           
    (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]);     
    (ptr)->given_current = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]); 
    (ptr)->temperate = (rx_message)->Data[6];                                              
}

void update_gimbal_motor_feedback(motor_feedback_t* ptr, CanRxMsg* rx_message)                                                     \
{                                                                                          
    (ptr)->last_ecd = (ptr)->ecd;                                                          
    (ptr)->ecd = (uint16_t)((rx_message)->Data[0] << 8 | (rx_message)->Data[1]);           
    (ptr)->speed_rpm = (uint16_t)((rx_message)->Data[2] << 8 | (rx_message)->Data[3]);     
    (ptr)->given_current = (uint16_t)((rx_message)->Data[4] << 8 | (rx_message)->Data[5]); 
    (ptr)->temperate = (rx_message)->Data[6];                                              
}

static motor_feedback_t can1_motor[8], can2_motor[8];

static int16_t CAN1_CMD_BUF[8] = {0},
               CAN2_CMD_BUF[8] = {0};

static uint8_t CAN_MOTOR_CMD_FLG_INIT[2] = {0},
               CAN_MOTOR_CMD_FLG_SET[2] = {0};

const uint8_t CAN_MOTOR_CMD_FLG[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};


void can1_hook(CanRxMsg *p_rx_message)
{
    switch (p_rx_message->StdId)
    {
    case CAN_M5_ID:{
        update_gimbal_motor_feedback(&can1_motor[4], p_rx_message);
    }
    break;
    
    case CAN_M6_ID:{
        update_gimbal_motor_feedback(&can1_motor[5], p_rx_message);
    }
    break;

    case CAN_M7_ID:{
        update_motor_feedback(&can1_motor[6], p_rx_message);
    }
    break;

    case CAN_M1_ID:
    case CAN_M2_ID:
    case CAN_M3_ID:
    case CAN_M4_ID:{
        static int i = 0;
        i = p_rx_message->StdId - CAN_M1_ID;
        update_motor_feedback((can1_motor + i), p_rx_message);
    }
    break;

    default:
        break;
    }
}

void can2_hook(CanRxMsg *p_rx_message)
{

}

void CAN_MOTOR_FLG_INIT(CanMoterId id, CAN_TypeDef* can_device)
{
    if(can_device == CAN1){
        CAN_MOTOR_CMD_FLG_INIT[0] |= CAN_MOTOR_CMD_FLG[id - CAN_M1_ID];
    }
    else if(can_device == CAN2){
        CAN_MOTOR_CMD_FLG_INIT[1] |= CAN_MOTOR_CMD_FLG[id - CAN_M1_ID];
    }
}

void CAN_MOTOR_CMD(CanMoterId id, CAN_TypeDef* can_device, int16_t spd)
{
    if(can_device == CAN1){
        CAN1_CMD_BUF[id - CAN_M1_ID] = spd;
        CAN_MOTOR_CMD_FLG_SET[0] |= CAN_MOTOR_CMD_FLG[id - CAN_M1_ID];

        if(((CAN_MOTOR_CMD_FLG_SET[0] ^ CAN_MOTOR_CMD_FLG_INIT[0]) >> 4) == 0x00){
            CAN_MOTOR_CMD_SEND(CAN1_CMD_BUF[0],
                               CAN1_CMD_BUF[1],
                               CAN1_CMD_BUF[2],
                               CAN1_CMD_BUF[3],
                               CAN1,
                               CAN_1T4_CMD_ID);
            CAN_MOTOR_CMD_FLG_SET[0] &= 0x0F;
        }
				
				//rt_kprintf("%3x\n",(((CAN_MOTOR_CMD_FLG_SET[0] ^ CAN_MOTOR_CMD_FLG_INIT[0]) << 4)&0xFF));
        if((((CAN_MOTOR_CMD_FLG_SET[0] ^ CAN_MOTOR_CMD_FLG_INIT[0]) << 4)&0xFF) == 0x00){
            CAN_MOTOR_CMD_SEND(CAN1_CMD_BUF[4],
                               CAN1_CMD_BUF[5],
                               CAN1_CMD_BUF[6],
                               CAN1_CMD_BUF[7],
                               CAN1,
                               CAN_5T8_CMD_ID);
            CAN_MOTOR_CMD_FLG_SET[0] &= 0xF0;
        }

    }
    else if(can_device == CAN2){
        CAN2_CMD_BUF[id - CAN_M1_ID] = spd;
        CAN_MOTOR_CMD_FLG_SET[1] |= CAN_MOTOR_CMD_FLG[id - CAN_M1_ID];

        if(((CAN_MOTOR_CMD_FLG_SET[1] ^ CAN_MOTOR_CMD_FLG_INIT[1]) >> 4) == 0x00){
            CAN_MOTOR_CMD_SEND(CAN2_CMD_BUF[0],
                               CAN2_CMD_BUF[1],
                               CAN2_CMD_BUF[2],
                               CAN2_CMD_BUF[3],
                               CAN2,
                               CAN_1T4_CMD_ID);
            CAN_MOTOR_CMD_FLG_SET[1] &= 0x0F;
        }

        if(((CAN_MOTOR_CMD_FLG_SET[1] ^ CAN_MOTOR_CMD_FLG_INIT[1]) << 4) == 0x00){
            CAN_MOTOR_CMD_SEND(CAN2_CMD_BUF[4],
                               CAN2_CMD_BUF[5],
                               CAN2_CMD_BUF[6],
                               CAN2_CMD_BUF[7],
                               CAN2,
                               CAN_5T8_CMD_ID);
            CAN_MOTOR_CMD_FLG_SET[1] &= 0xF0;
        }
    }
}

motor_feedback_t* get_p_motor_feedback(CanMoterId id, CAN_TypeDef* can_device)
{
    if(can_device == CAN1){
        return &can1_motor[id - CAN_M1_ID];
    }
    else{
        return &can2_motor[id - CAN_M1_ID];
    }
}

void CAN1_RX0_IRQHandler(void)
{
    static CanRxMsg rx1_message;

    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx1_message);
        can1_hook(&rx1_message);
    }
}

void CAN2_RX0_IRQHandler(void)
{
    static CanRxMsg rx2_message;
    if (CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
        CAN_Receive(CAN2, CAN_FIFO0, &rx2_message);
        can2_hook(&rx2_message);
    }
}

void CAN_MOTOR_CMD_SEND(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, CAN_TypeDef* can, CanMoterId id)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = id;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
    TxMessage.Data[0] = motor1 >> 8;
    TxMessage.Data[1] = motor1;
    TxMessage.Data[2] = motor2 >> 8;
    TxMessage.Data[3] = motor2;
    TxMessage.Data[4] = motor3 >> 8;
    TxMessage.Data[5] = motor3;
    TxMessage.Data[6] = motor4 >> 8;
    TxMessage.Data[7] = motor4;

    CAN_Transmit(can, &TxMessage);
}

uint8_t bsp_can1_init(uint8_t tsjw, uint8_t tbs2, uint8_t tbs1, uint16_t brp, uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, DISABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = mode;
    CAN_InitStructure.CAN_SJW = tsjw;
    CAN_InitStructure.CAN_BS1 = tbs1;
    CAN_InitStructure.CAN_BS2 = tbs2;
    CAN_InitStructure.CAN_Prescaler = brp;
    CAN_Init(CAN1, &CAN_InitStructure);    

    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAN1_NVIC;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    return 0;
}
uint8_t bsp_can2_init(uint8_t tsjw, uint8_t tbs2, uint8_t tbs1, uint16_t brp, uint8_t mode)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, DISABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);

    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = mode;
    CAN_InitStructure.CAN_SJW = tsjw;
    CAN_InitStructure.CAN_BS1 = tbs1;
    CAN_InitStructure.CAN_BS2 = tbs2;
    CAN_InitStructure.CAN_Prescaler = brp;
    CAN_Init(CAN2, &CAN_InitStructure);

    CAN_FilterInitStructure.CAN_FilterNumber = 14;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAN2_NVIC;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    return 0;
}
