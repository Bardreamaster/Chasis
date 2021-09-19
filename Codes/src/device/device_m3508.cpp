#include "device/device_m3508.h"

#include "util_rtos.h"
#include "util.h"

DeviceM3508::DeviceM3508(CanMotorIdType id, CAN_TypeDef* can) : m_MotorId(id),
                                                                m_CanDevice(can)
{
}

void DeviceM3508::Init(void)
{
    m_hasPosFdb = true;
    m_hasSpdFdb = true;
    m_SpdSet = 0.0f;
    m_PosSet = 0.0f;
    m_SpdFdb = 0.0f;
    m_PosFdb = 0.0f;
    m_ControlMode = SPD_MODE;

    m_ControlPeriodMs = 0;

    CAN_MOTOR_FLG_INIT(m_MotorId, m_CanDevice);

    m_Raw2Rad = POS_RAW_TO_RAD_3508;
    m_Raw2Rps = SPD_RAW_TO_RPS_3508;

    m_pRawFdb = get_p_motor_feedback(m_MotorId, m_CanDevice);

    UpdateFdb();
	
		dbg_sst=SST::sorftware_simulation_test();
		dbg_sst.Init();
}

void DeviceM3508::Update(void)
{
    UpdateFdb();
	
		dbg_sst.GPIO_test();
	
    if(m_ControlMode == POS_MODE){
        float temp_Pos;
        m_PidPos.SetRef(m_PosSet);
        if(fabs(m_PosFdb - m_PosSet) > PI){
            if((m_PosFdb - m_PosSet) > 0.0f){
                temp_Pos = m_PosFdb - 2 * PI;
            }
            else{
                temp_Pos = m_PosFdb + 2 * PI;
            }
        }
        m_PidPos.SetFdb(temp_Pos);
        m_PidPos.Update();
        m_SpdSet = m_PidPos.GetResult();
    }

    m_PidSpd.SetRef(m_SpdSet);
    m_PidSpd.SetFdb(m_SpdFdb);

    m_PidSpd.Update();

    m_CurrentSet = m_PidSpd.GetResult();

    Command();
}

void DeviceM3508::UpdateFdb(void)
{
    rtos::enter_critical();

    m_PosFdbLast = m_PosFdb;
    m_PosFdb = m_pRawFdb->ecd * m_Raw2Rad;
    m_SpdFdbAlt = loop_float_constrain((m_PosFdb - m_PosFdbLast), -PI, PI) * m_ControlFreq;
    m_SpdFdb = m_pRawFdb->speed_rpm * m_Raw2Rps;
    rtos::exit_critical();
}

void DeviceM3508::Command(void)
{
    int16_t can_cmd = (int16_t)m_CurrentSet;
    CAN_MOTOR_CMD(m_MotorId, m_CanDevice, can_cmd);
}
