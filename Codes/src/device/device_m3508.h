/**
  ****************************************************************
  * @file    device_m3508.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-13
  ****************************************************************
  */

#ifndef DEVICE_M3508_H_
#define DEVICE_M3508_H_

#include "device/device_motor.h"
#include "bsp/bsp_can.h"
#include "mathlib/math_pid.h"
#include "test/sorftware_simulation_test.h"

#define POS_RAW_TO_RAD_3508 0.0007670840321f  /* 2Pi / 8191 */
#define SPD_RAW_TO_RPS_3508 0.005817764173f   /* 2Pi / 60 / 18 */

class DeviceM3508 : public DeviceMotor
{
private:
    motor_feedback_t* m_pRawFdb;
    float m_Temperature;
    CanMotorIdType m_MotorId;
    float m_SpdFdbAlt;
    float m_PosFdbLast;
    float m_CurrentSet;
	float m_Raw2Rad;
	float m_Raw2Rps;

    Pid m_PidSpd;
    Pid m_PidPos;

    CAN_TypeDef* m_CanDevice;

public:
	
		SST::sorftware_simulation_test dbg_sst;

    DeviceM3508(CanMotorIdType id, CAN_TypeDef* can);
    virtual void Init(void);
    virtual void Update(void);
    virtual void UpdateFdb(void);
    virtual void Command(void);
    Pid* GetSpdPidInstance(void){return &m_PidSpd;};
    Pid* GetPosPidInstance(void){return &m_PidPos;};
    float GetCurrentSet(void){return m_CurrentSet;};
	void SetRawFdb(motor_feedback_t* p_fdb){m_pRawFdb = p_fdb;};
	uint32_t GetMotorId(void){return m_MotorId;};
};

#endif /* DEVICE_M3508_H_ */
