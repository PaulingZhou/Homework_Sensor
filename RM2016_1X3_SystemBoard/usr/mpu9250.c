#include <stdlib.h> 
#include <stdio.h> 
# include "mpu9250.h"
# include "spi1.h"

//S_INT16_XYZ MPU9250_ACC_LAST;
//S_INT16_XYZ MPU9250_GYRO_LAST;
//S_INT16_XYZ MPU9250_MAG_LAST;
//int16_t			MPU9250_TEMP_LAST;
uint8_t MPU9250_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	//使能GPIOB时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	SPI1_Config();	//MPU9250 SPI初始化
	if(MPU9250_Read_Reg(WHO_AM_I)==0x73){
		MPU9250_Write_Reg(USER_CTRL,0X10); //使能MPU9250SPI
		MPU9250_Write_Reg(PWR_MGMT_1,0X80);  //电源管理，复位MPU9250
		MPU9250_Write_Reg(SMPLRT_DIV,0x07);//采样率1000/(1+7)=125HZ
		MPU9250_Write_Reg(CONFIG,GYRO_BW_5);				//陀螺仪与温度低通滤波器 0x06 5hz
		MPU9250_Write_Reg(GYRO_CONFIG,GYRO_2000DPS);  //陀螺仪测量范围 0X18 +2000 dps
		MPU9250_Write_Reg(ACCEL_CONFIG,ACC_16G); //加速度计测量范围 0X18 +-16g
		MPU9250_Write_Reg(ACCEL_CONFIG2,ACC_BW_5);	//加速度计低通滤波器，5Hz
		return 1;
	}
	return 0;
}

//SPI读取指定寄存器
//reg:指定寄存器的地址
uint8_t MPU9250_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);					//使能SPI传输
	reg_val = SPI1_ReadWriteByte(reg|0x80);	 	//发送读命令+寄存器号
 	reg_val = SPI1_ReadWriteByte(0xFF);				//读取寄存器值
	GPIO_SetBits(GPIOB,GPIO_Pin_8);						//禁止SPI传输
	return(reg_val);
}

//SPI写寄存器
//reg:指定的寄存器地址
//value:写入的值
uint8_t MPU9250_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //使能SPI传输
	status=SPI1_ReadWriteByte(reg); //发送写命令+寄存器号
	SPI1_ReadWriteByte(value);//写入寄存器值
	GPIO_SetBits(GPIOB,GPIO_Pin_8);  //禁止MPU9250
	return(status);//返回状态值
}

////读取MPU9250数据
//void MPU9250_ReadValue(void)
//{
//	uint8_t MPU9250_buf[14],i;
//	GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //使能SPI传输
//	SPI1_ReadWriteByte(ACCEL_XOUT_H|0x80);
//	for(i=0;i<14;i++)//一共读取14字节的数据
//	{
//		MPU9250_buf[i]=SPI1_ReadWriteByte(0xFF); //循环读取
//	}
//	//加速度计
//	MPU9250_ACC_LAST.X = ((int16_t)MPU9250_buf[0]<<8) | MPU9250_buf[1];
//	MPU9250_ACC_LAST.Y = ((int16_t)MPU9250_buf[2]<<8) | MPU9250_buf[3];
//	MPU9250_ACC_LAST.Z = ((int16_t)MPU9250_buf[4]<<8) | MPU9250_buf[5];
//	//温度
//	MPU9250_TEMP_LAST =  ((int16_t)MPU9250_buf[6]<<8) | MPU9250_buf[7];
//	//陀螺仪
//	MPU9250_GYRO_LAST.X = ((int16_t)MPU9250_buf[8]<<8) | MPU9250_buf[9];
//	MPU9250_GYRO_LAST.Y = ((int16_t)MPU9250_buf[10]<<8) | MPU9250_buf[11];
//	MPU9250_GYRO_LAST.Z = ((int16_t)MPU9250_buf[12]<<8) | MPU9250_buf[13];
//	
//	GPIO_SetBits(GPIOB,GPIO_Pin_8);  //禁止MPU9250
//}

void MPU9250_ReadValue(uint8_t *dataPointer)
{
	uint8_t i;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //使能SPI传输
	SPI1_ReadWriteByte(ACCEL_XOUT_H|0x80);
	for(i=0;i<14;i++)//一共读取14字节的数据
	{
		*dataPointer = SPI1_ReadWriteByte(0xFF); //循环读取
		dataPointer = dataPointer+1;
	}
	GPIO_SetBits(GPIOB,GPIO_Pin_8);  //禁止MPU9250
}
