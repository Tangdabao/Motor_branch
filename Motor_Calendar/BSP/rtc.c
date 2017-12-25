#include "rtc.h"
#include "rcc.h"
#include "uart.h"

#define BKUPRead_Data 0x2339

RTC_HandleTypeDef RtcHandle;//用于RTC处理的结构体
void RTC_NVIC_Config(uint32_t PreemptPriority, uint32_t SubPriority);
/*******************************************************************************
** 功能	      RTC初始化
** 参数       无
** 返回值	    无
** 注意       无
** 修改日志
*******************************************************************************/
void RTC_Init(void)
{
    /* RTC基本计数参数配置 */
    RTC_BaseConfig();

}

void RTC_BaseConfig(void)
{
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    /* 使能LSE时钟 */
    LSE_ClockEnable();

    /* 配置RTC时钟源 */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;//使用LSE作为RTC时钟源
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {

    }

    /* 使能RTC时钟 */
    __HAL_RCC_RTC_ENABLE();

    /* 配置RTC基本参数 */
    RtcHandle.Instance = RTC;
    RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;//小时格式
    RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;//异步预分频
    RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;//同步预分频
    RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;//RTC输出：不输出
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;//输出极性
    RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;//输出模式：开漏


    if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
    {
        /* Error */
    }

    /* 通过备份寄存器里的用户自定义标记检测RTC是否已经配置过时钟 */
    if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR0) != BKUPRead_Data)//未配置时钟
    {
        /* Configure RTC Calendar */
        
        RTC_CalendarConfig();

    }
}

/*******************************************************************************
** 功能	      配置RTC日期和时钟
** 参数       无
** 返回值	    无
** 注意       无
** 修改日志
*******************************************************************************/
void RTC_CalendarConfig(void)
{
    RTC_DateTypeDef sdatestructure;
    RTC_TimeTypeDef stimestructure;

	  RTC_Set_Alarm(RTC_WEEKDAY_MONDAY);///设置闹钟日期///
	
    /* Set Date: Monday DECEMBER 25th 2017 */
    sdatestructure.Year = 0x17;
    sdatestructure.Month = RTC_MONTH_DECEMBER;
    sdatestructure.Date = 0x25;
    sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
    if(HAL_RTC_SetDate(&RtcHandle, &sdatestructure, FORMAT_BCD) != HAL_OK)
    {
        /* Error */
    }
     /* Set Time: 00：01：00 */
    stimestructure.Hours = 0x00;
    stimestructure.Minutes = 0x01;
    stimestructure.Seconds = 0x00;
    stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
    stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
    if(HAL_RTC_SetTime(&RtcHandle, &stimestructure, FORMAT_BCD) != HAL_OK)
    {
        /* Error */
    }

    RTC_NVIC_Config(0x00, 0x00);

    /* 写配置标记到RTC备份寄存器0 */
    HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR0, BKUPRead_Data);
}


/*******************************************************************************
** 功能	      配置RTC唤醒特性
** 参数       period：唤醒周期 = period+1 单位：s
** 返回值	    无
** 注意       无
** 修改日志
*******************************************************************************/
#if 0
void RTC_WakeUpTimerConfig(uint16_t period)
{
    /* 先失能唤醒定时器 */
    // HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);
    /* 清除唤醒标志 */
    //__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RtcHandle, RTC_FLAG_WUTF);
    /* 配置唤醒定时器参数，中断方式 */
    //HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, period, RTC_WAKEUPCLOCK_CK_SPRE_16BITS); //配置周期与时钟源
    /* 配置唤醒定时器中断 */
    HAL_NVIC_SetPriority(RTC_IRQn, 0x0, 0);//设置中断优先级
    HAL_NVIC_EnableIRQ(RTC_IRQn);//使能RTC中断
}
#endif



void RTC_NVIC_Config(uint32_t PreemptPriority, uint32_t SubPriority)
{
    HAL_NVIC_SetPriority(RTC_IRQn, PreemptPriority, SubPriority);//设置中断优先级
    HAL_NVIC_EnableIRQ(RTC_IRQn);//使能RTC中断
}


void RTC_Set_Alarm(uint8_t Weekday_Sel)
{
    RTC_AlarmTypeDef sAlarmstructure;
     /* Set A——Alarm: 00：01：20  WEEKDAYn */
    sAlarmstructure.AlarmTime.Hours = 0x00;
    sAlarmstructure.AlarmTime.Minutes = 0x01;
    sAlarmstructure.AlarmTime.Seconds = 0x20;
    sAlarmstructure.AlarmTime.TimeFormat = RTC_HOURFORMAT12_PM;
    sAlarmstructure.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarmstructure.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarmstructure.AlarmMask = RTC_ALARMMASK_NONE;
    sAlarmstructure.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_None;
    sAlarmstructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
    sAlarmstructure.AlarmDateWeekDay = Weekday_Sel;
    sAlarmstructure.Alarm = RTC_ALARM_A;
    if (HAL_RTC_SetAlarm_IT(&RtcHandle, &sAlarmstructure, FORMAT_BCD) != HAL_OK)
    {
    }
		/* Set B——Alarm: 00：01：20 WEEKDAYn+1 */
    sAlarmstructure.AlarmTime.Minutes = 0x01;
    sAlarmstructure.AlarmTime.Seconds = 0x40;
    sAlarmstructure.AlarmDateWeekDay = Weekday_Sel + 1;
    sAlarmstructure.Alarm = RTC_ALARM_B;
    if (HAL_RTC_SetAlarm_IT(&RtcHandle, &sAlarmstructure, FORMAT_BCD) != HAL_OK)
    {
    }
}




