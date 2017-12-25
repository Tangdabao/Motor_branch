#include "rtc.h"
#include "rcc.h"
#include "uart.h"

#define BKUPRead_Data 0x2339

RTC_HandleTypeDef RtcHandle;//����RTC����Ľṹ��
void RTC_NVIC_Config(uint32_t PreemptPriority, uint32_t SubPriority);
/*******************************************************************************
** ����	      RTC��ʼ��
** ����       ��
** ����ֵ	    ��
** ע��       ��
** �޸���־
*******************************************************************************/
void RTC_Init(void)
{
    /* RTC���������������� */
    RTC_BaseConfig();

}

void RTC_BaseConfig(void)
{
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    /* ʹ��LSEʱ�� */
    LSE_ClockEnable();

    /* ����RTCʱ��Դ */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;//ʹ��LSE��ΪRTCʱ��Դ
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {

    }

    /* ʹ��RTCʱ�� */
    __HAL_RCC_RTC_ENABLE();

    /* ����RTC�������� */
    RtcHandle.Instance = RTC;
    RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;//Сʱ��ʽ
    RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;//�첽Ԥ��Ƶ
    RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;//ͬ��Ԥ��Ƶ
    RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;//RTC����������
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;//�������
    RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;//���ģʽ����©


    if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
    {
        /* Error */
    }

    /* ͨ�����ݼĴ�������û��Զ����Ǽ��RTC�Ƿ��Ѿ����ù�ʱ�� */
    if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR0) != BKUPRead_Data)//δ����ʱ��
    {
        /* Configure RTC Calendar */
        
        RTC_CalendarConfig();

    }
}

/*******************************************************************************
** ����	      ����RTC���ں�ʱ��
** ����       ��
** ����ֵ	    ��
** ע��       ��
** �޸���־
*******************************************************************************/
void RTC_CalendarConfig(void)
{
    RTC_DateTypeDef sdatestructure;
    RTC_TimeTypeDef stimestructure;

	  RTC_Set_Alarm(RTC_WEEKDAY_MONDAY);///������������///
	
    /* Set Date: Monday DECEMBER 25th 2017 */
    sdatestructure.Year = 0x17;
    sdatestructure.Month = RTC_MONTH_DECEMBER;
    sdatestructure.Date = 0x25;
    sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
    if(HAL_RTC_SetDate(&RtcHandle, &sdatestructure, FORMAT_BCD) != HAL_OK)
    {
        /* Error */
    }
     /* Set Time: 00��01��00 */
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

    /* д���ñ�ǵ�RTC���ݼĴ���0 */
    HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR0, BKUPRead_Data);
}


/*******************************************************************************
** ����	      ����RTC��������
** ����       period���������� = period+1 ��λ��s
** ����ֵ	    ��
** ע��       ��
** �޸���־
*******************************************************************************/
#if 0
void RTC_WakeUpTimerConfig(uint16_t period)
{
    /* ��ʧ�ܻ��Ѷ�ʱ�� */
    // HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);
    /* ������ѱ�־ */
    //__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RtcHandle, RTC_FLAG_WUTF);
    /* ���û��Ѷ�ʱ���������жϷ�ʽ */
    //HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, period, RTC_WAKEUPCLOCK_CK_SPRE_16BITS); //����������ʱ��Դ
    /* ���û��Ѷ�ʱ���ж� */
    HAL_NVIC_SetPriority(RTC_IRQn, 0x0, 0);//�����ж����ȼ�
    HAL_NVIC_EnableIRQ(RTC_IRQn);//ʹ��RTC�ж�
}
#endif



void RTC_NVIC_Config(uint32_t PreemptPriority, uint32_t SubPriority)
{
    HAL_NVIC_SetPriority(RTC_IRQn, PreemptPriority, SubPriority);//�����ж����ȼ�
    HAL_NVIC_EnableIRQ(RTC_IRQn);//ʹ��RTC�ж�
}


void RTC_Set_Alarm(uint8_t Weekday_Sel)
{
    RTC_AlarmTypeDef sAlarmstructure;
     /* Set A����Alarm: 00��01��20  WEEKDAYn */
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
		/* Set B����Alarm: 00��01��20 WEEKDAYn+1 */
    sAlarmstructure.AlarmTime.Minutes = 0x01;
    sAlarmstructure.AlarmTime.Seconds = 0x40;
    sAlarmstructure.AlarmDateWeekDay = Weekday_Sel + 1;
    sAlarmstructure.Alarm = RTC_ALARM_B;
    if (HAL_RTC_SetAlarm_IT(&RtcHandle, &sAlarmstructure, FORMAT_BCD) != HAL_OK)
    {
    }
}




