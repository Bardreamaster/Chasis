#include "bsp/bsp_buzzer.h"

void bsp_buzzer_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);    
       
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    //Input freq = 84Mhz
    TIM_InitStruct.TIM_Period = 279;
    TIM_InitStruct.TIM_Prescaler = 300000 / 440 - 1;
    TIM_InitStruct.TIM_ClockDivision = 0;
    TIM_InitStruct.TIM_CounterMode =TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_InitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC1Init(TIM3, &TIM_OCInitStruct);
    TIM3->CCR1 = 140;

    TIM_Cmd(TIM3, DISABLE);
}

void bsp_buzzer_on(void)
{
    TIM_Cmd(TIM3, ENABLE);
}

void bsp_buzzer_off(void)
{
    TIM_Cmd(TIM3, DISABLE);
}

void bsp_buzzer_set_freq(uint32_t freq)
{
    TIM3->PSC = 300000 / freq;
}
