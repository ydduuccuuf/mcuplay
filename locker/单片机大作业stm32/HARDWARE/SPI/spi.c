#include "spi.h"
#include "stm32f4xx.h"                  // Device header

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//SPI 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
//void SPI1_Init(void)
//{	 
//	u16 tempreg=0;
//	RCC->AHB1ENR|=1<<0;    	//使能PORTA时钟	   
//	RCC->APB2ENR|=1<<12;   	//SPI1时钟使能 
//	GPIO_Set(GPIOB,7<<3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PB3~5复用功能输出	
//  	GPIO_AF_Set(GPIOB,3,5);	//PB3,AF5
// 	GPIO_AF_Set(GPIOB,4,5);	//PB4,AF5
// 	GPIO_AF_Set(GPIOB,5,5);	//PB5,AF5 

//	//这里只针对SPI口初始化
//	RCC->APB2RSTR|=1<<12;	//复位SPI1
//	RCC->APB2RSTR&=~(1<<12);//停止复位SPI1
//	tempreg|=0<<10;			//全双工模式	
//	tempreg|=1<<9;			//软件nss管理
//	tempreg|=1<<8;			 
//	tempreg|=1<<2;			//SPI主机  
//	tempreg|=0<<11;			//8位数据格式	
//	tempreg|=1<<1;			//空闲模式下SCK为1 CPOL=1 
//	tempreg|=1<<0;			//数据采样从第2个时间边沿开始,CPHA=1  
// 	//对SPI1属于APB2的外设.时钟频率最大为84Mhz频率.
//	tempreg|=7<<3;			//Fsck=Fpclk1/256
//	tempreg|=0<<7;			//MSB First  
//	tempreg|=1<<6;			//SPI启动 
//	SPI1->CR1=tempreg; 		//设置CR1  
//	SPI1->I2SCFGR&=~(1<<11);//选择SPI模式
//	SPI1_ReadWriteByte(0xff);//启动传输		 
//}   
////SPI1速度设置函数
////SpeedSet:0~7
////SPI速度=fAPB2/2^(SpeedSet+1)
////fAPB2时钟一般为84Mhz
//void SPI1_SetSpeed(u8 SpeedSet)
//{
//	SpeedSet&=0X07;			//限制范围
//	SPI1->CR1&=0XFFC7; 
//	SPI1->CR1|=SpeedSet<<3;	//设置SPI1速度  
//	SPI1->CR1|=1<<6; 		//SPI设备使能	  
//} 
////SPI1 读写一个字节
////TxData:要写入的字节
////返回值:读取到的字节
//u8 SPI1_ReadWriteByte(u8 TxData)
//{		 			 
//	while((SPI1->SR&1<<1)==0);		//等待发送区空 
//	SPI1->DR=TxData;	 	  		//发送一个byte  
//	while((SPI1->SR&1<<0)==0);		//等待接收完一个byte  
// 	return SPI1->DR;          		//返回收到的数据				    
//}
void SPI3_Init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);               //  使能GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);                //  使能SPI2时钟

    //GPIOPB3、4、5初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;//  PB3、PB4、PB5复用功能输出	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                        //  复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                      //  推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                  //  100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                        //  上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);                              //  初始化

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);            //  PB10复用为 SPI2
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);            //  PB14复用为 SPI2
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);            //  PB15复用为 SPI2

    //这里只针对SPI口初始化
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);                //  复位SPI2
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);               //  停止复位SPI2

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //  设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //  设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //  设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                    //  串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                    //  串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //  NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//  定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //  指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //  CRC值计算的多项式
    SPI_Init(SPI3, &SPI_InitStructure);                                 //  根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPI3, ENABLE);                                              //  使能SPI外设

    SPI3_ReadWriteByte(0xff);//启动传输		 
}   

//SPI2 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
unsigned char SPI3_ReadWriteByte(unsigned char TxData)
{		 			 
    //  等待发送区空  
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
    {
    
    }
	
	SPI_I2S_SendData(SPI3, TxData);                                     //  通过外设SPIx发送一个byte  数据
		
    //等待接收完一个byte  
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)
    {
    
    } 
 
	return SPI_I2S_ReceiveData(SPI3);                                   //  返回通过SPIx最近接收的数据	
 		    
}







