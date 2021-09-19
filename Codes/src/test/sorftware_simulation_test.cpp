#include "test/sorftware_simulation_test.h"

using namespace SST;

 sorftware_simulation_test::sorftware_simulation_test(/* args */)
    {

    }



 sorftware_simulation_test::~sorftware_simulation_test()
   {

    }


void sorftware_simulation_test::Init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

    {
        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
}

void sorftware_simulation_test::GPIO_test(void)
{
    GPIO_ToggleBits(GPIOC,GPIO_Pin_3);
}
