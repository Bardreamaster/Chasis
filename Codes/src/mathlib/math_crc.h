/**
  ****************************************************************
  * @file    math_crc.h
  * @author  Spaceman(2635575003@qq.com)
  * @date    2020-Jan-16
  ****************************************************************
  */
#ifndef MATH_CRC_H_
#define MATH_CRC_H_
#include <stdint.h>

uint8_t Get_CRC8_Check_Sum(uint8_t *pchMessage,unsigned int dwLength,uint8_t ucCRC8);
    /* 
    ** Descriptions: CRC8 Verify function 
    ** Input: Data to Verify,Stream length = Data + checksum 
    ** Output: True or False (CRC Verify Result) 
    */ 
uint32_t Verify_CRC8_Check_Sum(uint8_t *pchMessage, unsigned int dwLength);

    /* 
    ** Descriptions: append CRC8 to the end of data 
    ** Input: Data to CRC and append,Stream length = Data + checksum 
    ** Output: True or False (CRC Verify Result) 
    */
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength); 
    /* 
    ** Descriptions: CRC16 checksum function 
    ** Input: Data to check,Stream length, initialized checksum 
    ** Output: CRC checksum 
    */ 
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);

    /* 
    ** Descriptions: CRC16 Verify function 
    ** Input: Data to Verify,Stream length = Data + checksum 
    ** Output: True or False (CRC Verify Result) 
    */ 
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength) ;

    /* 
    ** Descriptions: append CRC16 to the end of data 
    ** Input: Data to CRC and append,Stream length = Data + checksum 
    ** Output: True or False (CRC Verify Result) 
    */ 
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);

#endif
