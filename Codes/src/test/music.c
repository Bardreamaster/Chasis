#include "test/music.h"

#include "rtthread.h"

typedef enum
{
    gs3 = 349,
    a_3 = 370,
    as3 = 392,
    b_3 = 415,
    c_4 = 440,
    cs4 = 466,
    d_4 = 523,
    ds4 = 587,
    e_4 = 622,
    f_4 = 659,
    fs4 = 698,
    g_4 = 740,
    gs4 = 784,
    a_4 = 831,
    as4 = 880,
    b_4 = 932,
    c_5 = 988,
    cs5 = 1047,
    d_5 = 1109,
    ds5 = 1175,
    e_5 = 1245,
    f_5 = 1319,
    fs5 = 1397,
    g_5 = 1480,
    gs5 = 1568,
    a_5 = 1661,
    as5 = 1760,
    b_5 = 1865,
    c_6 = 1976,
    cs6 = 2093,
    d_6 = 2217,
    ds6 = 2349,
    e_6 = 2489,
    f_6 = 2637,
    fs6 = 2794,
    g_6 = 2960,
    gs6 = 3136,
    a_6 = 3322,
    as6 = 3520,
    
    slt = 0,
    eos = -1,
}freq_e;

typedef struct note_t
{
    freq_e freq;
    int length;
}
note;

const note sheet[] = 
{
    {cs4,4},
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {d_4,2}, {d_4,2}, {b_3,2}, {b_3,2}, {b_3,1}, {slt,1},
    
    {cs4,4},
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {d_4,2}, {d_4,2}, {b_3,2}, {b_3,2}, {b_3,1}, {slt,1},
    
    {cs4,4},
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {d_4,2}, {d_4,2}, {b_3,2}, {b_3,2}, {b_3,1}, {slt,1},
    
    {cs4,4},
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1},
    {cs4,2}, {cs4,2}, {d_4,2}, {d_4,2}, {d_4,2}, {d_4,2}, {d_4,2}, 
    {b_3,2}, {b_3,2}, {b_3,1}, {slt,1}, 
    
    {cs4,4},
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {b_3,1}, {slt,1}, {cs4,4}, 
    {d_4,4}, {b_3,4}, {b_3,1}, {slt,1},
    
    {cs4,4},
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {b_3,1}, {slt,1}, {cs4,4}, 
    {d_4,4}, {b_3,4}, {b_3,1}, {slt,1},
    
    {cs4,4},
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {b_3,1}, {slt,1}, {cs4,4}, 
    {d_4,4}, {b_3,4}, 
    
    {gs3,4}, {gs3,4}, {gs3,4}, {a_3,4}, {a_3,4}, {as3,4}, {as3,4}, {b_3,4}, {b_3,1}, {slt,1}, 
    
    {cs4,4},
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {b_3,1}, {slt,1}, {cs4,4}, 
    {d_4,4}, {b_3,4}, {b_3,1}, {slt,1},
    
    {cs4,4},
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {b_3,1}, {slt,1}, {cs4,4}, 
    {d_4,4}, {b_3,4}, {b_3,1}, {slt,1},
    
    {cs4,4},
    {b_3,1}, {slt,1}, {cs4,4}, {b_3,1}, {slt,1}, {cs4,4}, 
    {b_3,1}, {slt,1}, {cs4,4}, 
    {d_4,4}, {b_3,4}, 
    
    {gs3,4}, {gs3,4}, {gs3,4}, {a_3,4}, {a_3,4}, {as3,4}, {as3,4},
    {b_3,4}, {cs4,6}, {gs4,6}, {g_4,6}, {b_4,6}, {fs4,2}, {e_4,4},
    {d_4,2}, {cs4,6}, {gs4,6}, {g_4,20},{cs4,6}, {gs4,6}, {g_4,6},
    {b_4,6}, 
    {fs4,2}, {e_4,4}, {e_4,2}, 
    {fs4,2}, {e_4,4}, {e_4,2}, 
    {fs4,2}, {e_4,4}, {e_4,2}, 
    {fs4,2}, {e_4,4}, {e_4,2}, 
    {fs4,2}, {e_4,4}, {e_4,2}, 
    {cs4,6}, {gs4,6}, {g_4,6}, {b_4,6},
    {fs4,2}, {e_4,4}, {d_4,2}, {cs4,6}, {gs4,6}, {g_4,20},
    {cs4,6}, {gs4,6}, {g_4,6}, {b_4,6},
    {fs4,2}, {e_4,4}, {e_4,2}, 
    {fs4,2}, {e_4,4}, {e_4,2}, 
    {fs4,2}, {e_4,4}, {e_4,2}, 
    {fs4,2}, {e_4,4}, {e_4,2}, 
    {fs4,2}, {e_4,4}, {e_4,2}, 
        
    {eos,0}
};
static int index = 0;

