#ifndef USART_H
#define USART_H

#include "stdio.h"
#include "stm32f4xx.h"
//  ����USART�������ݰ�
struct UsartData                                                        
{		
	unsigned char *Rxbuf;
    unsigned int   RXlenth;
    unsigned char  Time;
    unsigned char  ReceiveFinish;
};
typedef  struct UsartData USARTDATA;
typedef  USARTDATA       *PUSARTDATA;

// ��������
extern USARTDATA   Uart3;

// ��������
void UART3_Configuration(void);
void USART3_Senddata(unsigned char *Data, unsigned int length);
////////////
/*********************************************************************************************************
//I2C���Ŷ���
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

//  ��������
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
#define		USE_CH455_KEY		1			// ʹ����CH455�İ����ж�

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
// ����ϵͳ��������

#define CH455_BIT_ENABLE	0x01		// ����/�ر�λ
#define CH455_BIT_SLEEP		0x04		// ˯�߿���λ
#define CH455_BIT_7SEG		0x08		// 7�ο���λ
#define CH455_BIT_INTENS1	0x10		// 1������
#define CH455_BIT_INTENS2	0x20		// 2������
#define CH455_BIT_INTENS3	0x30		// 3������
#define CH455_BIT_INTENS4	0x40		// 4������
#define CH455_BIT_INTENS5	0x50		// 5������
#define CH455_BIT_INTENS6	0x60		// 6������
#define CH455_BIT_INTENS7	0x70		// 7������
#define CH455_BIT_INTENS8	0x00		// 8������

#define CH455_SYSOFF	0x0400			// �ر���ʾ���رռ���
#define CH455_SYSON		( CH455_SYSOFF | CH455_BIT_ENABLE )	// ������ʾ������
#define CH455_SLEEPOFF	CH455_SYSOFF	// �ر�˯��
#define CH455_SLEEPON	( CH455_SYSOFF | CH455_BIT_SLEEP )	// ����˯��
#define CH455_7SEG_ON	( CH455_SYSON | CH455_BIT_7SEG )	// �����߶�ģʽ
#define CH455_8SEG_ON	( CH455_SYSON | 0x00 )	// �����˶�ģʽ
#define CH455_SYSON_4	( CH455_SYSON | CH455_BIT_INTENS4 )	// ������ʾ�����̡�4������
#define CH455_SYSON_8	( CH455_SYSON | CH455_BIT_INTENS8 )	// ������ʾ�����̡�8������


// ��������������
#define CH455_DIG0		0x1400			// �����λ0��ʾ,�����8λ����
#define CH455_DIG1		0x1500			// �����λ1��ʾ,�����8λ����
#define CH455_DIG2		0x1600			// �����λ2��ʾ,�����8λ����
#define CH455_DIG3		0x1700			// �����λ3��ʾ,�����8λ����



// ��ȡ������������
#define CH455_GET_KEY	0x0700					// ��ȡ����,���ذ�������


// CH455�ӿڶ���
#define		CH455_I2C_ADDR		0x40			// CH455�ĵ�ַ
#define		CH455_I2C_MASK		0x3E			// CH455�ĸ��ֽ���������

extern u8 Alarm_flag;                                                   //  ������־λ

// �����ӳ���
extern	u8 CH455_Read( void );				// ��CH455��ȡ��������
extern  void CH455_Write( u16 cmd );	// ��CH455������������

void init_CH455(void);
void Alarm_GPIO_Configuration(void);

#endif
