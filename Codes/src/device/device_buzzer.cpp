#include "device/device_buzzer.h"

#include "util.h"

#include "bsp/bsp_buzzer.h"

DeviceBuzzer::DeviceBuzzer()
{
    bsp_buzzer_init();
    bsp_buzzer_off();
}

DeviceBuzzer* DeviceBuzzer::Instance()
{
    static DeviceBuzzer instance;
    return &instance;
}

void DeviceBuzzer::On(void)
{
    bsp_buzzer_on();
}

void DeviceBuzzer::Off(void)
{
    bsp_buzzer_off();
}

void DeviceBuzzer::SetFreq(uint32_t freq)
{
    DeviceBuzzer::Instance()->m_Freq = freq;
    bsp_buzzer_set_freq(freq);
}

uint32_t DeviceBuzzer::GetFreq(void)
{
    return DeviceBuzzer::Instance()->m_Freq;
}


static void set_buzzer_freq(int argc, char**argv)
{
    Buzzer->SetFreq(util_atoi(argv[1]));
}

static void buzzer_on(void)
{
    Buzzer->On();
}

static void buzzer_off(void)
{
    Buzzer->Off();
}
MSH_CMD_EXPORT(buzzer_on, turn on the buzzer);
MSH_CMD_EXPORT(buzzer_off, turn off the buzzer);
MSH_CMD_EXPORT(set_buzzer_freq, change the frequence of the buzzer);
