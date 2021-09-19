/**
  ****************************************************************
  * @file    math_pid.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-13
  ****************************************************************
  */

#ifndef MATH_PID_H_
#define MATH_PID_H_
#include <stdint.h>
#include <math.h>

enum PidModeType{
    PID_POSITION = 0,
    PID_DELTA,
};

class Pid
{
private:
    PidModeType m_Mode;

    float m_Kp;
    float m_Ki;
    float m_Kd;

    float m_MaxOut;
    float m_MaxIOut;

    float m_Ref;
    float m_Fdb;

    float m_Result;
    float m_PResult;
    float m_IResult;
    float m_DResult;

    float m_DBuf[3];
    float m_Err[3];

public:
    Pid(void);
    Pid(PidModeType mode, float p, float i, float d, float max, float imax);

    void Init(void);
    void Update(void);
    void Clear(void);

    float GetKp(void){return m_Kp;};
    float GetKi(void){return m_Ki;};
    float GetKd(void){return m_Kd;};

    float GetMaxOut(void){return m_MaxOut;};
    float GetMaxIOut(void){return m_MaxIOut;};
    
    float GetRef(void){return m_Ref;};
    float GetFdb(void){return m_Fdb;};

    void SetRef(float ref){m_Ref = ref;};
    void SetFdb(float fdb){m_Fdb = fdb;};

    float GetResult(void){return m_Result;};
    float GetPResult(void){return m_PResult;};
    float GetIResult(void){return m_IResult;};
    float GetDResult(void){return m_DResult;};

    void SetKp(float p){m_Kp = p;};
    void SetKi(float i){m_Ki = i;};
    void SetKd(float d){m_Kd = d;};

    void SetMaxOut(float max){m_MaxOut = max;};
    void SetMaxIout(float imax){m_MaxIOut = imax;};

};

#endif /* MATH_PID_H_ */
