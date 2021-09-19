/**
  ****************************************************************
  * @file    util.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-11
  ****************************************************************
  */

#ifndef UTIL_H_
#define UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stdint.h"
#include "string.h"

uint32_t util_atoi(char* src);
float loop_float_constrain(float Input, float minValue, float maxValue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UTIL_H_ */