const note sheet2[] = 
{
    {g_6, 4}, {fs6, 4}, {e_6, 4}, {e_6, 4}, {fs6, 4}, {slt, 44},
    
    {a_5,4},{g_6,4},{fs6,4},{e_6,4},{e_6,8},{fs6,4},{slt, 8},{d_6,8},{e_6,4},{a_5,8},{slt, 24},
    
    {a_5,4},{e_6,8},{fs6,4},{g_6,12},{e_6,8},{cs6,8},{d_6,12},{e_6,8},{a_5,4},{a_5,8},{fs6,8},{slt,36},
        
    {a_5,4},{g_6,4},{fs6,4},{e_6,4},{e_6,8},{slt,4},{fs6,4},{d_6,8},{slt,4},{e_6,4},{a_5,8},{slt,28},
        
    {e_6,8},{fs6,4},{g_6,12},{e_6,4},{cs6,12},{d_6,4},{e_6,4},{slt,4},
    
    {a_5,4},{d_6,4},{e_6,4},{f_6,4},{e_6,4},{d_6,4},{c_6,4},{slt,8},
    
    {a_5,4},{as5,4},{c_6,8},{f_6,8},{e_6,4},{d_6,4},{d_6,4},{c_6,4},{d_6,4},{c_6,4},{c_6,8},{c_6,4},{slt,4},
        
    {a_5,4},{as5,4},{c_6,8},{f_6,8},{g_6,4},{f_6,4},{d_6,4},{d_6,4},{e_6,4},{f_6,8},{f_6,4},{slt,4},
        
    {g_6,4},{a_6,4},{as6,4},{as6,4},{a_6,8},{g_6,8},{f_6,4},{g_6,4},{a_6,4},{a_6,4},{g_6,8},{f_6,8},{d_6,4},{c_6,4},{d_6,4},{f_6,4},{f_6,4},{e_6,8},{e_6,4},{fs6,4},{fs6,16},{slt,100},

    {eos, 0}
};

void buzzer_music(void)
{
    int index_inner, n;
    if(index ==0)rt_kprintf("Now playing: Still Alive\n");
    if(sheet2[index].freq != eos)
    {
        index_inner = index;
        n = sheet2[index].length;
        index ++;
    }
    else
    {
        index_inner = index;
        index = 0;        
    }
    
    if(sheet2[index_inner].freq == slt)
    {
        TIM3->CCR1 = 0;
        for(int i = 0;i < n;i ++)
        {
            rt_thread_mdelay(63);
        }
        return ;
    }
    
    uint32_t psc = (uint32_t)(200000.0f/sheet2[index_inner].freq);
    TIM3->PSC = psc;
    TIM3->CCR1 = 360;

    for(int i = 0;i < n - 1;i ++)
    {
        rt_thread_mdelay(63);
    }
    
    rt_thread_mdelay(50);
    
    TIM3->CCR1 = 0;
    
    rt_thread_mdelay(26);    
}

void buzzer_init(void)
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
    
    
    TIM_InitStruct.TIM_Period = 839;
    TIM_InitStruct.TIM_Prescaler = 100000 / 440 - 1;
    TIM_InitStruct.TIM_ClockDivision = 0;
    TIM_InitStruct.TIM_CounterMode =TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_InitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC1Init(TIM3, &TIM_OCInitStruct);
    TIM3->CCR1 = 720;

    TIM_Cmd(TIM3, ENABLE);
}
