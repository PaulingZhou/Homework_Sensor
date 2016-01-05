/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "spi1.h"


/******************************************************************************* 
* Function Name  : SPI_Config 
* Description    : Config the SPI1	
* Pin Map				 : PB3-->SPI1_SCK;	
*									 PB4-->SPI1_MISO; 	
* 								 PB5-->SPI1_MOSI; 
* Input          : None 
* Output         : None 
* Return         : None 
*******************************************************************************/
 void SPI1_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_I2S_DeInit(SPI1);

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);		//使能SPI1时钟
	
	//GPIOPB3,4,5初始化设置：复用功能输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);	//PB3复用为SPI1_SCK
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);	//PB4复用为SPI1_MISO
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);	//PB5复用为SPI1_MOSI
	
	//Init SPI1 & Set Working Mode
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										//SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;													//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;												//第二个跳变沿数据采样（下降沿）
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														//NSS信号由软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//预分频4，传输速度84M/32=2.625Mhz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRC值计算的多项式
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);
	
	SPI1_ReadWriteByte(0xFF);
}

/******************************************************************************* 
* Function Name  : SPI_SetSpeed 
* Description    : SPI Speed = fAPB2/分频系数;fAPB2时钟一般为84Mhz	 
* Input          : @ref SPI_BaudRate_Prescaler
*									 SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
* Output         : None 
* Return         : None 
*******************************************************************************/
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//判断有效性
	SPI1->CR1 &= 0xFFC7;		//位3~5清零，用来设置波特率
	SPI1->CR2 |= SPI_BaudRatePrescaler;		//设置SPI1速度
	SPI_Cmd(SPI1,ENABLE);
}

/******************************************************************************* 
* Function Name  : SPI_ReadWriteByte(u8 TxData) 
* Description    : 读写数据	 
* Input          : TxData
* Output         : RxData 
* Return         : RxData 
*******************************************************************************/
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
	uint8_t retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry>200)return 0;
	}
	SPI_I2S_SendData(SPI1,TxData);
	retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry>200)return 0;
	}
	return SPI_I2S_ReceiveData(SPI1);
}
