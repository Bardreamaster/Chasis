#include "rtthread.h"
#include "stm32f4xx.h"

void rt_hw_console_output(const char *str)
{
    rt_size_t size = 0;
    char a = '\r';

    size = rt_strlen(str);
    for(int i = 0; i < size; i++){
        if (*(str + i) == '\n')
        {
            USART_SendData(USART2, (uint16_t)a);
 //           while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){
 //              ;
 //          }
        }
        USART_SendData(USART2, (uint16_t)(*(str + i)));
//        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){
//            ;
//       }
    }

}
