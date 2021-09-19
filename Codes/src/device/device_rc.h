/**
  ****************************************************************
  * @file    device_rc.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-12
  ****************************************************************
  */

#ifndef DEVICE_RC_H_
#define DEVICE_RC_H_

#define RC DeviceRC::Instance()

#include "util_rtos.h"

#include "bsp/bsp_rc.h"

/**
  * #define RC_SW_UP ((uint16_t)1)
  * #define RC_SW_MID ((uint16_t)3)
  * #define RC_SW_DOWN ((uint16_t)2)
  * 
  * #define RC_KBD_W ((uint16_t)1 << 0)
  * #define RC_KBD_S ((uint16_t)1 << 1)
  * #define RC_KBD_A ((uint16_t)1 << 2)
  * #define RC_KBD_D ((uint16_t)1 << 3)
  * #define RC_KBD_SHIFT ((uint16_t)1 << 4)
  * #define RC_KBD_CTRL ((uint16_t)1 << 5)
  * #define RC_KBD_Q ((uint16_t)1 << 6)
  * #define RC_KBD_E ((uint16_t)1 << 7)
  * #define RC_KBD_R ((uint16_t)1 << 8)
  * #define RC_KBD_F ((uint16_t)1 << 9)
  * #define RC_KBD_G ((uint16_t)1 << 10)
  * #define RC_KBD_Z ((uint16_t)1 << 11)
  * #define RC_KBD_X ((uint16_t)1 << 12)
  * #define RC_KBD_C ((uint16_t)1 << 13)
  * #define RC_KBD_V ((uint16_t)1 << 14)
  * #define RC_KBD_B ((uint16_t)1 << 15)
  */

enum PcKeyStateType{
    KEY_DOWN = 0,
    KEY_FALL,
    KEY_RISE,
};

enum PcKeyType{
    PC_KEY_W = 0,
    PC_KEY_S,
    PC_KEY_A,
    PC_KEY_D,
    PC_KEY_SHIFT,
    PC_KEY_CTRL,
    PC_KEY_Q,
    PC_KEY_E,
    PC_KEY_R,
    PC_KEY_F,
    PC_KEY_G,
    PC_KEY_Z,
    PC_KEY_X,
    PC_KEY_C,
    PC_KEY_V,
    PC_KEY_B,
    PC_M_L,
    PC_M_R,
};

enum RcSwStatusType{
    RC_SW_UP   = 1,
    RC_SW_MID  = 3,
    RC_SW_DOWN = 2,
    RC_SW_U2M  = 4,
    RC_SW_M2D  = 5,
    RC_SW_D2M  = 6,
    RC_SW_M2U  = 7,
};

enum RcSwType{
    RC_SW_L = 1,
    RC_SW_R = 0,
};

class KeyEventDetector
{
protected:
    PcKeyType* m_pTargetKeyArray;
    void* m_pOwner;
    uint32_t m_KeyCount;
    uint32_t m_MaxTick;
    KeyEventDetector* m_pNextDetector;
    bool m_EventState;

public:
    KeyEventDetector();
    void Attach(void);

    bool GetEventState(void);
    virtual void Update(void) = 0;

};

class DeviceRC
{
private:

    RC_raw_t* m_pRcRaw;
    KeyEventDetector* m_pEventDetectorList;
    /* Left Horizontal, Left Vertical, Right Horizontal, Right Vertical, normalized */
    float m_LHAxis;
    float m_LVAxis;
    float m_RHAxis;
    float m_RVAxis;
    float m_JoyStickDeadzone;

    uint16_t m_KbdState;
    uint16_t m_PrevKbdState;
    uint16_t m_KbdFall;
    uint16_t m_KbdRise;
    uint8_t m_MouseLState;
    uint8_t m_PrevMouseLState;
    uint8_t m_MouseRState;
    uint8_t m_PrevMouseRState;

    char m_SwState[2];
    char m_PrevSwState[2];

    DeviceRC();
    DeviceRC(const DeviceRC &);
    DeviceRC& operator = (const DeviceRC &);

    void UpdateNormalizedAxis(void);
    void UpdateKbdEvents(void);
    float NormalizeAxis(int16_t ch);

public:

    void Init(void);
    void Enable(void);
    void Disable(void);
    void Restart(uint16_t dma_buf_num);
    void Update(void);
    RC_raw_t* GetRcRaw(void);
    KeyEventDetector** GetKeyEventDetector(void);
    float GetLHAxis(void){return m_LHAxis;};
    float GetLVAxis(void){return m_LVAxis;};
    float GetRHAxis(void){return m_RHAxis;};
    float GetRVAxis(void){return m_RVAxis;};
    int16_t GetMouseX(void){return m_pRcRaw->mouse.x;};
    int16_t GetMouseY(void){return m_pRcRaw->mouse.y;};
    int16_t GetMouseZ(void){return m_pRcRaw->mouse.z;};
    float GetJoyStickDeadZone(void){return m_JoyStickDeadzone;};
    void SetJoyStickDeadZone(float deadzone){m_JoyStickDeadzone = deadzone;};

    bool QuerySwState(RcSwType target, RcSwStatusType queryType);
    bool QueryPcKeyState(PcKeyType target, PcKeyStateType queryType);

    static DeviceRC* Instance();
};

class ChordDetector:public KeyEventDetector
{
private:
    DeviceRC* p_owner;
    uint32_t m_FastDetection;
    uint32_t m_CurrentChord;
public:
    ChordDetector(PcKeyType* p_key_arr_header, uint32_t array_len);
    uint32_t GetCurrentChord(void);
    virtual void Update(void);
};

class RapidTapDector:public KeyEventDetector
{
private:
    DeviceRC* p_owner;
    uint8_t m_MinActiveCount;
    uint8_t m_StartupCounter;
    uint32_t m_LastTick;
public:
    RapidTapDector(PcKeyType* p_key, uint32_t timeout_ms, uint8_t min_active_count);

    void SetMinActiveCount(uint8_t min_active_count){m_MinActiveCount = min_active_count;};
    uint8_t GetMinActiveCount(void){return m_MinActiveCount;};

    virtual void Update(void);
};

class SequenceDetector:public KeyEventDetector
{
private:
    DeviceRC* p_owner;
    uint32_t m_LastTick;
    uint32_t m_CurrentSeq;
    uint32_t m_ActiveTimeout;
public:
    SequenceDetector(PcKeyType* p_key_arr_header, uint32_t array_len, uint32_t tick, uint32_t timeout);
    virtual void Update(void);
};

#endif /* DEVICE_RC_H_ */
