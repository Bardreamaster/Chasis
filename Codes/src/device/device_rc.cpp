#include "device/device_rc.h"

#include "util.h"

#include "bsp/bsp_rc.h"

#include "rtthread.h"

KeyEventDetector::KeyEventDetector():m_pTargetKeyArray(NULL),
                                     m_KeyCount(0),
                                     m_MaxTick(0),
                                     m_pNextDetector(NULL),
                                     m_EventState(false)
{
}

void KeyEventDetector::Attach(void)
{
    KeyEventDetector** ptr_list = RC->GetKeyEventDetector();
    while((*ptr_list) != NULL){
        ptr_list = &((*ptr_list)->m_pNextDetector);
    }

    *ptr_list = this;
}

bool KeyEventDetector::GetEventState(void)
{
    return m_EventState;
}


DeviceRC::DeviceRC():m_LHAxis(0.0f),
                     m_LVAxis(0.0f),
                     m_RHAxis(0.0f),
                     m_RVAxis(0.0f),
                     m_JoyStickDeadzone(0.1f),
                     m_KbdState(0),
                     m_PrevKbdState(0),
                     m_KbdFall(0),
                     m_KbdRise(0),
                     m_MouseLState(0),
                     m_PrevMouseLState(0),
                     m_MouseRState(0),
                     m_PrevMouseRState(0)
{
    bsp_remote_control_init();
    m_pRcRaw = get_remote_control_raw();
}

DeviceRC* DeviceRC::Instance()
{
    static DeviceRC instance;
    return &instance;
}

void DeviceRC::Enable(void)
{
    bsp_rc_enable();
}

void DeviceRC::Disable(void)
{
    bsp_rc_disable();
}

void DeviceRC::Restart(uint16_t dma_buf_num)
{
    bsp_rc_restart(dma_buf_num);
}

void DeviceRC::Init(void)
{
    DeviceRC::Instance()->Enable();
}

void DeviceRC::Update(void)
{
    this->UpdateNormalizedAxis();
    this->UpdateKbdEvents();
    if(this->m_pEventDetectorList != NULL){
        this->m_pEventDetectorList->Update();
    }
}

RC_raw_t* DeviceRC::GetRcRaw(void)
{
    return m_pRcRaw;
}

KeyEventDetector** DeviceRC::GetKeyEventDetector(void)
{
    return &m_pEventDetectorList;
}

void DeviceRC::UpdateNormalizedAxis(void)
{
    m_LHAxis = NormalizeAxis(m_pRcRaw->rc.ch[2]);
    m_LVAxis = NormalizeAxis(m_pRcRaw->rc.ch[3]);
    m_RHAxis = NormalizeAxis(m_pRcRaw->rc.ch[0]);
    m_RVAxis = NormalizeAxis(m_pRcRaw->rc.ch[1]);
}

void DeviceRC::UpdateKbdEvents(void)
{
    m_PrevKbdState = m_KbdState;
    m_KbdState = m_pRcRaw->key.v;

    m_PrevMouseLState = m_MouseLState;
    m_MouseLState = m_pRcRaw->mouse.press_l;

    m_PrevMouseRState = m_MouseRState;
    m_MouseRState = m_pRcRaw->mouse.press_r;

    m_PrevSwState[RC_SW_L] = m_SwState[RC_SW_L];
    m_SwState[RC_SW_L] = m_pRcRaw->rc.s[RC_SW_L];

    m_PrevSwState[RC_SW_R] = m_SwState[RC_SW_R];
    m_SwState[RC_SW_R] = m_pRcRaw->rc.s[RC_SW_R];

    uint16_t kbdChanges = m_KbdState ^ m_PrevKbdState;

    m_KbdFall = kbdChanges & m_KbdState;
    m_KbdRise = kbdChanges & (~m_KbdState);
}

float DeviceRC::NormalizeAxis(int16_t ch)
{
    float normalizedAxis = (float)ch / (float)RC_CH_OFFSET_MAX;
    if(normalizedAxis < m_JoyStickDeadzone && normalizedAxis > -m_JoyStickDeadzone){
        normalizedAxis = 0.0f;
    }
    return normalizedAxis;
}

bool DeviceRC::QuerySwState(RcSwType target, RcSwStatusType queryType)
{
    switch (queryType)
    {
    case RC_SW_U2M:
        return ((m_PrevSwState[target] == RC_SW_UP) && (m_SwState[target] == RC_SW_MID));

    case RC_SW_M2D:
        return ((m_PrevSwState[target] == RC_SW_MID) && (m_SwState[target] == RC_SW_DOWN));

    case RC_SW_D2M:
        return ((m_PrevSwState[target] == RC_SW_DOWN) && (m_SwState[target] == RC_SW_MID));

    case RC_SW_M2U:
        return ((m_PrevSwState[target] == RC_SW_MID) && (m_SwState[target] == RC_SW_UP));
    
    default:
        return (m_SwState[target] == queryType);
    }
}

