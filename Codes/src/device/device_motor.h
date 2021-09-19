/**
  ****************************************************************
  * @file    device_motor.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-13
  ****************************************************************
  */

#ifndef DEVICE_MOTOT_H_
#define DEVICE_MOTOT_H_

#include <stdint.h>

enum MotorControlModeType{
    POS_MODE,
    SPD_MODE,
};

class DeviceMotor
{
protected:
    bool m_hasSpdFdb;
    float m_SpdFdb;
    float m_SpdSet;

    bool m_hasPosFdb;
    float m_PosFdb;
    float m_PosSet;

    uint32_t m_ControlPeriodMs;
    float m_ControlFreq;

    MotorControlModeType m_ControlMode;

public:
    bool HasSpdFdb(void){return m_hasSpdFdb;};
    bool HasPosFdb(void){return m_hasPosFdb;};
    float GetSpdFdb(void){return m_SpdFdb;};
    float GetPosFdb(void){return m_PosFdb;};
    void SetSpd(float spd){m_SpdSet = spd;};
    void SetPos(float pos){m_SpdSet = pos;};

    uint32_t GetControlPeriodMs(void){return m_ControlPeriodMs;};
    void SetControlPeriodMs(uint32_t tick){m_ControlPeriodMs = tick;m_ControlFreq = 1000.0f / (float)tick;};

    virtual void Init(void) = 0;
    virtual void Update(void) = 0;    
    virtual void UpdateFdb(void) = 0;
    virtual void Command(void) = 0;
};


#endif /* DEVICE_MOTOT_H_ */
