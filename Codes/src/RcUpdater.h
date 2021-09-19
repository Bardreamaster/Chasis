/**
  ****************************************************************
  * @file    RcUpdater.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-13
  ****************************************************************
  */

#ifndef RC_UPDATER_H_
#define RC_UPDATER_H_

#include "util_rtos.h"
#include "thread_param.h"

#include "device/device_rc.h"

#include "device/device_buzzer.h"

#define RcUpdater RcManager::Instance()

void piano(void);

class RcManager
{
private:
    rtos::Thread* m_pThreadRcUpdater;
    uint32_t m_TickLastUpdate;

    RcManager();

public:
    void Init(void);
    uint32_t GetTickLastUpdate(void){return m_TickLastUpdate;};
    static void Update(void *parameter);
    static RcManager* Instance();
};

#endif /* RC_UPDATER_H_ */
