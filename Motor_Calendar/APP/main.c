/*******************************************************************************
** 可设置参数：闹钟日期——————》》》》》RTC_Set_Alarm(RTC_WEEKDAY_MONDAY);
**             正反转圈数：#define Ring_num 50
*******************************************************************************/

#include "main.h"
#include "rcc.h"
#include "led.h"
#include "uart.h"
#include "rtc.h"
#include "motor.h"

#define Ring_num 50
#define Z_Flag   1
#define F_Flag   2

RTC_DateTypeDef RTC_Date;
RTC_TimeTypeDef RTC_Time;

uint8_t Flag = 0;
uint8_t Ring_num_Flag = 0;

int main(void)
{
    uint32_t Count_Last = 0;
    HAL_Init();
    SystemClock_Config();
    SystemCoreClockUpdate();
    LED_Init();
    LED_Off();
    Motor_GPIO_Init();
    USART2_Init();
    printf("uart test\r\n");
    RTC_Init();
    //RTC_Set_Alarm(RTC_WEEKDAY_MONDAY);
    while(1)
    {

        HAL_RTC_GetDate(&RtcHandle, &RTC_Date, FORMAT_BIN);
        HAL_RTC_GetTime(&RtcHandle, &RTC_Time, FORMAT_BIN);
        if(Flag != 0)
        {
            printf("RTC_Time:  %2d:%2d:%2d\r\n", RTC_Time.Hours, RTC_Time.Minutes, RTC_Time.Seconds);

            if(Flag == Z_Flag)
            {

                do
                {
                    Ring_num_Flag++;
                    Motor_Z();
                }
                while(Ring_num_Flag < Ring_num);
                printf("Motor_Z Success\r\n");

            }
            else if(Flag == F_Flag)
            {

                do
                {
                    Ring_num_Flag++;
                    Motor_F();
                }
                while(Ring_num_Flag < Ring_num);
                printf("Motor_F Success\r\n");

            }
            printf("Motor Ring_num:%d\r\n", Ring_num_Flag);
            Ring_num_Flag = 0;
            Flag = 0;
        }

        if(Count_Last != RTC_Time.Minutes)
        {
            Count_Last = RTC_Time.Minutes;
            printf("RTC_Date:  20%2d.%2d.%2d  星期%d\r\n", RTC_Date.Year, RTC_Date.Month, RTC_Date.Date, RTC_Date.WeekDay);
            printf("RTC_Time:  %2d:%2d:%2d\r\n", RTC_Time.Hours, RTC_Time.Minutes, RTC_Time.Seconds);
        }

    }
}


void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    LED_On();
    Flag = Z_Flag;
    printf("This is Test--A\r\n");

}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{

    LED_Off();
    Flag = F_Flag;
    printf("This is Test--B\r\n");
    //RTC_New_Alarm();
    //printf("This is RTC_Set_Alarm\r\n");
}
