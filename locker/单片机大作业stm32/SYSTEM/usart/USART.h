#ifndef USART_H
#define USART_H

#include "stdio.h"
#include "stm32f4xx.h"
//  用于USART接收数据包
struct UsartData                                                        
{		
	unsigned char *Rxbuf;
    unsigned int   RXlenth;
    unsigned char  Time;
    unsigned char  ReceiveFinish;
};
typedef  struct UsartData USARTDATA;
typedef  USARTDATA       *PUSARTDATA;

// 变量声明
extern USARTDATA   Uart3;

// 函数声明
void UART3_Configuration(void);
void USART3_Senddata(unsigned char *Data, unsigned int length);
////////////
/*********************************************************************************************************
//I2C引脚定义
//SCL	PA1
//SDA	PA0
*********************************************************************************************************/
//SCL		PA1
#define  	SCL_L      		GPIO_ResetBits(GPIOG, GPIO_Pin_2)
#define  	SCL_H      		GPIO_SetBits(GPIOG, GPIO_Pin_2)

//SDA		PA0
#define  	SDA_L      		GPIO_ResetBits(GPIOG, GPIO_Pin_4)
#define  	SDA_H      		GPIO_SetBits(GPIOG, GPIO_Pin_4)

#define  	SDA_READ()     	GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_4)

//  函数声明
void I2C_GPIO_Configuration(void);
void I2C_SDA_Input(void);
void I2C_SDA_Output(void);
void I2C_delay(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendACK(unsigned char ack);
void I2C_ReceiveACK(void);
void I2C_SendByte(unsigned char dat);
unsigned char I2C_ReceiveByte(void);
//////////////
#define		USE_CH455_KEY		1			// 使用了CH455的按键中断

#ifdef USE_CH455_KEY
	#define		DISABLE_KEY_INTERRUPT	{EX1=0;}
	#define		ENABLE_KEY_INTERRUPT	{EX1=1;}
	#define		CLEAR_KEY_INTER_FLAG	{IE1=0;}
#else
	#define		DISABLE_KEY_INTERRUPT	{}
	#define		ENABLE_KEY_INTERRUPT	{}
	#define		CLEAR_KEY_INTER_FLAG	{}
#endif

/* ********************************************************************************************* */
// 设置系统参数命令

#define CH455_BIT_ENABLE	0x01		// 开启/关闭位
#define CH455_BIT_SLEEP		0x04		// 睡眠控制位
#define CH455_BIT_7SEG		0x08		// 7段控制位
#define CH455_BIT_INTENS1	0x10		// 1级亮度
#define CH455_BIT_INTENS2	0x20		// 2级亮度
#define CH455_BIT_INTENS3	0x30		// 3级亮度
#define CH455_BIT_INTENS4	0x40		// 4级亮度
#define CH455_BIT_INTENS5	0x50		// 5级亮度
#define CH455_BIT_INTENS6	0x60		// 6级亮度
#define CH455_BIT_INTENS7	0x70		// 7级亮度
#define CH455_BIT_INTENS8	0x00		// 8级亮度

#define CH455_SYSOFF	0x0400			// 关闭显示、关闭键盘
#define CH455_SYSON		( CH455_SYSOFF | CH455_BIT_ENABLE )	// 开启显示、键盘
#define CH455_SLEEPOFF	CH455_SYSOFF	// 关闭睡眠
#define CH455_SLEEPON	( CH455_SYSOFF | CH455_BIT_SLEEP )	// 开启睡眠
#define CH455_7SEG_ON	( CH455_SYSON | CH455_BIT_7SEG )	// 开启七段模式
#define CH455_8SEG_ON	( CH455_SYSON | 0x00 )	// 开启八段模式
#define CH455_SYSON_4	( CH455_SYSON | CH455_BIT_INTENS4 )	// 开启显示、键盘、4级亮度
#define CH455_SYSON_8	( CH455_SYSON | CH455_BIT_INTENS8 )	// 开启显示、键盘、8级亮度


// 加载字数据命令
#define CH455_DIG0		0x1400			// 数码管位0显示,需另加8位数据
#define CH455_DIG1		0x1500			// 数码管位1显示,需另加8位数据
#define CH455_DIG2		0x1600			// 数码管位2显示,需另加8位数据
#define CH455_DIG3		0x1700			// 数码管位3显示,需另加8位数据



// 读取按键代码命令
#define CH455_GET_KEY	0x0700					// 获取按键,返回按键代码


// CH455接口定义
#define		CH455_I2C_ADDR		0x40			// CH455的地址
#define		CH455_I2C_MASK		0x3E			// CH455的高字节命令掩码

extern u8 Alarm_flag;                                                   //  报警标志位

// 对外子程序
extern	u8 CH455_Read( void );				// 从CH455读取按键代码
extern  void CH455_Write( u16 cmd );	// 向CH455发出操作命令

void init_CH455(void);
void Alarm_GPIO_Configuration(void);

#endif
