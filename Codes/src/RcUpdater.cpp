#include "RcUpdater.h"

rtos::Thread ThreadRcUpdater("ThreadRcUpdater");
static char rc_updater_stack[RC_UPDATER_STACK_SIZE];

RcManager::RcManager(){}

RcManager* RcManager::Instance()
{
    static RcManager instance;
    return &instance;
}

void RcManager::Init(void)
{    
    m_pThreadRcUpdater = &ThreadRcUpdater;
    
    m_pThreadRcUpdater->SetEntry(RcManager::Update);
    m_pThreadRcUpdater->SetParameter(RT_NULL);
    m_pThreadRcUpdater->SetPriority(RC_UPDATER_PRIOITY);
    m_pThreadRcUpdater->SetStackStart(rc_updater_stack);
    m_pThreadRcUpdater->SetStackSize(RC_UPDATER_STACK_SIZE);
    m_pThreadRcUpdater->SetTick(RC_UPDATER_TIMESLICE);

    m_TickLastUpdate = 0;

    RC->Init();
    
    m_pThreadRcUpdater->Init();  
}

void RcManager::Update(void *parameter)
{
    while (1)
    {
        rtos::enter_critical();
			
        RC->Update();
        RcUpdater->m_TickLastUpdate = rtos::tick_get();
			
        rtos::exit_critical();
			
        rtos::delay_ms(RC_UPDATER_CONTROL_PERIOD);
    }

}
