#include "led.h"

/*******************************************************************************
** 功能	      初始化LED引脚
              LED连接到PA5，LED2对应于Nucleo开发板的的用户LED              
** 参数       无
** 返回值	    无
** 注意
** 修改日志
*******************************************************************************/
void LED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  __GPIOA_CLK_ENABLE();

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;//失能内部上下拉电阻
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;//输出速度：快速
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

