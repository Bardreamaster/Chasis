#include "util_rtos.h"
#include "util.h"

#include "device/device_buzzer.h"
#include "device/device_rc.h"

#include "RcUpdater.h"

#include "bsp/bsp_can.h"
#include "device/device_m3508.h"

DeviceM3508 testmotor(CAN_M2_ID, CAN1);
DeviceM3508 testmotor2(CAN_M6_ID, CAN1);
DeviceBuzzer* dbg_buzzer_ins;
DeviceRC* dbg_rc_ins;


static char thread2_stack[1024];
rtos::Thread thread2("test");

PcKeyType test_chord[] = {PC_KEY_Q, PC_KEY_E, PC_M_L, PC_M_R};
ChordDetector test_chord_detector(test_chord, sizeof(test_chord)/sizeof(PcKeyType));

PcKeyType test_tap[] = {PC_KEY_B};
RapidTapDector rapid_tap_test(test_tap, 200, 5);

PcKeyType test_seq[] = {PC_KEY_W, PC_KEY_W, PC_KEY_S, PC_KEY_S, PC_KEY_A, PC_KEY_D, PC_KEY_A, PC_KEY_D, PC_KEY_E, PC_KEY_Q};
SequenceDetector seq_test(test_seq, sizeof(test_seq)/sizeof(PcKeyType), 250, 1000);

static void thread2_entry(void *parameter)
{
    bool last_B_result = false, B_result = false;
    bool last_seq_res = false, seq_res = false;
    uint32_t Q_last_update;

    testmotor.SetControlPeriodMs(2);
    testmotor.GetSpdPidInstance()->SetKp(3135.0f);
    testmotor.GetSpdPidInstance()->SetKi(1.0f);
    testmotor.GetSpdPidInstance()->SetKd(95.0f);
    testmotor.GetSpdPidInstance()->SetMaxOut(10000.0f);
    testmotor.GetSpdPidInstance()->SetMaxIout(500.0f);
    testmotor.GetSpdPidInstance()->Init();
    testmotor.Init();

    testmotor2.SetControlPeriodMs(2);
    testmotor2.GetSpdPidInstance()->SetKp(3135.0f);
    testmotor2.GetSpdPidInstance()->SetKi(1.0f);
    testmotor2.GetSpdPidInstance()->SetKd(95.0f);
    testmotor2.GetSpdPidInstance()->SetMaxOut(10000.0f);
    testmotor2.GetSpdPidInstance()->SetMaxIout(500.0f);
    testmotor2.GetSpdPidInstance()->Init();
    testmotor2.Init();

    test_chord_detector.Attach();
    rapid_tap_test.Attach();
    seq_test.Attach();  

    while (1)
    {
        testmotor.Update();
        testmotor2.Update();

        if(Q_last_update != RcUpdater->GetTickLastUpdate()){
            Q_last_update = RcUpdater->GetTickLastUpdate();
            last_B_result = B_result;
            B_result = rapid_tap_test.GetEventState();
    
            last_seq_res =seq_res;
            seq_res = seq_test.GetEventState(); 
    
		    piano();
    
            if(RC->QuerySwState(RC_SW_L, RC_SW_M2D)){
                rt_kprintf("Left Switch From Mid to Down\n");
            }
    
            if(RC->QuerySwState(RC_SW_L, RC_SW_D2M)){
                rt_kprintf("Left Switch From Down to Mid\n");
            }
    
            if(RC->QuerySwState(RC_SW_L, RC_SW_U2M)){
                rt_kprintf("Left Switch From Up to Mid\n");
            }
    
            if(RC->QuerySwState(RC_SW_L, RC_SW_M2U)){
                rt_kprintf("Left Switch From Mid to Up\n");
            }
    
            if(RC->QuerySwState(RC_SW_R, RC_SW_M2D)){
                rt_kprintf("Right Switch From Mid to Down\n");
            }		
            if(RC->QuerySwState(RC_SW_R, RC_SW_M2U)){
                rt_kprintf("Right M2U\n");
            }
    
		    if(test_chord_detector.GetEventState()){
		    	rt_kprintf("Peek mode active!\n");
		    }
    
            if(B_result && !last_B_result){
		    	rt_kprintf("Rush B!No Stop!\n");
		    }
            else if(!B_result && last_B_result){
		    	rt_kprintf("GG!My friend!\n");
		    }
    
            if(seq_res && !last_seq_res){
		    	rt_kprintf("30 Extra life scquired.\n");
		    }
            else if(!seq_res && last_seq_res){
		    	//rt_kprintf("Cheat Deactivated.\n");
		    }
        }

        rtos::delay_ms(2);
    }
    
}

