#include "uart.h"
#include "rcc.h"

UART_HandleTypeDef Uart1Handle;//用于UART1处理的结构体
UART_HandleTypeDef Uart2Handle;//用于UART2处理的结构体
UART_HandleTypeDef LpUart1Handle;//用于LPUART处理的结构体

/*******************************************************************************
以下内容为printf函数重定向至串口的代码，重定向至USART2,可修改
*******************************************************************************/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  while((USART2->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//等待发送寄存器为空
  UART2_SendByte(ch);
  return ch;
}
/*******************************************************************************
以上内容为printf函数重定向至串口的代码
*******************************************************************************/



/*******************************************************************************
** 功能	      USART1初始化
** 参数       无
** 返回值	    无
** 注意       若使用奇偶校验，则数据长度包含校验位
** 修改日志
*******************************************************************************/
void USART1_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* 选择USART时钟源 */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  RCC_PeriphCLKInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;//USART1时钟源配置选择为系统时钟 
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);    

  /* Enable GPIO TX/RX clock */
  USART1_TX_GPIO_CLK_ENABLE();
  USART1_RX_GPIO_CLK_ENABLE();

  /* Enable USART1 clock */
  USART1_CLK_ENABLE(); 

  __HAL_UART_DISABLE(&Uart1Handle);//失能USART

  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USART1_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USART1_TX_AF;
  HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USART1_RX_PIN;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = USART1_RX_AF;
  HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);

  
  /* 初始化USART参数  */  
  Uart1Handle.Instance        = USART1;
  Uart1Handle.Init.BaudRate   = 9600;//波特率
  Uart1Handle.Init.WordLength = UART_WORDLENGTH_8B;//数据长度：8位
  Uart1Handle.Init.StopBits   = UART_STOPBITS_1;
  Uart1Handle.Init.Parity     = UART_PARITY_NONE;
  Uart1Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;//无硬件流控制
  Uart1Handle.Init.Mode       = UART_MODE_TX_RX;//使能发送和接收功能

  if(HAL_UART_Init(&Uart1Handle) != HAL_OK)//初始化USART
  {
      
  }
  __HAL_UART_DISABLE(&Uart1Handle);//失能USART
  USART1->RQR = 1<<3;//clears the RXNE flag
  USART1->CR1 |= 1<<5;//enable the RXNE interrupt
  HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);//设置USART中断优先级:抢占优先级3；子优先级0（M0+内核无子优先级）
  HAL_NVIC_EnableIRQ(USART1_IRQn);//使能USART中断
  __HAL_UART_ENABLE(&Uart1Handle);//使能USART
}

/*******************************************************************************
** 功能	      USART1发送一字节数据
** 参数       data：将要发送的字节
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void UART1_SendByte(uint8_t data)
{
  while((USART1->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);
  USART1->TDR = data;
}

/*******************************************************************************
** 功能	      USART1发送字符串
** 参数       str：将要发送的字符串
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void UART1_SendString(char * str)
{
  while((*str)!=0)
  {
    while((USART1->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);
    USART1->TDR = *str++;       
  }
}

/*******************************************************************************
** 功能	      USART2初始化
** 参数       无
** 返回值	    无
** 注意       若使用奇偶校验，则数据长度包含校验位
** 修改日志
*******************************************************************************/
void USART2_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* 选择USART时钟源 */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  RCC_PeriphCLKInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;//USART时钟源配置选择为系统时钟   
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);    

  /* Enable GPIO TX/RX clock */
  USART2_TX_GPIO_CLK_ENABLE();
  USART2_RX_GPIO_CLK_ENABLE();

  /* Enable USART2 clock */
  USART2_CLK_ENABLE(); 

  __HAL_UART_DISABLE(&Uart2Handle);//失能USART

  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USART2_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USART2_TX_AF;
  HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USART2_RX_PIN;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = USART2_RX_AF;
  HAL_GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStruct);

  Uart2Handle.Instance        = USART2;
  Uart2Handle.Init.BaudRate   = 115200;//波特率
  Uart2Handle.Init.WordLength = UART_WORDLENGTH_8B;//数据长度：8位
  Uart2Handle.Init.StopBits   = UART_STOPBITS_1;
  Uart2Handle.Init.Parity     = UART_PARITY_NONE;
  Uart2Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;//无硬件流控制
  Uart2Handle.Init.Mode       = UART_MODE_TX_RX;//使能发送和接收功能

  if(HAL_UART_Init(&Uart2Handle) != HAL_OK)//初始化USART
  {
      
  }

  __HAL_UART_DISABLE(&Uart2Handle);//失能USART
  USART2->RQR = 1<<3;//clears the RXNE flag
  USART2->CR1 |= 1<<5;//enable the RXNE interrupt
  HAL_NVIC_SetPriority(USART2_IRQn, 3, 0);//设置USART中断优先级:抢占优先级3；子优先级0（M0+内核无子优先级）
  HAL_NVIC_EnableIRQ(USART2_IRQn);//使能USART中断
  __HAL_UART_ENABLE(&Uart2Handle);//使能USART
}

