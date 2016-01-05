#include "stm32f4_discovery.h"

void STM_EVAL_COMInit(void);
void USART_Configuration(int BaudRate);
void USART_SendDataArray(USART_TypeDef* USARTx,uint8_t *arrayPointer);
