/**
  ****************************************************************
  * @file    device_buzzer.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-11
  ****************************************************************
  */

#ifndef DEVICE_BUZZER_H_
#define DEVICE_BUZZER_H_

#define Buzzer DeviceBuzzer::Instance()

#include "util_rtos.h"

class DeviceBuzzer
{
private:

    uint32_t m_Freq;

    DeviceBuzzer();
    DeviceBuzzer(const DeviceBuzzer &);
    DeviceBuzzer& operator = (const DeviceBuzzer &);

public:

    void On(void);
    void Off(void);
    void SetFreq(uint32_t freq);
    uint32_t GetFreq(void);

    static DeviceBuzzer* Instance();

};


#endif /* DEVICE_BUZZER_H_ */