bool DeviceRC::QueryPcKeyState(PcKeyType target, PcKeyStateType queryType)
{
    switch (target)
    {
    case PC_M_L:
            if(KEY_DOWN == queryType){
                return (m_pRcRaw->mouse.press_l != 0);
            }
            else if(KEY_FALL == queryType){
                return (((m_PrevMouseLState ^ m_MouseLState) & m_MouseLState) != 0);
            }
            else{
                return (((m_PrevMouseLState ^ m_MouseLState) & (~m_MouseLState)) != 0);
            }

    case PC_M_R:
            if(KEY_DOWN == queryType){
                return (m_pRcRaw->mouse.press_r != 0);
            }
            else if(KEY_FALL == queryType){
                return (((m_PrevMouseRState ^ m_MouseRState) & m_MouseRState) != 0);
            }
            else{
                return (((m_PrevMouseRState ^ m_MouseRState) & (~m_MouseRState)) != 0);
            }
    
    default:
            uint16_t query_key = 0;
            if(KEY_DOWN == queryType){
                query_key = m_KbdState;
            }
            else if(KEY_FALL == queryType){
                query_key = m_KbdFall;
            }
            else if(KEY_RISE == queryType){
                query_key = m_KbdRise;
            }
    
            return ((query_key & (uint16_t(1 << target))) != 0);
    }

    
}

ChordDetector::ChordDetector(PcKeyType* p_key_arr_header, uint32_t array_len):p_owner(RC),
                                                                              m_FastDetection(0)
{
    m_pTargetKeyArray = p_key_arr_header;
    m_KeyCount = array_len;
    m_MaxTick = 1;

    for(int i = 0;i < array_len;++i){
        if(m_pTargetKeyArray[i] == PC_M_L){
            m_FastDetection |= 0xf000000;
        }
        else if(m_pTargetKeyArray[i] == PC_M_R){
            m_FastDetection |= 0x0f00000;
        }
        else
        {
            m_FastDetection |= ((uint32_t)1 << m_pTargetKeyArray[i]);
        }
    }
}

uint32_t ChordDetector::GetCurrentChord(void)
{
    return m_CurrentChord;
}

void ChordDetector::Update(void)
{
    m_CurrentChord = 0x0000;
    if(RC->QueryPcKeyState(PC_M_L, KEY_DOWN)){
        m_CurrentChord |= 0xf000000;
    }
    if(RC->QueryPcKeyState(PC_M_R, KEY_DOWN)){
        m_CurrentChord |= 0x0f00000;
    }
    m_CurrentChord |= RC->GetRcRaw()->key.v;

    if(m_CurrentChord == m_FastDetection){
        m_EventState = true;
    }
    else{
        m_EventState = false;
    }

    if(m_pNextDetector != NULL){
        m_pNextDetector->Update();
    }

}

RapidTapDector::RapidTapDector(PcKeyType* p_key, uint32_t timeout_ms, uint8_t min_active_count):m_MinActiveCount(min_active_count),
                                                                                                m_StartupCounter(0),
                                                                                                m_LastTick(0)
{
    m_pTargetKeyArray = p_key;
    m_KeyCount = 1;
    m_MaxTick = timeout_ms;    
}

void RapidTapDector::Update(void)
{
    uint32_t tick = rtos::tick_get();
    uint32_t dt = tick - m_LastTick;
    if(dt > m_MaxTick){
        m_StartupCounter = 0;
        m_EventState = false;
    }

    if(RC->QueryPcKeyState(*m_pTargetKeyArray, KEY_RISE)){        
        
        m_LastTick = tick;
        
        if(m_StartupCounter < m_MinActiveCount - 1){
            ++m_StartupCounter;
            m_EventState = false;
        }
        else{
            m_EventState = true;
        }
    }

    if(m_pNextDetector != NULL){
        m_pNextDetector->Update();
    }
}

SequenceDetector::SequenceDetector(PcKeyType* p_key_arr_header, uint32_t array_len, uint32_t tick, uint32_t timeout):m_LastTick(0),
                                                                                                                     m_CurrentSeq(0)
{
    m_pTargetKeyArray = p_key_arr_header;
    m_KeyCount = array_len;
    m_MaxTick = tick;
    m_ActiveTimeout = timeout;
}

void SequenceDetector::Update(void)
{
    uint32_t tick = rtos::tick_get();
    uint32_t dt = tick - m_LastTick;

    if((!m_EventState && (dt > m_MaxTick)) || (m_EventState && (dt > m_ActiveTimeout))){
        m_CurrentSeq = 0;
        m_EventState = false;
    }

    if((RC->QueryPcKeyState(m_pTargetKeyArray[m_CurrentSeq], KEY_RISE)) && (m_CurrentSeq < m_KeyCount)){
        
        m_LastTick = tick;
        
        if(m_CurrentSeq < m_KeyCount - 1){
            ++m_CurrentSeq;
            m_EventState = false;
        }
        else{
            m_EventState = true;
        }
    }

    if(m_pNextDetector != NULL){
        m_pNextDetector->Update();
    }
}
