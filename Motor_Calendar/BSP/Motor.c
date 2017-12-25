#include "motor.h"


void Motor_GPIO_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin | MotorA1_Pin , GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, MotorB_Pin | MotorB1_Pin, GPIO_PIN_RESET);


    GPIO_InitStruct.Pin = MotorA_Pin | MotorA1_Pin ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = MotorB_Pin | MotorB1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}


void Motor_Z(void)
{
#if 1
    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_RESET);
    HAL_Delay(Time);//0001

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_RESET);
    HAL_Delay(Time); //0101

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_RESET);
    HAL_Delay(Time); //0100

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_RESET);
    HAL_Delay(Time); //0110

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_RESET);
    HAL_Delay(Time); //0010

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_SET);
    HAL_Delay(Time);  //1010

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_SET);
    HAL_Delay(Time);  //1000

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_SET);
    HAL_Delay(Time);  //1001
#else
        GPIOA->BSRR = 0X00000001;
        GPIOB->BSRR = 0X00000000;
        HAL_Delay(Time);   //0001 1

        GPIOA->BSRR = 0X00000001;
        GPIOB->BSRR = 0X00000001;
        HAL_Delay(Time);   //0101 2

        GPIOA->BSRR = 0X00010000;
        GPIOB->BSRR = 0X00000001;
        HAL_Delay(Time);   //0100 3

        GPIOA->BSRR = 0X00000002;
        GPIOB->BSRR = 0X00000001;
        HAL_Delay(Time);   //0110 4

        GPIOA->BSRR = 0X00000002;
        GPIOB->BSRR = 0X00010000;
        HAL_Delay(Time);   //0010 5

        GPIOA->BSRR = 0X00000002;
        GPIOB->BSRR = 0X00000002;
        HAL_Delay(Time);   //1010 6

        GPIOA->BSRR = 0X00020000;
        GPIOB->BSRR = 0X00000002;
        HAL_Delay(Time);   //1000 7

        GPIOA->BSRR = 0X00000001;
        GPIOB->BSRR = 0X00000002;
        HAL_Delay(Time);   //1001 8

#endif
}


void Motor_F(void)
{
    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_RESET);
    HAL_Delay(Time);//0001

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_SET);
    HAL_Delay(Time); //1001

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_SET);
    HAL_Delay(Time); //1000

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_SET);
    HAL_Delay(Time); //1010

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_RESET);
    HAL_Delay(Time); //0010

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_RESET);
    HAL_Delay(Time);  //0110

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_RESET);
    HAL_Delay(Time);  //0100

    HAL_GPIO_WritePin(GPIOA, MotorA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, MotorA1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, MotorB_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MotorB1_Pin, GPIO_PIN_RESET);
    HAL_Delay(Time);  //0101
}




