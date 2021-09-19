#include "util.h"

uint32_t util_atoi(char* src)
{
    uint8_t len = strlen(src);
    uint32_t result = 0;
    uint32_t multiplier = 1;
    for(int i = len - 1;i >= 0;--i){
        result += ((src[i] - '0') * multiplier);
        multiplier *= 10;
    }

    return result;
}

float loop_float_constrain(float Input, float minValue, float maxValue)
{
    if (maxValue < minValue)
    {
        return Input;
    }

    if (Input > maxValue)
    {
        float len = maxValue - minValue;
        while (Input > maxValue)
        {
            Input -= len;
        }
    }
    else if (Input < minValue)
    {
        float len = maxValue - minValue;
        while (Input < minValue)
        {
            Input += len;
        }
    }
    return Input;
}
