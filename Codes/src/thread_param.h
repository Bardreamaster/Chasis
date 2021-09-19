/**
  ****************************************************************
  * @file    thread_param.h
  * @author  TofuLemon(5thzhengjiarandom@gmail.com)
  * @date    2020-Jan-12
  ****************************************************************
  */

#ifndef THREAD_PARAM_H_
#define THREAD_PARAM_H_

enum ThreadPrioityType{
    RC_UPDATER_PRIOITY = 2,
};

enum ThreadStackSizeType{
    RC_UPDATER_STACK_SIZE = 1024,
};

enum ThreadTimeSliceType{
    RC_UPDATER_TIMESLICE = 1,
};

enum ThreadControlPeriodType{
    RC_UPDATER_CONTROL_PERIOD = 7,
};

#endif /* THREAD_PARAM_H_ */
