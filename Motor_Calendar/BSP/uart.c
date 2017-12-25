#include "uart.h"
#include "rcc.h"

UART_HandleTypeDef Uart1Handle;//����UART1����Ľṹ��
UART_HandleTypeDef Uart2Handle;//����UART2����Ľṹ��
UART_HandleTypeDef LpUart1Handle;//����LPUART����Ľṹ��

/*******************************************************************************
��������Ϊprintf�����ض��������ڵĴ��룬�ض�����USART2,���޸�
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
  while((USART2->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//�ȴ����ͼĴ���Ϊ��
  UART2_SendByte(ch);
  return ch;
}
/*******************************************************************************
��������Ϊprintf�����ض��������ڵĴ���
*******************************************************************************/



/*******************************************************************************
** ����	      USART1��ʼ��
** ����       ��
** ����ֵ	    ��
** ע��       ��ʹ����żУ�飬�����ݳ��Ȱ���У��λ
** �޸���־
*******************************************************************************/
void USART1_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* ѡ��USARTʱ��Դ */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  RCC_PeriphCLKInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;//USART1ʱ��Դ����ѡ��Ϊϵͳʱ�� 
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);    

  /* Enable GPIO TX/RX clock */
  USART1_TX_GPIO_CLK_ENABLE();
  USART1_RX_GPIO_CLK_ENABLE();

  /* Enable USART1 clock */
  USART1_CLK_ENABLE(); 

  __HAL_UART_DISABLE(&Uart1Handle);//ʧ��USART

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

  
  /* ��ʼ��USART����  */  
  Uart1Handle.Instance        = USART1;
  Uart1Handle.Init.BaudRate   = 9600;//������
  Uart1Handle.Init.WordLength = UART_WORDLENGTH_8B;//���ݳ��ȣ�8λ
  Uart1Handle.Init.StopBits   = UART_STOPBITS_1;
  Uart1Handle.Init.Parity     = UART_PARITY_NONE;
  Uart1Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;//��Ӳ��������
  Uart1Handle.Init.Mode       = UART_MODE_TX_RX;//ʹ�ܷ��ͺͽ��չ���

  if(HAL_UART_Init(&Uart1Handle) != HAL_OK)//��ʼ��USART
  {
      
  }
  __HAL_UART_DISABLE(&Uart1Handle);//ʧ��USART
  USART1->RQR = 1<<3;//clears the RXNE flag
  USART1->CR1 |= 1<<5;//enable the RXNE interrupt
  HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);//����USART�ж����ȼ�:��ռ���ȼ�3�������ȼ�0��M0+�ں��������ȼ���
  HAL_NVIC_EnableIRQ(USART1_IRQn);//ʹ��USART�ж�
  __HAL_UART_ENABLE(&Uart1Handle);//ʹ��USART
}

