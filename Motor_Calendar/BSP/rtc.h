#ifndef _RTC_H_
#define _RTC_H_

#include "stm32l0xx_hal.h"

#define RTC_CLOCK_SOURCE_LSE

#ifdef RTC_CLOCK_SOURCE_LSI
  #define RTC_ASYNCH_PREDIV  0x7F
  #define RTC_SYNCH_PREDIV   0x0130
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
  #define RTC_ASYNCH_PREDIV  0x7F
  #define RTC_SYNCH_PREDIV   0x00FF
#endif

extern RTC_HandleTypeDef RtcHandle;

void RTC_Init(void);
void RTC_CalendarConfig(void);
void RTC_BaseConfig(void);
void RTC_New_Alarm(void);
void RTC_WakeUpTimerConfig(uint16_t period);
void RTC_Set_Alarm(uint8_t Weekday_Sel);
#endif
