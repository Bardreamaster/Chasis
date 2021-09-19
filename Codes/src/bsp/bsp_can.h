/**
  ****************************************************************
  * @file    bsp_can.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-13
  ****************************************************************
  */

#ifndef BSP_CAN_H_
#define BSP_CAN_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f4xx.h"
#include "bsp/bsp_common_define.h"

#define CHASSIS_CAN CAN1
#define GIMBAL_CAN CAN1

typedef enum CanMotorIdType{
    CAN_5T8_CMD_ID = 0x1FF,

    CAN_1T4_CMD_ID = 0x200,
	
    CAN_M1_ID = 0x201,
    CAN_M2_ID = 0x202,
    CAN_M3_ID = 0x203,
    CAN_M4_ID = 0x204,

    CAN_M5_ID = 0x205,
    CAN_M6_ID = 0x206,
    CAN_M7_ID = 0x207,
    CAN_M8_ID = 0x208,


} CanMoterId;

typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
} motor_feedback_t;

void CAN_MOTOR_FLG_INIT(CanMoterId id, CAN_TypeDef* can_device);

void CAN_MOTOR_CMD(CanMoterId id, CAN_TypeDef* can_device, int16_t spd);

void update_motor_feedback(motor_feedback_t* ptr, CanRxMsg* rx_message);
void update_gimbal_motor_feedback(motor_feedback_t* ptr, CanRxMsg* rx_message);

void can1_hook(CanRxMsg *p_rx_message);
void can2_hook(CanRxMsg *p_rx_message);

motor_feedback_t* get_p_motor_feedback(CanMoterId id, CAN_TypeDef* can_device);
void CAN_MOTOR_CMD_SEND(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, CAN_TypeDef* can, CanMoterId id);

uint8_t bsp_can1_init(uint8_t tsjw, uint8_t tbs2, uint8_t tbs1, uint16_t brp, uint8_t mode);
uint8_t bsp_can2_init(uint8_t tsjw, uint8_t tbs2, uint8_t tbs1, uint16_t brp, uint8_t mode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BSP_CAN_H_ */
