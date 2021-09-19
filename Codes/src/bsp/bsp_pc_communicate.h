/**
  ****************************************************************
  * @file    bsp_pc_communicate.h
  * @author  Spaceman(2635575003@qq.com)
  * @date    2020-Jan-15
  ****************************************************************
  */

#ifndef PC_COMMUNICATE_H
#define PC_COMMUNICATE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f4xx.h"
#include "bsp/bsp_common_define.h"

#define BUFFER_SIZE 36
#define SOF_FLAG 0x99

#define SOF head[0]
#define Type head[1]
#define Length head[2]
#define CRC_8 head[3]

typedef enum PC_communicate_IdType{
// pc to F405
    AUTO_AIM_17 = 0x00,
    AUTO_AIM_42 = 0x01,
    
// F405 to pc
 FIRE_CONTROL = 0xFF
 
} PC_communicate_Id;


typedef __packed struct
{
    uint8_t buffer[2][36];
    uint8_t *p_buffer;
    uint8_t buffer_ptr_order;
}PC_communicate_buffer_t;

typedef __packed struct
{
    uint8_t head[4];
    uint8_t data[30];
    uint8_t CRC16[2];
}PC_communicate_frame_t;

void bsp_PC_communicate_init(void);
void pc_communicate_hook(void);
void p_buffer_return_to_zero(uint8_t p_buffer_order);
inline void p_buffer_move();
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
