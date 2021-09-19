#include "mathlib/math_pid.h"

#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }

Pid::Pid():m_Mode(PID_POSITION),
           m_Kp(0.0f),
           m_Ki(0.0f),
           m_Kd(0.0f),
           m_MaxOut(0.0f),
           m_MaxIOut(0.0f)
{
    m_DBuf[0] = m_DBuf[1] = m_DBuf[2] =0.0f;
    m_Err[0] = m_Err[1] = m_Err[2] =0.0f;
}

Pid::Pid(PidModeType mode = PID_POSITION,
         float p = 0.0f,
         float i = 0.0f,
         float d = 0.0f,
         float max = 0.0f,
         float imax = 0.0f):m_Mode(mode),
                            m_Kp(p),
                            m_Ki(i),
                            m_Kd(d),
                            m_MaxOut(max),
                            m_MaxIOut(imax)
{
}

void Pid::Init(void)
{
    m_Result = 0.0f;
    m_DBuf[0] = m_DBuf[1] = m_DBuf[2] =0.0f;
    m_Err[0] = m_Err[1] = m_Err[2] =0.0f;
}

void Pid::Update(void)
{
    m_Err[2] = m_Err[1];
    m_Err[1] = m_Err[0];

    m_Err[0] = m_Ref - m_Fdb;

    if(m_Mode == PID_POSITION){
        m_PResult = m_Kp * m_Err[0];
        m_IResult += m_Ki * m_Err[0];
        m_DBuf[2] = m_DBuf[1];
        m_DBuf[1] = m_DBuf[0];
        m_DBuf[0] = m_Err[0] - m_Err[1];
        m_DResult = m_Kd * m_DBuf[0];
        LimitMax(m_IResult, m_MaxIOut);
    }
    else if(m_Mode == PID_DELTA){
        m_PResult = m_Kp * (m_Err[0] - m_Err[1]);
        m_IResult = m_Ki * m_Err[0];
        m_DBuf[2] = m_DBuf[1];
        m_DBuf[1] = m_DBuf[0];
        m_DBuf[0] = (m_Err[0] - 2.0f * m_Err[1] + m_Err[2]);
        m_DResult = m_Kd * m_DBuf[0];
    }

    m_Result = m_PResult + m_IResult + m_DResult;
    LimitMax(m_Result, m_MaxOut);

}

void Pid::Clear(void)
{
    m_DBuf[0] = m_DBuf[1] = m_DBuf[2] =0.0f;
    m_Err[0] = m_Err[1] = m_Err[2] =0.0f;
    m_PResult = m_IResult = m_DResult = m_Result = 0.0f;
    m_Ref = m_Fdb = 0.0f;
}
