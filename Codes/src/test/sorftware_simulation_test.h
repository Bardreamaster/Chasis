#ifndef SORFTWARE_SIMULATION_TEST_H
#define SORFTWARE_SIMULATION_TEST_H

#include "stm32f4xx.h"

namespace SST
{

    class sorftware_simulation_test
    {
    private:
        /* data */

    public:
        void Init(void);
        void GPIO_test(void);
        sorftware_simulation_test(/* args */);
        ~sorftware_simulation_test();
    };
}
#endif