/*******************************************************************************
** ����	      USART1����һ�ֽ�����
** ����       data����Ҫ���͵��ֽ�
** ����ֵ	    ��
** ע��       
** �޸���־
*******************************************************************************/
void UART1_SendByte(uint8_t data)
{
  while((USART1->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);
  USART1->TDR = data;
}

/*******************************************************************************
** ����	      USART1�����ַ���
** ����       str����Ҫ���͵��ַ���
** ����ֵ	    ��
** ע��       
** �޸���־
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
** ����	      USART2��ʼ��
** ����       ��
** ����ֵ	    ��
** ע��       ��ʹ����żУ�飬�����ݳ��Ȱ���У��λ
** �޸���־
*******************************************************************************/
void USART2_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* ѡ��USARTʱ��Դ */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  RCC_PeriphCLKInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;//USARTʱ��Դ����ѡ��Ϊϵͳʱ��   
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);    

  /* Enable GPIO TX/RX clock */
  USART2_TX_GPIO_CLK_ENABLE();
  USART2_RX_GPIO_CLK_ENABLE();

  /* Enable USART2 clock */
  USART2_CLK_ENABLE(); 

  __HAL_UART_DISABLE(&Uart2Handle);//ʧ��USART

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
  Uart2Handle.Init.BaudRate   = 115200;//������
  Uart2Handle.Init.WordLength = UART_WORDLENGTH_8B;//���ݳ��ȣ�8λ
  Uart2Handle.Init.StopBits   = UART_STOPBITS_1;
  Uart2Handle.Init.Parity     = UART_PARITY_NONE;
  Uart2Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;//��Ӳ��������
  Uart2Handle.Init.Mode       = UART_MODE_TX_RX;//ʹ�ܷ��ͺͽ��չ���

  if(HAL_UART_Init(&Uart2Handle) != HAL_OK)//��ʼ��USART
  {
      
  }

  __HAL_UART_DISABLE(&Uart2Handle);//ʧ��USART
  USART2->RQR = 1<<3;//clears the RXNE flag
  USART2->CR1 |= 1<<5;//enable the RXNE interrupt
  HAL_NVIC_SetPriority(USART2_IRQn, 3, 0);//����USART�ж����ȼ�:��ռ���ȼ�3�������ȼ�0��M0+�ں��������ȼ���
  HAL_NVIC_EnableIRQ(USART2_IRQn);//ʹ��USART�ж�
  __HAL_UART_ENABLE(&Uart2Handle);//ʹ��USART
}

/*******************************************************************************
** ����	      USART2����һ�ֽ�����
** ����       data����Ҫ���͵��ֽ�
** ����ֵ	    ��
** ע��       
** �޸���־
*******************************************************************************/
void UART2_SendByte(uint8_t data)
{
  while((USART2->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//�ȴ����ͼĴ���Ϊ��
  USART2->TDR = data;
}

/*******************************************************************************
** ����	      USART2�����ַ���
** ����       str����Ҫ���͵��ַ���
** ����ֵ	    ��
** ע��       
** �޸���־
*******************************************************************************/
void UART2_SendString(char * str)
{
  while((*str)!=0)
  {
    while((USART2->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//�ȴ����ͼĴ���Ϊ��
    USART2->TDR = *str++;       
  }
}

void LPUART1_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* ʹ��LSEʱ�� */  
  LSE_ClockEnable();

  /* ѡ��USARTʱ��Դ */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
  RCC_PeriphCLKInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;//USARTʱ��Դ����ѡ��ΪLSE
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);    

  /* Enable GPIO TX/RX clock */
  LPUART1_TX_GPIO_CLK_ENABLE();
  LPUART1_RX_GPIO_CLK_ENABLE();

  /* Enable LPUART1 clock */
  LPUART1_CLK_ENABLE(); 

  __HAL_UART_DISABLE(&LpUart1Handle);//ʧ��USART

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
  HAL_NVIC_SetPriority(LPUART1_IRQn, 3, 0);//����USART�ж����ȼ�:��ռ���ȼ�3�������ȼ�0��M0+�ں��������ȼ���
  HAL_NVIC_EnableIRQ(LPUART1_IRQn);//ʹ��USART�ж�
  __HAL_UART_ENABLE(&LpUart1Handle);//ʹ��USART

}

/*******************************************************************************
** ����	      LPUSART1����һ�ֽ�����
** ����       data����Ҫ���͵��ֽ�
** ����ֵ	    ��
** ע��       
** �޸���־
*******************************************************************************/
void LPUART1_SendByte(uint8_t data)
{
  while((LPUART1->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//�ȴ����ͼĴ���Ϊ��
  LPUART1->TDR = data;
}

/*******************************************************************************
** ����	      LPUSART1�����ַ���
** ����       str����Ҫ���͵��ַ���
** ����ֵ	    ��
** ע��       
** �޸���־
*******************************************************************************/
void LPUART1_SendString(char * str)
{
  while((*str)!=0)
  {
    while((LPUART1->ISR&UART_FLAG_TXE)!=UART_FLAG_TXE);//�ȴ����ͼĴ���Ϊ��
    LPUART1->TDR = *str++;       
  }
}


