#include "led.h"

/*******************************************************************************
** ����	      ��ʼ��LED����
              LED���ӵ�PA5��LED2��Ӧ��Nucleo������ĵ��û�LED              
** ����       ��
** ����ֵ	    ��
** ע��
** �޸���־
*******************************************************************************/
void LED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  __GPIOA_CLK_ENABLE();

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//�������
  GPIO_InitStruct.Pull = GPIO_NOPULL;//ʧ���ڲ�����������
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;//����ٶȣ�����
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

