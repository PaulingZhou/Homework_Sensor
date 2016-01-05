/**
  ******************************************************************************
  * @file    main.c 
  * @author  ZSH
  * @version V1.0.0
  * @date    2-January-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  *	The program of system board in RM2015 1X3, which includes mpu9250, usart for now.
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "spi1.h"
#include "mpu9250.h"
#include "usart.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);
void NVIC_Config(void);  
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t initSuccess;
	uint8_t mpuData[14];
	initSuccess = 0;
	NVIC_Config();
	STM_EVAL_COMInit();
	USART_Configuration(9600);
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
	initSuccess = MPU9250_Init();
  while (initSuccess)
  {
//		MPU9250_ReadValue();
		MPU9250_ReadValue(mpuData);
		USART_SendDataArray(USART1,mpuData);
		Delay(0xFFFF);
  }
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
	while(nCount--)
  {
  }
}


void NVIC_Config(void)  
{  
  NVIC_InitTypeDef NVIC_InitStructure;  
  
  /* Enable the USARTx Interrupt */  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);  
} 

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
