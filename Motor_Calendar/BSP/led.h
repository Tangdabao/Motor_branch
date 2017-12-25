#ifndef _LED_H_
#define _LED_H_

#include "stm32l0xx_hal.h"

/*  LED2��Ӧ��Nucleo������ĵ��û�LED */
#define LED_On()       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define LED_Off()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)

void LED_Init(void);

#endif
