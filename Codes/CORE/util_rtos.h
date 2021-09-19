/**
  ****************************************************************
  * @file    util_rtos.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-11
  ****************************************************************  
  */

#ifndef UTIL_RTOS_H_
#define UTIL_RTOS_H_

#include "stm32f4xx.h"

#include "rtthread.h"
#include "rthw.h"

//  Seems like ARMCC only support inline namespace, so I had to
//  implement stuff in the header file which sucks
namespace rtos{
    
inline void delay_ms(uint32_t ms)
{
    rt_thread_mdelay((rt_uint32_t)ms);
}

inline void interrupt_enter(void)
{
    rt_interrupt_enter();
}

inline void interrupt_leave(void)
{
    rt_interrupt_leave();
}

inline uint8_t interrupt_get_nest(void)
{
    return rt_interrupt_get_nest();
}

inline uint32_t tick_get(void)
{
    return rt_tick_get();
}

inline void tick_set(uint32_t tick)
{
    rt_tick_set(tick);
}

inline void enter_critical(void)
{
    rt_enter_critical();
}

inline void exit_critical(void)
{
    rt_exit_critical();
}

class Thread
{
public:
    enum ThreadInitFlagType{
        ENTRY_FLAG       = 0x01,
        PARAMETER_FLAG   = 0x02,
        STACK_START_FLAG = 0x04,
        STACK_SIZE_FLAG  = 0x08,
        PRIORITY_SIZE    = 0x10,
        TICK_FLAG        = 0x20
    };

    Thread(const char* name)
    {
        rt_strncpy(m_Name, name, rt_strlen(name));
        m_Name[rt_strlen(name)] = 0;
        m_InitFlag = 0x00;
    };

    ~Thread()
    {
        rt_kprintf("%s is destoryed\n", m_Thread.name);
        rt_thread_detach(&m_Thread);
    };

    void SetEntry(void (*entry)(void* parameter))
    {
        m_pEntry = entry;
        m_InitFlag |= ENTRY_FLAG;
    };

    void SetParameter(void* parameter)
    {
        m_pParameter = parameter;
        m_InitFlag |= PARAMETER_FLAG;
    };

    void SetStackStart(void* stackStart)
    {
        m_pStackStart = stackStart;
        m_InitFlag |= STACK_START_FLAG;
    };

    void SetStackSize(uint32_t stackSize)
    {
        m_StackSize = stackSize;
        m_InitFlag |= STACK_SIZE_FLAG;
    };

    void SetPriority(uint8_t priority)
    {
        m_Priority = priority;
        m_InitFlag |= PRIORITY_SIZE;
    };

    void SetTick(uint8_t tick)
    {
        m_Tick = tick;
        m_InitFlag |= TICK_FLAG;
    };

    uint32_t Init(void)
    {
        if(0x3f == m_InitFlag){
            rt_err_t init_result = rt_thread_init(&m_Thread, m_Name, m_pEntry, m_pParameter, m_pStackStart, m_StackSize, m_Priority, m_Tick);
            if(RT_EOK == init_result){
                init_result = rt_thread_startup(&m_Thread);
            }

            return init_result;
        }
        else{
            while(1){
                rt_kprintf("Failed to init %s, Error code: %2x\n", this->m_Name, this->m_InitFlag);
                delay_ms(1000);
            }
        }
    };

    uint32_t Yield(void)
    {
        return rt_thread_yield();
    };

private:
    rt_thread m_Thread;
    char m_Name[RT_NAME_MAX];
    void (*m_pEntry)(void* parameter);
    void* m_pParameter;
    void* m_pStackStart;
    uint32_t m_StackSize;
    uint8_t m_Priority; 
    uint32_t m_Tick;
    uint8_t m_InitFlag;
};

class Semaphore
{
public:
    enum SemaphoreFlagType{
        IPC_FLAG_FIFO = 0,
        IPC_FLAG_PRIO
    };

    Semaphore(const char* name, rt_uint32_t value = 1, SemaphoreFlagType flagIn = IPC_FLAG_PRIO)
    {        
        rt_strncpy(m_Name, name, RT_NAME_MAX);
        m_Name[rt_strlen(name)] = 0;
        m_SuspendThreadCnt = 0;
        m_Value = value;
        m_Flag = (flagIn == IPC_FLAG_PRIO) ? RT_IPC_FLAG_PRIO : RT_IPC_FLAG_FIFO;

        rt_sem_init(m_Semaphore, m_Name, m_Value, m_Flag);
    };

    ~Semaphore()
    {
        rt_sem_detach(m_Semaphore);        
    };

    void TryTake(void)
    {
        rt_sem_trytake(m_Semaphore);
    };

    void TakeForever(void)
    {
        while(RT_EOK != rt_sem_take(m_Semaphore, 0xFFFF)){
            ;
        }
    }

    void Take(uint32_t timeout)
    {
        rt_sem_take(m_Semaphore, timeout);
    }

private:
    char m_Name[RT_NAME_MAX] ;
    uint16_t m_SuspendThreadCnt;
    rt_sem_t m_Semaphore;
    rt_uint32_t m_Value;
    rt_uint8_t m_Flag;

};

} // namespace rtos


#endif /* UTIL_RTOS_H_ */