void piano(void)
{
        if(RC->QueryPcKeyState(PC_KEY_Q, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_W, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_E, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_R, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_A, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_S, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_D, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_F, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_G, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_Z, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_X, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_C, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_V, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_B, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_SHIFT, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_KEY_CTRL, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_M_L, KEY_DOWN)
         ||RC->QueryPcKeyState(PC_M_R, KEY_DOWN)){
            Buzzer->On();
        }
        else{
            Buzzer->Off();
        }

        if(RC->QueryPcKeyState(PC_KEY_Q, KEY_FALL)){
            Buzzer->SetFreq(440);
            rt_kprintf("Q pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_W, KEY_FALL)){
            Buzzer->SetFreq(466);
            rt_kprintf("W pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_E, KEY_FALL)){
            Buzzer->SetFreq(523);
            rt_kprintf("E pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_R, KEY_FALL)){
            Buzzer->SetFreq(589);
            rt_kprintf("R pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_A, KEY_FALL)){
            Buzzer->SetFreq(622);
            rt_kprintf("A pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_S, KEY_FALL)){
            Buzzer->SetFreq(659);
            rt_kprintf("S pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_D, KEY_FALL)){
            Buzzer->SetFreq(698);
            rt_kprintf("D pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_F, KEY_FALL)){
            Buzzer->SetFreq(740);
            rt_kprintf("F pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_G, KEY_FALL)){
            Buzzer->SetFreq(784);
            rt_kprintf("G pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_Z, KEY_FALL)){
            Buzzer->SetFreq(831);
            rt_kprintf("Z pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_X, KEY_FALL)){
            Buzzer->SetFreq(880);
            rt_kprintf("X pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_C, KEY_FALL)){
            Buzzer->SetFreq(932);
            rt_kprintf("C pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_V, KEY_FALL)){
            Buzzer->SetFreq(988);
            rt_kprintf("V pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_B, KEY_FALL)){
            Buzzer->SetFreq(1047);
            rt_kprintf("B pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_SHIFT, KEY_FALL)){
            Buzzer->SetFreq(1109);
            rt_kprintf("SHIFT pressed\n");
        }

        if(RC->QueryPcKeyState(PC_KEY_CTRL, KEY_FALL)){
            Buzzer->SetFreq(1175);
            rt_kprintf("CTRL pressed\n");
        }

        if(RC->QueryPcKeyState(PC_M_L, KEY_FALL)){
            Buzzer->SetFreq(1245);
            rt_kprintf("M1 pressed\n");
        }

        if(RC->QueryPcKeyState(PC_M_R, KEY_FALL)){
            Buzzer->SetFreq(1319);
            rt_kprintf("M2 pressed\n");
        }  

}


int main(void)
{
	
	/*
    Buzzer->SetFreq(440);
    Buzzer->On();
	*/
    dbg_buzzer_ins = Buzzer;
    dbg_rc_ins = RC;
    
	/* test*/
		bsp_can2_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_11tq, 3, CAN_Mode_Normal);
		/* test end*/
    RcUpdater->Init();
    bsp_can1_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_11tq, 3, CAN_Mode_Normal);

    thread2.SetEntry(thread2_entry);
    thread2.SetParameter(RT_NULL);
    thread2.SetPriority(20);
    thread2.SetStackStart(thread2_stack);
    thread2.SetStackSize(sizeof(thread2_stack));
    thread2.SetTick(1);
    thread2.Init();

    rtos::delay_ms(3000);
    /*Buzzer->Off();*/
}