/*******************************************************************************
** 功能	      USART2发送一字节数据
** 参数       data：将要发送的字节
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void UART2_SendByte(uint8_t data)
{
  while((USART2->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//等待发送寄存器为空
  USART2->TDR = data;
}

/*******************************************************************************
** 功能	      USART2发送字符串
** 参数       str：将要发送的字符串
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void UART2_SendString(char * str)
{
  while((*str)!=0)
  {
    while((USART2->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//等待发送寄存器为空
    USART2->TDR = *str++;       
  }
}

void LPUART1_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* 使能LSE时钟 */  
  LSE_ClockEnable();

  /* 选择USART时钟源 */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
  RCC_PeriphCLKInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;//USART时钟源配置选择为LSE
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);    

  /* Enable GPIO TX/RX clock */
  LPUART1_TX_GPIO_CLK_ENABLE();
  LPUART1_RX_GPIO_CLK_ENABLE();

  /* Enable LPUART1 clock */
  LPUART1_CLK_ENABLE(); 

  __HAL_UART_DISABLE(&LpUart1Handle);//失能USART

  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = LPUART1_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = LPUART1_TX_AF;
  HAL_GPIO_Init(LPUART1_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = LPUART1_RX_PIN;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = LPUART1_RX_AF;
  HAL_GPIO_Init(LPUART1_RX_GPIO_PORT, &GPIO_InitStruct);

  LpUart1Handle.Instance        = LPUART1;
  LpUart1Handle.Init.BaudRate   = 9600;
  LpUart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
  LpUart1Handle.Init.StopBits   = UART_STOPBITS_1;
  LpUart1Handle.Init.Parity     = UART_PARITY_NONE;
  LpUart1Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  LpUart1Handle.Init.Mode       = UART_MODE_TX_RX;

  if(HAL_UART_Init(&LpUart1Handle) != HAL_OK)
  {
      
  }
  __HAL_UART_DISABLE(&LpUart1Handle);
  LPUART1->RQR = 1<<3;//clears the RXNE flag
  LPUART1->CR1 |= 1<<5;//enable the RXNE interrupt
  HAL_NVIC_SetPriority(LPUART1_IRQn, 3, 0);//设置USART中断优先级:抢占优先级3；子优先级0（M0+内核无子优先级）
  HAL_NVIC_EnableIRQ(LPUART1_IRQn);//使能USART中断
  __HAL_UART_ENABLE(&LpUart1Handle);//使能USART

}

/*******************************************************************************
** 功能	      LPUSART1发送一字节数据
** 参数       data：将要发送的字节
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void LPUART1_SendByte(uint8_t data)
{
  while((LPUART1->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//等待发送寄存器为空
  LPUART1->TDR = data;
}

/*******************************************************************************
** 功能	      LPUSART1发送字符串
** 参数       str：将要发送的字符串
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void LPUART1_SendString(char * str)
{
  while((*str)!=0)
  {
    while((LPUART1->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//等待发送寄存器为空
    LPUART1->TDR = *str++;       
  }
}


