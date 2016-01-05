/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"

void delay(void);

/** 
  * @brief  Configures COM port. 
  * @param  COM: Specifies the COM port to be configured. 
  *   This parameter can be one of following parameters:     
  *     @arg COM1 
  *     @arg COM2   
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that 
  *   contains the configuration information for the specified USART peripheral. 
  * @retval None 
  */  
void STM_EVAL_COMInit(void)  
{  
  GPIO_InitTypeDef GPIO_InitStructure;  
  
  /* Enable GPIO clock */  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
  /* Enable UART clock */  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
   
  /* Connect PXx to USARTx_Tx*/  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);  
  /* Connect PXx to USARTx_Rx*/  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);  
  
  /* Configure USART Tx as alternate function  */  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  
  /* Configure USART Rx as alternate function  */  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
} 

/******************************************************************************* 
* Function Name  : USART_Configuration 
* Description    : Configures the USART1. 
* Input          : BaudRate 
* Output         : None 
* Return         : None 
*******************************************************************************/ 
void USART_Configuration(int BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  Transmits single data through the USARTx peripheral.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 or 6 to select the USART or 
  *         UART peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void USART_SendDataArray(USART_TypeDef* USARTx,uint8_t *arrayPointer)
{
	uint8_t length = 14,i,checksum=0x00;
  /* Check the parameters */
//  assert_param(IS_USART_ALL_PERIPH(USARTx));
//  assert_param(IS_USART_DATA(MPU9250_buf)); 
    
  /* Transmit Data */
	USART_SendData(USART1,0xAA);
	delay();
	USART_SendData(USART1,0xBB);
	delay();
	USART_SendData(USART1,length);
	delay();
	checksum^=length;
	for(i=0;i<length;i++)
	{
		USART_SendData(USART1,*arrayPointer);
		delay();
		checksum^=*arrayPointer;
		arrayPointer = arrayPointer+1;
	}
	USART_SendData(USART1,checksum);
	delay();
}

void delay(void)
{
	uint16_t count = 0xFFFF;
	while(count--)
  {
		;
  }
}
