#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32l0xx_hal.h"

#define Time  5

#define MotorA_Pin GPIO_PIN_0
#define MotorA_GPIO_Port GPIOA

#define MotorA1_Pin GPIO_PIN_1
#define MotorA1_GPIO_Port GPIOA

#define MotorB_Pin GPIO_PIN_0
#define MotorB_GPIO_Port GPIOB

#define MotorB1_Pin GPIO_PIN_1
#define MotorB1_GPIO_Port GPIOB


void Motor_GPIO_Init(void);
void Motor_Z(void);
void Motor_F(void);

#endif
