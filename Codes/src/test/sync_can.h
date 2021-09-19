#ifndef SYNC_CAN_H
#define SYNC_CAN_H

#include "stm32f4xx.h"
#include "test/can_list.h"

class sync_can
{
private:
    CAN_TypeDef can;
public:
    void sync_can_bsp_init(void);
    sync_can();
    ~sync_can();
};

sync_can::sync_can(/* args */)
{
}

sync_can::~sync_can()
{
}



#endif
